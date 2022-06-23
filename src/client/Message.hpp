#ifndef CLIENT_MESSAGE_HPP
#define CLIENT_MESSAGE_HPP

#include "command/returncode.hpp"
#include "util/LazyString/LazyString.hpp"
#include "util/config/config.hpp"
#include "util/strutil/conversion.hpp"

struct Message {
  void clear() {
    prefix.clear();
    command.clear();
    params.clear();
  }
  util::LazyString prefix;
  util::LazyString command;
  std::vector<util::LazyString> params;

  static Message as_numeric_reply(util::returnCode code,
                                  std::vector<std::string> params) {
    std::vector<util::LazyString> lazy_params(params.begin(), params.end());
    Message reply = {config.name, util::pad_num(code), lazy_params};
    return reply;
  }
  static Message as_not_enough_params_reply(std::string command) {
    return as_numeric_reply(util::ERR_NEEDMOREPARAMS,
                            VA((command, "Not enough parameters")));
  }
};

#endif  // CLIENT_MESSAGE_HPP
