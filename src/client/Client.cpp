#include "Client.hpp"

#include <unistd.h>

#include <iostream>

#include "returncode.hpp"
#include "server/Server.hpp"
#include "util/algorithm/algorithm.hpp"
#include "util/color.hpp"
#include "util/config/config.hpp"
#include "util/general/logging.hpp"
#include "util/irctype/irctype.hpp"
#include "util/strutil/conversion.hpp"
#include "util/strutil/format.hpp"
#include "util/vargs/container_of.hpp"
#include "version.hpp"

using util::p;

const Client::CmdMap Client::map_before_register_ =
    container_of<CmdMap, CmdMap::value_type>(
        p("PING", &Client::notRegistered), p("QUIT", &Client::notRegistered),
        p("JOIN", &Client::notRegistered), p("PRIVMSG", &Client::notRegistered),
        p("KILL", &Client::notRegistered), p("OPER", &Client::notRegistered),
        p("MODE", &Client::notRegistered), p("PART", &Client::notRegistered),
        p("PASS", &Client::registerPass), p("USER", &Client::registerUser),
        p("NICK", &Client::registerNick), p("PONG", &Client::notRegistered));

const Client::CmdMap Client::map_after_register_ =
    container_of<CmdMap, CmdMap::value_type>(
        p("PING", &Client::ping), p("QUIT", &Client::quit),
        p("JOIN", &Client::join), p("PRIVMSG", &Client::privmsg),
        p("KILL", &Client::kill), p("OPER", &Client::oper),
        p("MODE", &Client::mode), p("PART", &Client::part),
        p("PASS", &Client::alredyRegistered),
        p("USER", &Client::alredyRegistered), p("NICK", &Client::nick),
        p("PONG", &Client::pong));
/*CLIENT===============================*/

Client::Client(int sock, struct sockaddr_in addr, ClientList::iterator pos)
    : map_(&map_before_register_),
      sock_(sock),
      pos_(pos),
      event_state_(EventKind::kNone),
      conn_state_(ConnState::kClear) {
  addEvent(EventKind::kRead);
  server.getPool().addTimer(TimerKind::kRegister, this, server.config_.timeout);
  ident_.addr_ = addr;
}

Client::~Client() {
  if (conn_state_ == ConnState::kRegistered) {
    server.getPool().removeTimer(TimerKind::kPing, this, server.config_.ping);
    for (ChannelMap::iterator it = joined_channels_.begin(),
                              end = joined_channels_.end();
         it != end; ++it) {
      it->second->removeUser(this);
    }
  }
  server.getPool().removeTimer(TimerKind::kRegister, this,
                               server.config_.timeout);
  server.getPool().removeTimer(TimerKind::kPong, this, server.config_.timeout);
  server.getPool().removeTimer(TimerKind::kPing, this, server.config_.ping);
  close(sock_);
}

int Client::getFd() const { return sock_; }

void Client::send(const std::string &str) {
  util::debug_output(str);
  send_queue_.push(StringBuffer(str + "\r\n"));
  addEvent(EventKind::kWrite);
}

void Client::sendRegisterMessage() {
  util::debug_info("sending register message to", ident_.nickname_);
  send(Message::as_numeric_reply(
      util::RPL_WELCOME,
      VA((ident_.nickname_, "Welcome to the Internet Relay Network!"))));
  send(Message::as_numeric_reply(
      util::RPL_YOURHOST,
      VA((ident_.nickname_,
          FMT("Your host is {servername}, running version {version}",
              (server.config_.name, VERSION_STRING))))));
  send(Message::as_numeric_reply(
      util::RPL_CREATED,
      VA((ident_.nickname_, FMT("This server was created {create_time}",
                                (server.config_.create_time))))));
  send(Message::as_numeric_reply(util::RPL_MYINFO,
                                 VA((ident_.nickname_, server.config_.name,
                                     VERSION_STRING, "o", "o", ""))));
  sendMOTD();
}

result_t::e Client::handleWriteEvent(Event &e) {
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
      removeEvent(EventKind::kWrite);
    }
  }
  return result_t::kOK;
}

