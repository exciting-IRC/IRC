#include "IRCParser.hpp"

#include <iostream>

#include "util/irctype/irctype.hpp"

IRCParser::IRCParser() : state_(ParserState::kBegin), length_(0) {}

ParserResult::e IRCParser::parseBegin(const char *cursor) {
  if (*cursor == ':') {
    state_ = ParserState::kPrefix;
    msg_.prefix.setStart(cursor);
  } else if (*cursor == '\r') {
    state_ = ParserState::kEmptyLF;
  } else if (util::isLetter(*cursor) || util::isDigit(*cursor)) {
    state_ = ParserState::kCommand;
    msg_.command.setStart(cursor);
  } else {
    return ParserResult::kFailure;
  }
  return ParserResult::kSuccess;
}

ParserResult::e IRCParser::parsePrefix(const char *cursor) {
  if (*cursor == ' ') {
    msg_.prefix.setEnd(cursor);
    msg_.prefix.apply();
    state_ = ParserState::kCommand;
  }
  return ParserResult::kSuccess;
}

ParserResult::e IRCParser::parseCommand(const char *cursor) {
  if (*cursor == ' ') {
    msg_.command.setEnd(cursor);
    msg_.command.apply();
    state_ = ParserState::kParamStart;
  } else if (*cursor == '\r') {
    msg_.command.setEnd(cursor);
    msg_.command.apply();
    state_ = ParserState::kLF;
  } else if (!util::isLetter(*cursor) && !util::isDigit(*cursor)) {
    return ParserResult::kFailure;
  }
  return ParserResult::kSuccess;
}

ParserResult::e IRCParser::parseParamStart(const char *cursor) {
  if (*cursor == ':') {
    state_ = ParserState::kTrailingStart;
  } else if (util::isRegular(*cursor)) {
    state_ = ParserState::kParam;
    msg_.params.push_back(util::LazyString());
    msg_.params.back().setStart(cursor);
  } else {
    return ParserResult::kFailure;
  }
  return ParserResult::kSuccess;
}

ParserResult::e IRCParser::parseParam(const char *cursor) {
  switch (*cursor) {
    case ' ':
      msg_.params.back().setEnd(cursor);
      msg_.params.back().apply();
      state_ = ParserState::kParamStart;
      break;

    case '\r':
      msg_.params.back().setEnd(cursor);
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

ParserResult::e IRCParser::parseTrailingStart(const char *cursor) {
  msg_.params.push_back(util::LazyString());
  switch (*cursor) {
    case '\r':
      state_ = ParserState::kLF;
      break;

    case '\0':
    case '\n':
      return ParserResult::kFailure;
      break;
    default:
      msg_.params.back().setStart(cursor);
      state_ = ParserState::kTrailing;
  }
  return ParserResult::kSuccess;
}

ParserResult::e IRCParser::parseTrailing(const char *cursor) {
  switch (*cursor) {
    case '\r':
      msg_.params.back().setEnd(cursor);
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
      case ParserState::kPrefix:
        if (parsePrefix(cursor) == ParserResult::kFailure)
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
        return ParserResult::kFailure;
    }
  }
  applyAll(buffer.data(), buffer.data() + buffer.tellp());
  return ParserResult::kContinue;
}

void IRCParser::clearMessage() { msg_.clear(); }

void IRCParser::clearState() {
  state_ = ParserState::kBegin;
  length_ = 0;
}

void IRCParser::clear() {
  clearMessage();
  clearState();
}

const Message &IRCParser::getMessage() { return msg_; }

void IRCParser::applyAll(const char *start, const char *end) {
  if (state_ == ParserState::kPrefix) {
    msg_.prefix.setEnd(end);
    msg_.prefix.apply();
    msg_.prefix.setStart(start);
  } else if (state_ == ParserState::kCommand) {
    msg_.command.setEnd(end);
    msg_.command.apply();
    msg_.command.setStart(start);
  } else if (state_ == ParserState::kParam || state_ == ParserState::kTrailing) {
    msg_.params.back().setEnd(end);
    msg_.params.back().apply();
    msg_.params.back().setStart(start);
  }
}
