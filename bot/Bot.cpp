#include "Bot.hpp"

#include "util/vargs/container_of.hpp"

using util::p;

const Bot::CmdMap Bot::map_ = container_of<CmdMap, CmdMap::value_type>(
    p("PRIVMSG", &Bot::privmsg), p("PING", &Bot::ping));

Bot::Bot(const BotConfig config) : sock_(-1), config_(config) {}

Bot::~Bot() {
  if (sock_ != -1) {
    send("QUIT :goodbye");
    close(sock_);
  }
}

result_t::e Bot::init(int backlog) {
  if (pool_.init(backlog) == result_t::kError)
    return result_t::kError;

  sock_ = util::socket(PF_INET, SOCK_STREAM);
  if (sock_ == -1)
    return result_t::kError;

  struct sockaddr_in addr;
  std::memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(config_.server_ip.c_str());
  if (addr.sin_addr.s_addr == INADDR_NONE)
    return result_t::kError;
  addr.sin_port = htons(config_.port);

  if (util::connect(sock_, reinterpret_cast<const struct sockaddr *>(&addr),
                    sizeof(addr)) == -1) {
    return result_t::kError;
  }

  pool_.addEvent(EventKind::kRead, this);

  send(FMT("PASS {password}", (config_.password)));
  send(FMT("USER msbot msbot {server} :msbot", (config_.server_ip)));
  send("NICK msbot");
  send("JOIN #minesweeper");
  return result_t::kOK;
}

void Bot::loop() { pool_.dispatchEvent(1); }

result_t::e Bot::handle(Event e) {
  result_t::e result;
  switch (e.kind) {
    case EventKind::kRead:
      result = handleReadEvent(e);
      break;

    case EventKind::kWrite:
      result = handleWriteEvent(e);
      break;

    default:
      result = result_t::kError;
      break;
  }
  return result;
}

int Bot::getFd() const { return sock_; }

void Bot::send(const std::string &str) {
  send_queue_.push(StringBuffer(str + "\r\n"));
  pool_.addEvent(EventKind::kWrite, this);
}

void Bot::sendTo(const std::string &target, const std::string &str) {
  send(FMT("PRIVMSG {target} :{message}", (target, str)));
}

result_t::e Bot::handleWriteEvent(Event &e) {
  StringBuffer &buffer = send_queue_.front();

  if (e.flags.test(EventFlag::kEOF)) {
    return result_t::kError;
  }

  std::size_t write_size =
      std::min(buffer.size(), static_cast<std::size_t>(e.data));

  ssize_t send_length = util::send(getFd(), buffer.data(), write_size);
  if (send_length == -1) {
    return result_t::kError;
  } else {
    buffer.advance(send_length);
  }

  if (buffer.empty()) {
    send_queue_.pop();
    if (send_queue_.empty()) {
      pool_.removeEvent(EventKind::kWrite, this);
    }
  }
  return result_t::kOK;
}

ssize_t Bot::recvToBuffer(size_t length) {
  ssize_t ret = util::recv(sock_, recv_buffer_.begin(),
                           std::min(length, recv_buffer_.max_size()));
  if (ret == -1)
    return ret;
  recv_buffer_.seekg(0);
  recv_buffer_.seekp(ret);
  return ret;
}

result_t::e Bot::handleReceive(Event &e) {
  ssize_t len = e.data;

  if (e.data != 0)
    len = recvToBuffer(e.data);

  if (len <= 0 || e.flags.test(EventFlag::kEOF))
    return result_t::kError;

  return result_t::kOK;
}

result_t::e Bot::handleReadEvent(Event &e) {
  if (handleReceive(e) == result_t::kError) {
    return result_t::kError;
  } else {
    ParserResult::e result;
    while ((result = parser_.parse(recv_buffer_)) == ParserResult::kSuccess) {
      Message msg = parser_.getMessage();
      result_t::e result = processMessage(msg);
      if (result != result_t::kOK)
        return result;
    }
    if (result == ParserResult::kFailure) {
      return result_t::kError;
    }
  }
  return result_t::kOK;
}

result_t::e Bot::processMessage(const Message &m) {
  CmdMap::const_iterator it = map_.find(util::to_upper(m.command));

  const bool found = it != map_.end();
  if (found)
    return (this->*(it->second))(m);
  return result_t::kOK;
}

void Bot::sendBoardTo(const board_type &board, bool mask_board,
                      const std::string &nickname) {
  std::stringstream ss(board.toString(mask_board));

  std::string line;
  while (std::getline(ss, line)) {
    sendTo(nickname, line);
  }
}

void Bot::checkState(MSMap::iterator user) {
  board_type &board = user->second;
  const std::string &nickname = user->first;

  switch (board.getState()) {
    case GameState::kContinue:
      sendBoardTo(board, true, nickname);
      break;

    case GameState::kMineExploded:
      sendBoardTo(board, false, nickname);
      sendTo(nickname, "mine exploded!! :c");
      user_map_.erase(user);
      break;

    case GameState::kMineSwept:
      sendBoardTo(board, false, nickname);
      sendTo(nickname, "mine swept!! :p");
      user_map_.erase(user);
      break;
  }
}

