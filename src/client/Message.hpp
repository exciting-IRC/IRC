#ifndef CLIENT_MESSAGE_HPP
#define CLIENT_MESSAGE_HPP

#include <string>

#include "command/returncode.hpp"
#include "util/LazyString/LazyString.hpp"
#include "util/algorithm/algorithm.hpp"
#include "util/config/config.hpp"
#include "util/strutil/conversion.hpp"

struct Message {
  util::LazyString prefix;
  util::LazyString command;
  std::vector<util::LazyString> params;

  void clear();
  operator std::string() const;
  static Message as_reply(const std::string& prefix, const std::string& command,
                          const std::vector<std::string>& params);
  static Message as_numeric_reply(util::returnCode code,
                                  std::vector<std::string> params);
  static Message as_not_enough_params_reply(std::string command);
};

std::ostream& operator<<(std::ostream& os, const Message& msg);

#endif  // CLIENT_MESSAGE_HPP
