#include "IRCParser.hpp"

#include "util/irctype/irctype.hpp"

IRCParser::IRCParser() : state_(ParserState::kBegin), length_(0) {}

ParserResult::e IRCParser::parseBegin(util::Buffer &buffer, char ch) {
  if (ch == ':') {
    state_ = ParserState::kPrefix;
    msg_.prefix.setStart(buffer.data() + buffer.tellg());
  } else if (util::isLetter(ch) || util::isDigit(ch)) {
    state_ = ParserState::kCommand;
    msg_.command.setStart(buffer.data() + buffer.tellg());
  } else {
    return ParserResult::kFailure;
  }
  return ParserResult::kSuccess;
}

ParserResult::e IRCParser::parsePrefix(util::Buffer &buffer, char ch) {
  if (ch == ' ') {
    msg_.prefix.setEnd(buffer.data() + buffer.tellg());
    msg_.prefix.apply();
    state_ = ParserState::kCommand;
  }
  return ParserResult::kSuccess;
}

ParserResult::e IRCParser::parseCommand(util::Buffer &buffer, char ch) {
  if (ch == ' ') {
    msg_.command.setEnd(buffer.data() + buffer.tellg());
    msg_.command.apply();
    state_ = ParserState::kParamStart;
  } else if (ch == '\r') {
    msg_.command.setEnd(buffer.data() + buffer.tellg());
    msg_.command.apply();
    state_ = ParserState::kLF;
  } else if (!util::isLetter(ch) && !util::isDigit(ch)) {
    return ParserResult::kFailure;
  }
  return ParserResult::kSuccess;
}

ParserResult::e IRCParser::parseParamStart(util::Buffer &buffer, char ch) {
  if (ch == ':') {
    state_ = ParserState::kTrailingStart;
  } else if (util::isRegular(ch)) {
    state_ = ParserState::kParam;
    msg_.params.push_back(util::LazyString());
    msg_.params.back().setStart(buffer.data() + buffer.tellg());
  } else {
    return ParserResult::kFailure;
  }
  return ParserResult::kSuccess;
}

ParserResult::e IRCParser::parseParam(util::Buffer &buffer, char ch) {
  switch (ch) {
    case ' ':
      msg_.params.back().setEnd(buffer.data() + buffer.tellg());
      msg_.params.back().apply();
      state_ = ParserState::kParamStart;
      break;

    case '\r':
      msg_.params.back().setEnd(buffer.data() + buffer.tellg());
      msg_.params.back().apply();
      state_ = ParserState::kLF;
      break;

    case '\n':
    case '\0':
      return ParserResult::kFailure;
      break;

    default:
      break;
  }
  return ParserResult::kSuccess;
}

ParserResult::e IRCParser::parseTrailingStart(util::Buffer &buffer, char ch) {
  msg_.params.push_back(util::LazyString());
  switch (ch) {
    case '\r':
      state_ = ParserState::kLF;
      break;

    case '\0':
    case '\n':
      return ParserResult::kFailure;
      break;
    default:
      msg_.params.back().setStart(buffer.data() + buffer.tellg());
      state_ = ParserState::kTrailing;
  }
  return ParserResult::kSuccess;
}

ParserResult::e IRCParser::parseTrailing(util::Buffer &buffer, char ch) {
  switch (ch) {
    case '\r':
      msg_.params.back().setEnd(buffer.data() + buffer.tellg());
      msg_.params.back().apply();
      state_ = ParserState::kLF;
      break;
    case '\0':
    case '\n':
      return ParserResult::kFailure;
      break;
    default:
      break;
  }
  return ParserResult::kSuccess;
}

ParserResult::e IRCParser::parseLF(char ch) {
  if (ch == '\n') {
    return ParserResult::kSuccess;
  } else {
    return ParserResult::kFailure;
  }
}

ParserResult::e IRCParser::parse(util::Buffer &buffer) {
  while (!buffer.eof()) {
    ++length_;
    if (length_ > 512) {
      return ParserResult::kFailure;
    }
    char ch = buffer.peek();
    switch (state_) {
      case ParserState::kBegin:
        if (parseBegin(buffer, ch) == ParserResult::kFailure)
          return ParserResult::kFailure;
        break;
      case ParserState::kPrefix:
        if (parsePrefix(buffer, ch) == ParserResult::kFailure)
          return ParserResult::kFailure;
        break;
      case ParserState::kCommand:
        if (parseCommand(buffer, ch) == ParserResult::kFailure)
          return ParserResult::kFailure;
        break;
      case ParserState::kParamStart:
        if (parseParamStart(buffer, ch) == ParserResult::kFailure)
          return ParserResult::kFailure;
        break;
      case ParserState::kParam:
        if (parseParam(buffer, ch) == ParserResult::kFailure)
          return ParserResult::kFailure;
        break;
      case ParserState::kTrailingStart:
        if (parseTrailingStart(buffer, ch) == ParserResult::kFailure)
          return ParserResult::kFailure;
        break;
      case ParserState::kTrailing:
        if (parseTrailing(buffer, ch) == ParserResult::kFailure)
          return ParserResult::kFailure;
        break;
      case ParserState::kLF:
        return parseLF(ch);
        break;
      default:
        return ParserResult::kFailure;
    }
    buffer.pop();
  }
  msg_.prefix.apply();
  msg_.command.apply();
  if (!msg_.params.empty())
    msg_.params.back().apply();
  return ParserResult::kContinue;
}

void IRCParser::clearMessage() {
  msg_.clear();
}

void IRCParser::clearState() {
  state_ = ParserState::kBegin;
  length_ = 0;
}

void IRCParser::clear() {
  clearMessage();
  clearState();
}
