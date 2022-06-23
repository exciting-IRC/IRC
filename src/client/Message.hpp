#ifndef CLIENT_MESSAGE_HPP
#define CLIENT_MESSAGE_HPP

#include "command/returncode.hpp"
#include "util/LazyString/LazyString.hpp"
#include "util/config/config.hpp"
#include "util/strutil/conversion.hpp"

struct Message {
  util::LazyString prefix;
  util::LazyString command;
  std::vector<util::LazyString> params;

  void clear();
  static Message as_numeric_reply(util::returnCode code,
                                  std::vector<std::string> params);
  static Message as_not_enough_params_reply(std::string command);
};

#endif  // CLIENT_MESSAGE_HPP