result_t::e Bot::parseCoord(const std::string &x, const std::string &y,
                            std::pair<int, int> &result) {
  if (x.size() != 1 || y.size() != 1) {
    return result_t::kError;
  }
  if (!std::isdigit(x[0]) || x[0] == 0 || !std::isalpha(y[0])) {
    return result_t::kError;
  }
  result.first = x[0] - '0' - 1;
  result.first = std::toupper(y[0]) - 'A';
  return result_t::kOK;
}
//========================================================================================================================================

#include <algorithm>
#include <iostream>

#include "MineSweeper.hpp"
#include "util/algorithm/functor.hpp"
#include "util/strutil/format.hpp"
struct command {
  bool typed_x;
  bool typed_y;
  pos p;
  GameAction::e action;

  command() : typed_x(false), typed_y(false), action(GameAction::kOpen) {}

  bool is_typed() const { return typed_x && typed_y; }

  void parseline(std::string s);
};

/**
 * @brief 메인 게임 루프
 *
 * 명령어 일람
 * - (a-jA-J): x 좌표 입력
 * - (0-9): y 좌표 입력
 * - p, (m)ark: 깃발 추가 및 제거
 * - x, (q)uit: 게임 종료
 */
void Bot::runGame(MSMap::iterator user, std::string line) {
  MineSweeper<9, 9> &ms = user->second;
  const std::string &nickname = user->first;

  FUNCTOR(void, erase_spaces, (std::string & str), {
    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
  });

  erase_spaces(line);
  command cmd;
  for (std::string::iterator it = line.begin(); it != line.end(); ++it) {
    const char c = tolower(*it);
    if (isdigit(c)) {
      cmd.typed_y = true;
      cmd.p.y = c - '0';
    } else {
      switch (c) {
        case 'p':
        case 'm':
          cmd.action = GameAction::kFlag;
          break;

        case 'x':
        case 'q':
          cmd.action = GameAction::kExit;
          break;

        default:
          cmd.typed_x = true;
          cmd.p.x = c - 'a';
          break;
      }
    }
  }
  if (cmd.action == GameAction::kExit) {
    sendTo(nickname, "goodbye...");
    user_map_.erase(user);
    return;
  }
  if (not cmd.is_typed()) {
    sendTo(nickname, "command is missing.");
    return;
  }
  if (not ms.isInBoard(cmd.p)) {
    sendTo(nickname, FMT("{}, {} is not in board", (cmd.p.x, cmd.p.y)));
    return;
  }
  switch (cmd.action) {
    case GameAction::kFlag:
      ms.toggleFlag(cmd.p);
      break;
    default:
      ms.exmine(cmd.p);
      break;
  }
  sendBoardTo(ms, true, nickname);
  GameState::e state = ms.getState();
  if (state == GameState::kMineSwept) {
    sendBoardTo(ms, false, nickname);
    sendTo(nickname, "All mine swept. congratulations!!!");
    user_map_.erase(user);
  } else if (state == GameState::kMineExploded) {
    sendBoardTo(ms, false, nickname);
    sendTo(nickname, "Mine exploded. try again...");
    user_map_.erase(user);
  }
}

//========================================================================================================================================
result_t::e Bot::privmsg(const Message &m) {
  const std::string &msg_string = m.params[1];

  std::size_t nick_end = m.prefix.find("!");

  if (nick_end == std::string::npos)
    return result_t::kOK;  // 메시지 무시

  std::string nickname = m.prefix.substr(0, nick_end);

  MSMap::iterator user = user_map_.find(nickname);

  if (msg_string == "!start") {
    // 등록이 필요한 경우
    if (user == user_map_.end()) {
      const board_type &board =
          user_map_.insert(MSMap::value_type(nickname, board_type(0.1)))
              .first->second;
      sendTo(nickname, FMT("{nickname} - you are now registered!", (nickname)));
      sendBoardTo(board, true, nickname);
      sendTo(nickname, "ex) type c0 to open a tile");
      sendTo(nickname, "cmd: e(x)it | (p)ut a flag");
      sendTo(nickname, "x_coord: [1-9]");
      sendTo(nickname, "y_coord: [A-I]");
    }
    // 이미 등록된 경우
    else {
      sendTo("#minesweeper",
             FMT("{nickname} - you are already registered!", (nickname)));
    }
  } else {
    if (user == user_map_.end()) {
      sendTo("#minesweeper",
             FMT("{nickname} - type '!start' to start new game", (nickname)));
    } else {
      runGame(user, msg_string);
    }
  }
  return result_t::kOK;
}

result_t::e Bot::ping(const Message &m) {
  if (m.params.size() != 1) {
    send(Message::as_numeric_reply(util::ERR_NOORIGIN,
                                   VA(("No origin specified"))));
    return result_t::kOK;
  }
  send(FMT("PONG {origin}", (m.params[0])));
  return result_t::kOK;
}
