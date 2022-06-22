#ifndef IRCPARSER_HPP
#define IRCPARSER_HPP

#include <vector>

#include "command/returncode.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"
#include "util/LazyString/LazyString.hpp"
#include "util/config/config.hpp"
#include "util/strutil/conversion.hpp"

struct ParserResult {
  enum e { kContinue, kSuccess, kFailure };
};

struct ParserState {
  enum e {
    kBegin,
    kPrefix,
    kCommand,
    kParamStart,
    kParam,
    kTrailingStart,
    kTrailing,
    kLF,
    kEmptyLF
  };
};

struct Message {
  void clear() {
    prefix.clear();
    command.clear();
    params.clear();
  }
  util::LazyString prefix;
  util::LazyString command;
  std::vector<util::LazyString> params;
  /**
   * @brief
   *
   */
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

class IRCParser {
 public:
  IRCParser();
  // ~IRCParser() = default;

 private:
  IRCParser(const IRCParser &);             // = delete;
  IRCParser &operator=(const IRCParser &);  // = delete;

 public:
  ParserResult::e parse(util::Buffer &buffer);
  void clearMessage();
  void clearState();
  void clear();
  const Message &getMessage();

 private:
  void applyAll(const char *start, const char *end);
  ParserResult::e parseBegin(const char *cursor);
  ParserResult::e parsePrefix(const char *cursor);
  ParserResult::e parseCommand(const char *cursor);
  ParserResult::e parseParamStart(const char *cursor);
  ParserResult::e parseParam(const char *cursor);
  ParserResult::e parseTrailingStart(const char *cursor);
  ParserResult::e parseTrailing(const char *cursor);
  ParserResult::e parseLF(const char *cursor);

 private:
  Message msg_;
  ParserState::e state_;
  std::size_t length_;
};

#endif  // IRCPARSER_HPP
