#ifndef BOT_HPP
#define BOT_HPP

#include <unistd.h>

#include <map>
#include <queue>

#include "MineSweeper.hpp"
#include "client/IRCParser.hpp"
#include "event/Event.hpp"
#include "event/EventPool.hpp"
#include "util/StringBuffer.hpp"

struct BotConfig {
  std::string password;
  std::string server_ip;
  uint16_t port;
};

typedef std::map<std::string, MineSweeper<9, 9> > MSMap;

class Bot : public IEventHandler {
 public:
  Bot(const BotConfig config);
  ~Bot();

 private:
  Bot(const Bot &);             // = delete;
  Bot &operator=(const Bot &);  // = delete;

 private:
  typedef std::map<std::string, result_t::e (Bot::*)(const Message &)> CmdMap;
  typedef MineSweeper<9, 9> board_type;

 public:
  result_t::e init(int backlog);

  result_t::e handle(Event e);

  void dispatchEvent();

  void handleError() {}

  int getFd() const;

  void send(const std::string &str);

  void sendTo(const std::string &target, const std::string &str);

  void sendSyncTimeout(const std::string &str, int sec);

 private:
  result_t::e handleWriteEvent(Event &e);

  ssize_t recvToBuffer(size_t length);

  result_t::e handleReceive(Event &e);

  result_t::e handleReadEvent(Event &e);

  result_t::e processMessage(const Message &m);

  void sendBoardTo(const board_type &board, bool mask_board,
                   const std::string &nickname);

  void checkState(MSMap::iterator user);

  void runGame(MSMap::iterator user, std::string line);

  result_t::e privmsg(const Message &m);
  result_t::e ping(const Message &m);

 private:
  int sock_;
  EventPool pool_;
  IRCParser parser_;

  util::Buffer recv_buffer_;
  std::queue<StringBuffer> send_queue_;
  MSMap user_map_;
  const static CmdMap map_;

  const BotConfig config_;
};

#endif
