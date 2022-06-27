#include "client/Client.hpp"
#include "server/Server.hpp"

void Message::clear() {
  prefix.clear();
  command.clear();
  params.clear();
}

Message::operator std::string() const { return util::to_string(*this); }

Message Message::as_reply(const std::string& prefix, const std::string& command,
                          const std::vector<std::string>& params) {
  Message reply = {prefix, command, params};
  return reply;
}

Message Message::as_numeric_reply(util::returnCode code,
                                  std::vector<std::string> params) {
  return as_reply(server.config_.name, util::pad_num(code), params);
}

Message Message::as_not_enough_params_reply(std::string command) {
  return as_numeric_reply(util::ERR_NEEDMOREPARAMS,
                          VA((command, "Not enough parameters")));
}

std::ostream& operator<<(std::ostream& os, const Message& msg) {
  typedef std::vector<std::string>::const_iterator const_it;

  if (not msg.prefix.empty()) {
    os << ":" << msg.prefix << " ";
  }

  os << msg.command;

  if (not msg.params.empty()) {
    for (const_it it = msg.params.begin(), end = util::prev(msg.params.end());
         it != end; ++it) {
      os << " " + *it;
    }
    os << " :" << *msg.params.rbegin();
  } else {
    os << " :";
  }
  return os;
}