void Client::sendMOTD() {
  std::stringstream ss(server.config_.motd);

  std::string line;
  while (std::getline(ss, line)) {
    send(Message::as_numeric_reply(util::RPL_MOTD,
                                   VA((ident_.nickname_, line))));
  }
  send(Message::as_numeric_reply(util::RPL_ENDOFMOTD,
                                 VA((ident_.nickname_, "End of MOTD"))));
}

void Client::sendNotice(const std::string &msg) {
  send(FMT(":{servername} NOTICE {nickname} :{message}",
           (server.config_.name, ident_.nickname_, msg)));
}

void Client::sendError(const std::string &msg) {
  send(FMT("ERROR :{message}", (msg)));
}

void Client::sendPing() {
  send(Message::as_reply("", "PING", VA((server.config_.name))));
}

const UserIdent &Client::getIdent() { return ident_; }

result_t::e Client::processMessage(const Message &m) {
  CmdMap::const_iterator it = map_->find(util::to_upper(m.command));

  const bool found = it != map_->end();
  util::debug_input(std::string(m), found);
  if (found)
    return (this->*(it->second))(m);
  return result_t::kOK;
}

result_t::e Client::handleTimerEvent(Event &e) {
  switch (e.data) {
    case TimerKind::kRegister:
      sendError("Closing link: Registration timed out.");
      return result_t::kClosing;

    case TimerKind::kPing:
      server.getPool().addTimer(TimerKind::kPong, this, server.config_.timeout);
      sendPing();
      return result_t::kOK;

    case TimerKind::kPong:
      sendError(
          FMT("Closing link: ({userinfo}) [Ping timeout: {timeout} seconds]",
              (ident_.toString(), server.config_.timeout)));
      return result_t::kClosing;

    default:
      return result_t::kError;
  }
}

result_t::e Client::handleReceive(Event &e) {
  ssize_t len = e.data;

  if (e.data != 0)
    len = recvToBuffer(e.data);

  if (len <= 0 || e.flags.test(EventFlag::kEOF))
    return result_t::kError;

  return result_t::kOK;
}

