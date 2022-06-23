#include "client/Client.hpp"
#include "server/Server.hpp"

void Message::clear() {
  prefix.clear();
  command.clear();
  params.clear();
}

Message Message::as_numeric_reply(util::returnCode code,
                                  std::vector<std::string> params) {
  std::vector<util::LazyString> lazy_params(params.begin(), params.end());
  Message reply = {server.config_.name, util::pad_num(code), lazy_params};
  return reply;
}

Message Message::as_not_enough_params_reply(std::string command) {
  return as_numeric_reply(util::ERR_NEEDMOREPARAMS,
                          VA((command, "Not enough parameters")));
}
