#ifndef IRCPARSER_HPP
#define IRCPARSER_HPP

#include <vector>

#include "util/FixedBuffer/FixedBuffer.hpp"
#include "util/LazyString/LazyString.hpp"

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
  ParserResult::e parseBegin(util::Buffer &buffer, char ch);
  ParserResult::e parsePrefix(util::Buffer &buffer, char ch);
  ParserResult::e parseCommand(util::Buffer &buffer, char ch);
  ParserResult::e parseParamStart(util::Buffer &buffer, char ch);
  ParserResult::e parseParam(util::Buffer &buffer, char ch);
  ParserResult::e parseTrailingStart(util::Buffer &buffer, char ch);
  ParserResult::e parseTrailing(util::Buffer &buffer, char ch);
  ParserResult::e parseLF(char ch);

 private:
  Message msg_;
  ParserState::e state_;
  std::size_t length_;
};

#endif  // IRCPARSER_HPP