result_t::e Client::handleReadEvent(Event &e) {
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

ssize_t Client::recvToBuffer(size_t length) {
  ssize_t ret = util::recv(sock_, recv_buffer_.begin(), length);
  if (ret == -1)
    return ret;
  recv_buffer_.seekg(0);
  recv_buffer_.seekp(ret);
  return ret;
}

result_t::e Client::handle(Event e) {
  result_t::e result;
  switch (e.kind) {
    case EventKind::kRead:
      result = handleReadEvent(e);
      break;
    case EventKind::kWrite:
      result = handleWriteEvent(e);
      break;
    case EventKind::kTimer:
      result = handleTimerEvent(e);
      break;
    default:
      result = result_t::kError;
      break;
  }
  if (result == result_t::kClosing) {
    removeEvent(EventKind::kRead);
    result = result_t::kOK;
  }
  if (event_state_ == EventKind::kNone)
    return result_t::kError;
  return result;
}

void Client::handleError() {
  if (conn_state_ == ConnState::kRegistered)
    server.removeClient(ident_.nickname_);
  else
    server.removeClient(pos_);
}

int Client::addEvent(EventKind::e kind) {
  int result = result_t::kOK;
  if (!(event_state_ & kind)) {
    result = server.getPool().addEvent(kind, this);
    event_state_ |= kind;
  }
  return result;
}

int Client::removeEvent(EventKind::e kind) {
  int result = result_t::kOK;
  if ((event_state_ & kind)) {
    result = server.getPool().removeEvent(kind, this);
    event_state_ &= (~kind);
  }
  return result;
}

result_t::e Client::pong(const Message &m) {
  (void)m;
  server.getPool().removeTimer(TimerKind::kPong, this, server.config_.timeout);
  return result_t::kOK;
}

result_t::e Client::ping(const Message &m) {
  if (m.params.size() != 1) {
    send(Message::as_numeric_reply(util::ERR_NOORIGIN,
                                   VA(("No origin specified"))));
    return result_t::kOK;
  }
  server.getPool().removeTimer(TimerKind::kPong, this, server.config_.timeout);
  server.getPool().addTimer(TimerKind::kPing, this, server.config_.ping);
  send(FMT(":{0} PONG {0} :{1}", (server.config_.name, m.params[0])));
  return result_t::kOK;
}

result_t::e Client::oper(const Message &m) {
  if (m.params.size() < 2) {
    send(Message::as_not_enough_params_reply(m.command));
    return result_t::kOK;
  }

  const bool is_ok = server.config_.oper_user == m.params[0] &&
                     server.config_.oper_password == m.params[1];

  if (is_ok) {
    ident_.mode_ |= UserMode::o;
    send(Message::as_numeric_reply(
        util::RPL_YOUAREOPER,
        VA((ident_.nickname_, "You are now an IRC operator"))));
    return result_t::kOK;
  } else {  // 보안적인 이유로 비밀번호가 틀렸을 때, 유저네임이 틀렸을 때 모두
            // 같은 에러로 응답함.
    send(Message::as_numeric_reply(
        util::ERR_NOOPERHOST,
        VA((ident_.nickname_, "Invalid oper credentials"))));
    return result_t::kOK;
  }
}

result_t::e Client::kill(const Message &m) {
  if (m.params.size() < 2) {
    send(Message::as_not_enough_params_reply(m.command));
    return result_t::kOK;
  }

  ClientMap &clients = server.getClients();
  ClientMap::iterator target_it = clients.find(m.params[0]);
  if (target_it == clients.end()) {
    send(Message::as_reply("", util::pad_num(util::ERR_NOSUCHNICK),
                           VA((m.params[0], "No such nick/channel"))));
    return result_t::kOK;
  }

  bool has_privilege = (ident_.mode_ & UserMode::o);
  if (not has_privilege) {
    send(Message::as_reply(
        "", util::pad_num(util::ERR_NOPRIVILEGES),
        VA((ident_.nickname_,
            "Permission Denied- You're not an IRC operator"))));
  } else {
    Client &target = *target_it->second;
    target.send(Message::as_reply(ident_.toString(), "KILL",
                                  VA((ident_.nickname_, "Killed"))));
    target.sendError(
        FMT("Closing link: {ident} [ Killed ]", (ident_.toString())));
    target.removeEvent(EventKind::kRead);
  }
  return result_t::kOK;
}

result_t::e Client::quit(const Message &m) {
  Message reply = m;
  reply.prefix = ident_.toString();

  for (ChannelMap::iterator it = joined_channels_.begin(),
                            end = joined_channels_.end();
       it != end; ++it) {
    Channel &channel = *it->second;
    channel.sendAll(FMT("ERROR :Closing link: ({user}@{host}) [{msg}]",
                        (ident_.username_, ident_.hostname_, m.params[0])),
                    this);  // FIXME hostname이 아직 제대로 동작 안함
  }
  return result_t::kClosing;
}

result_t::e Client::join(const Message &m) {
  if (m.params.empty()) {
    send(Message::as_not_enough_params_reply(m.command));
    return result_t::kOK;
  }

  if (m.params[0] == "0") {
    part(
        Message::as_reply(server.config_.name, "PART",
                          VA((util::join(util::keys(joined_channels_), ",")))));
    return result_t::kOK;
  }

  std::vector<std::string> channels = util::split(m.params[0], ",");
  for (std::vector<std::string>::iterator it = channels.begin(),
                                          end = channels.end();
       it != end; ++it) {
    Channel *new_channel = server.addUserToChannel(*it, this);
    if (new_channel) {
      joined_channels_.insert(std::make_pair(*it, new_channel));
      // send(Message::as_numeric_reply(util::RPL_TOPIC, VA((*it, ""))));

      sendList(FMT(":{server} {code} {nick} = {channel} :",
                   (server.config_.name, util::pad_num(util::RPL_NAMREPLY),
                    ident_.nickname_, *it)),
               util::keys(new_channel->getUsers()),
               Message::as_numeric_reply(
                   util::RPL_ENDOFNAMES,
                   VA((ident_.nickname_, *it, "End of /NAMES"))));
    } else {
      send(Message::as_numeric_reply(util::ERR_NOSUCHCHANNEL,
                                     VA((*it, "No such channel"))));
    }
  }
  return result_t::kOK;
}

result_t::e Client::part(const Message &m) {
  if (m.params.empty()) {
    send(Message::as_not_enough_params_reply(m.command));
    return result_t::kOK;
  }

  std::vector<std::string> channels = util::split(m.params[0], ",");
  for (std::vector<std::string>::iterator it = channels.begin(),
                                          end = channels.end();
       it != end; ++it) {
    ChannelMap::iterator channel = joined_channels_.find(*it);
    if (channel == joined_channels_.end()) {
      send(Message::as_numeric_reply(util::ERR_NOTONCHANNEL,
                                     VA((*it, "You're not on that channel"))));
    } else {
      Channel *ch = channel->second;
      ch->removeUser(this);
      joined_channels_.erase(channel);
      if (ch->getUsers().empty()) {
        server.removeChannel(*it);
      }
    }
  }
  return result_t::kOK;
}

/// mode is not supported.
result_t::e Client::mode(const Message &m) {
  (void)m;
  send(Message::as_numeric_reply(util::ERR_UMODEUNKNOWNFLAG,
                                 VA((ident_.nickname_, "Unknown MODE flag"))));
  return result_t::kOK;
}

result_t::e Client::privmsg(const Message &m) {
  if (m.params.empty() or m.params[0].empty()) {
    send(Message::as_numeric_reply(util::ERR_NORECIPIENT, VA((m.command))));
    return result_t::kOK;
  }
  if (m.params.size() < 2 or m.params[1].empty()) {
    send(Message::as_numeric_reply(util::ERR_NOTEXTTOSEND,
                                   VA(("No text to send"))));
    return result_t::kOK;
  }
  const Message reply = {ident_.toString(), m.command, m.params};
  const std::string &recipient = reply.params[0];

  if (util::isChannelPrefix(recipient.front())) {
    ChannelMap &channels = server.getChannels();
    ChannelMap::iterator it = channels.find(recipient);
    if (it == channels.end()) {
      send(Message::as_numeric_reply(util::ERR_NOSUCHNICK,
                                     VA(("No such channel"))));
      return result_t::kOK;
    } else {
      it->second->sendAll(reply, this);
    }
  } else {
    ClientMap &clients = server.getClients();
    ClientMap::iterator it = clients.find(recipient);
    if (it == clients.end()) {
      send(Message::as_numeric_reply(util::ERR_NOSUCHNICK,
                                     VA(("No such nick"))));
      return result_t::kOK;
    } else {
      it->second->send(reply);
    }
  }
  return result_t::kOK;
}

result_t::e Client::notRegistered(const Message &m) {
  (void)m;
  send(Message::as_numeric_reply(util::ERR_NOTREGISTERED,
                                 VA(("You have not registered"))));
  return result_t::kOK;
}

result_t::e Client::registerPass(const Message &m) {
  if (m.params.empty()) {
    send(Message::as_not_enough_params_reply(m.command));
    return result_t::kOK;
  }

  if (m.params[0] != server.config_.password) {
    // 표준에서는 이 상황에서 ERR_PASSWDMISMATCH를 반환하지만, 보안상의 이유로
    // 일반화된 에러를 반환함.
    sendError("Closing link: [Access denied by configuration]");
    return result_t::kClosing;
  }

  ident_.password_ = m.params[0];
  conn_state_ |= ConnState::kPass;

  return result_t::kOK;
}

result_t::e Client::registerUser(const Message &m) {
  if (!(conn_state_ & ConnState::kPass)) {
    sendError("PASS needed before USER command.");
    return result_t::kClosing;
  }

  if (m.params.size() != 4) {
    send(Message::as_not_enough_params_reply(m.command));
    return result_t::kOK;
  }

  ident_.username_ = m.params[0];

  // TODO rDNS 쿼리로 검증
  // ident_.hostname_ = m.params[1];
  ident_.hostname_ = inet_ntoa(ident_.addr_.sin_addr);

  ident_.servername_ = m.params[2];
  ident_.realname_ = m.params[3];

  conn_state_ |= ConnState::kUser;
  if (conn_state_ == ConnState::kRegistered)
    completeRegister();
  return result_t::kOK;
}

bool isValidUser(const std::string &s) {
  return s.find("@") != std::string::npos;
}

bool isValidFirstNickChar(char c) {
  return util::isLetter(c) || util::isSpecial(c);
}

bool isValidNickChar(char c) {
  return util::isLetter(c) || util::isDigit(c) || util::isSpecial(c) ||
         c == '-';
}

bool isValidNick(const std::string &nick) {
  if (nick.size() > 9)
    return false;
  if (!isValidFirstNickChar(nick[0]))
    return false;
  for (std::string::const_iterator it = util::next(nick.begin()),
                                   end = nick.end();
       it != end; ++it) {
    if (!isValidNickChar(*it)) {
      return false;
    }
  }
  return true;
}

result_t::e Client::registerNick(const Message &m) {
  if (!(conn_state_ & ConnState::kPass)) {
    sendError("PASS needed before NICK command.");
    return result_t::kClosing;
  }
  if (m.params.empty()) {
    send(Message::as_numeric_reply(util::ERR_NONICKNAMEGIVEN,
                                   VA(("No nickname given"))));
    return result_t::kOK;
  }

  const std::string &nick = m.params[0];
  if (!isValidNick(nick)) {
    send(Message::as_numeric_reply(util::ERR_ERRONEUSNICKNAME,
                                   VA((nick, "Erroneous nickname"))));
    return result_t::kOK;
  }

  if (server.getClients().find(nick) != server.getClients().end()) {
    send(Message::as_numeric_reply(util::ERR_NICKNAMEINUSE,
                                   VA((nick, "Nickname is already in use"))));
    return result_t::kOK;
  }
  ident_.nickname_ = nick;
  conn_state_ |= ConnState::kNick;
  if (conn_state_ == ConnState::kRegistered)
    completeRegister();

  return result_t::kOK;
}

void Client::completeRegister() {
  server.eraseFromClientList(pos_);
  server.addClient(ident_.nickname_, this);

  server.getPool().addTimer(TimerKind::kPing, this, server.config_.ping);
  server.getPool().removeTimer(TimerKind::kRegister, this,
                               server.config_.timeout);

  updateCommandMap();
  sendRegisterMessage();
}

void Client::updateCommandMap() { map_ = &map_after_register_; }

result_t::e Client::alredyRegistered(const Message &m) {
  (void)m;
  send(Message::as_numeric_reply(util::ERR_ALREADYREGISTERED,
                                 VA(("You may not register"))));
  return result_t::kOK;
}

result_t::e Client::nick(const Message &m) {
  if (m.params.empty()) {
    send(Message::as_numeric_reply(util::ERR_NONICKNAMEGIVEN,
                                   VA(("No nickname given"))));
    return result_t::kOK;
  }

  const std::string &newnick = m.params[0];
  if (!isValidNick(newnick)) {
    send(Message::as_numeric_reply(
        util::ERR_ERRONEUSNICKNAME,
        VA((ident_.nickname_, newnick, "Erroneous nickname"))));
    return result_t::kOK;
  }

  ClientMap clients = server.getClients();
  if (clients.find(newnick) != clients.end()) {
    send(Message::as_numeric_reply(
        util::ERR_NICKNAMEINUSE,
        VA((ident_.nickname_, newnick, "Nickname is already in use"))));
    return result_t::kOK;
  }

  server.eraseFromClientMap(ident_.nickname_);
  server.addClient(newnick, this);

  const std::string oldnick = ident_.nickname_;

  for (ChannelMap::iterator it = joined_channels_.begin(),
                            end = joined_channels_.end();
       it != end; ++it) {
    Channel &channel = *it->second;
    channel.changeUserName(oldnick, newnick);
    channel.sendAll(Message::as_reply(oldnick, "NICK", VA((newnick))), this);
  }

  ident_.nickname_ = newnick;

  send(Message::as_reply(oldnick, "NICK", VA((newnick))));

  return result_t::kOK;
}

/*

:test1!root@root JOIN :#test
:exictingirc 353 test1 = #test :test1
:exictingirc 366 #test :End of /NAMES

:test1!root@127.0.0.1 JOIN :#test
:irc.local 353 test1 = #test :@test1
:irc.local 366 test1 #test :End of /NAMES list.

*/
