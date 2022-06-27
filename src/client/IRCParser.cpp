#include "IRCParser.hpp"

#include <iostream>

#include "util/irctype/irctype.hpp"

IRCParser::IRCParser() : state_(ParserState::kBegin), length_(0) {}

ParserResult::e IRCParser::parseBegin(const char *cursor) {
  if (*cursor == ':') {
    state_ = ParserState::kPrefixBegin;
  } else if (*cursor == '\r') {
    state_ = ParserState::kEmptyLF;
  } else if (util::isLetter(*cursor) || util::isDigit(*cursor)) {
    state_ = ParserState::kCommand;
    msg_.command.push_back(*cursor);
  } else {
    return ParserResult::kFailure;
  }
  return ParserResult::kSuccess;
}

ParserResult::e IRCParser::parsePrefixBegin(const char *cursor) {
  msg_.prefix.push_back(*cursor);
  state_ = ParserState::kPrefix;
  return ParserResult::kSuccess;
}

ParserResult::e IRCParser::parsePrefix(const char *cursor) {
  if (*cursor == ' ') {
    state_ = ParserState::kCommandBegin;
  } else {
    msg_.prefix.push_back(*cursor);
  }
  return ParserResult::kSuccess;
}

ParserResult::e IRCParser::parseCommandBegin(const char *cursor) {
  if (util::isLetter(*cursor) || util::isDigit(*cursor)) {
    state_ = ParserState::kCommand;
    msg_.command.push_back(*cursor);
    return ParserResult::kSuccess;
  }
  return ParserResult::kFailure;
}

ParserResult::e IRCParser::parseCommand(const char *cursor) {
  if (*cursor == ' ') {
    state_ = ParserState::kParamStart;
  } else if (*cursor == '\r') {
    state_ = ParserState::kLF;
  } else if (!util::isLetter(*cursor) && !util::isDigit(*cursor)) {
    return ParserResult::kFailure;
  } else {
    msg_.command.push_back(*cursor);
  }
  return ParserResult::kSuccess;
}

ParserResult::e IRCParser::parseParamStart(const char *cursor) {
  if (*cursor == ':') {
    state_ = ParserState::kTrailingStart;
  } else if (util::isRegular(*cursor)) {
    state_ = ParserState::kParam;
    msg_.params.push_back(std::string("") + *cursor);
  } else {
    return ParserResult::kFailure;
  }
  return ParserResult::kSuccess;
}

ParserResult::e IRCParser::parseParam(const char *cursor) {
  switch (*cursor) {
    case ' ':
      state_ = ParserState::kParamStart;
      break;

    case '\r':
      state_ = ParserState::kLF;
      break;

    case '\n':
    case '\0':
      return ParserResult::kFailure;
      break;

    default:
      msg_.params.back().push_back(*cursor);
      break;
  }
  return ParserResult::kSuccess;
}

ParserResult::e IRCParser::parseTrailingStart(const char *cursor) {
  msg_.params.push_back("");
  switch (*cursor) {
    case '\r':
      state_ = ParserState::kLF;
      break;

    case '\0':
    case '\n':
      return ParserResult::kFailure;
      break;
    default:
      msg_.params.back().push_back(*cursor);
      state_ = ParserState::kTrailing;
      break;
  }
  return ParserResult::kSuccess;
}

ParserResult::e IRCParser::parseTrailing(const char *cursor) {
  switch (*cursor) {
    case '\r':
      state_ = ParserState::kLF;
      break;
    case '\0':
    case '\n':
      return ParserResult::kFailure;
      break;
    default:
      msg_.params.back().push_back(*cursor);
      break;
  }
  return ParserResult::kSuccess;
}

ParserResult::e IRCParser::parseLF(const char *cursor) {
  if (*cursor == '\n') {
    return ParserResult::kSuccess;
  } else {
    return ParserResult::kFailure;
  }
}

ParserResult::e IRCParser::parse(util::Buffer &buffer) {
  while (!buffer.eof()) {
    const char *cursor = buffer.data() + buffer.tellg();
    buffer.pop();
    length_++;
    if (length_ > 512) {
      return ParserResult::kFailure;
    }
    switch (state_) {
      case ParserState::kBegin:
        if (parseBegin(cursor) == ParserResult::kFailure)
          return ParserResult::kFailure;
        break;
      case ParserState::kPrefixBegin:
        if (parsePrefixBegin(cursor) == ParserResult::kFailure) {
          return ParserResult::kFailure;
        }
        break;
      case ParserState::kPrefix:
        if (parsePrefix(cursor) == ParserResult::kFailure)
          return ParserResult::kFailure;
        break;
      case ParserState::kCommandBegin:
        if (parseCommandBegin(cursor) == ParserResult::kFailure)
          return ParserResult::kFailure;
        break;
      case ParserState::kCommand:
        if (parseCommand(cursor) == ParserResult::kFailure)
          return ParserResult::kFailure;
        break;
      case ParserState::kParamStart:
        if (parseParamStart(cursor) == ParserResult::kFailure)
          return ParserResult::kFailure;
        break;
      case ParserState::kParam:
        if (parseParam(cursor) == ParserResult::kFailure)
          return ParserResult::kFailure;
        break;
      case ParserState::kTrailingStart:
        if (parseTrailingStart(cursor) == ParserResult::kFailure)
          return ParserResult::kFailure;
        break;
      case ParserState::kTrailing:
        if (parseTrailing(cursor) == ParserResult::kFailure)
          return ParserResult::kFailure;
        break;
      case ParserState::kLF:
        return parseLF(cursor);
        break;
      case ParserState::kEmptyLF:
        if (parseLF(cursor) == ParserResult::kFailure)
          return ParserResult::kFailure;
        clear();
        break;
      default:
        abort();
        return ParserResult::kFailure;
    }
  }
  return ParserResult::kContinue;
}

void IRCParser::clearMessage() { msg_ = Message(); }

void IRCParser::clearState() {
  state_ = ParserState::kBegin;
  length_ = 0;
}

void IRCParser::clear() {
  clearMessage();
  clearState();
}

Message IRCParser::getMessage() {
  Message msg = msg_;
  clear();
  return msg;
}
