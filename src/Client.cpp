#include "Client.hpp"

#include <err.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "Server.hpp"
#include "event/event.hpp"
#include "socket/socket.hpp"
#include "util/FixedBuffer/FixedBuffer.hpp"

Client::Client(int sock, Server &server, ClientList::iterator this_position)
    : sock_(sock), server_(server), this_position_(this_position) {}

Client::~Client() {
  if (sock_ != -1)
    close();
}

int Client::getFd() const { return sock_; }

namespace tt {
bool isLetter(char c) { return std::isalpha(c); }

bool isDigit(char c) { return std::isdigit(c); }

bool isSeprator(char c) { return c == ':' || c == ' '; }

bool isSpecial(char c) { return c == '\r' || c == '\n' || c == '\0'; }

bool isNospcrlfcl(char c) { return !isSeprator(c) && !isSpecial(c); }

struct LazyString {
  char *start;
  char *end;
  std::string str;
  void append() {
    if (start && end) {
      str.append(start, end);
      start = NULL;
      end = NULL;
    }
  }
};

struct ParserResult {
  enum e { kContinue, kSuccess, kFailure };
};

struct ParserState {
  enum e { kBegin, kPrefix, kCommand, kTrailing, kParam, kParamStart, kLf };
};

class IRCParser {
 public:
  IRCParser() : state_(ParserState::kBegin), length_(0) {}
  ParserResult::e parse(Buffer &buffer) {
    while (!buffer.eof()) {
      ++length_;
      if (length_ > 512) {
        return ParserResult::kFailure;
      }
      char ch = buffer.peek();
      switch (state_) {
        case ParserState::kBegin:
          if (ch == ':') {
            state_ = ParserState::kPrefix;
            prefix_.start = buffer.data() + buffer.tellg();
          } else if (isLetter(ch) || isDigit(ch)) {
            state_ = ParserState::kCommand;
            command_.start = buffer.data() + buffer.tellg();
          } else {
            return ParserResult::kFailure;
          }
          break;
        case ParserState::kPrefix:
          if (ch == ' ') {
            prefix_.end = buffer.data() + buffer.tellg();
            prefix_.append();
            state_ = ParserState::kCommand;
          }
          break;
        case ParserState::kCommand:
          if (ch == ' ' || ch == '\r') {
            command_.end = buffer.data() + buffer.tellg();
            command_.append();
            if (ch == ' ') {
              state_ = ParserState::kParamStart;
            } else {
              state_ = ParserState::kLf;
            }
          } else if (!isLetter(ch) && !isDigit(ch)) {
            return ParserResult::kFailure;
          }
          break;
        case ParserState::kParam:
          if (ch == ' ')
            break;
        case ParserState::kParamStart:
          if (isNosp)
            params_.push_back(LazyString());
          params_.back().start = buffer.data() + buffer.tellg();

          break;
        case ParserState::kLf:
          if (ch == '\n') {
            return ParserResult::kSuccess;
          } else {
            return ParserResult::kFailure;
          }
      }
      buffer.pop();
    }

    if (buffer.eof()) {
    }
  }
  LazyString prefix_;
  LazyString command_;
  std::vector<LazyString> params_;
  ParserState::e state_;
  std::size_t length_;
};

int parse() {}
}  // namespace tt

void Client::handleReadEvent(Event &e) {
  if (e.data != 0) {
    std::cout << "-recv: " << e.data << std::endl;
    ssize_t len = recv(e.data);
    if (len > 0) {
      // parse while (!buffer.eof()) { std::cout << buffer.pop(); }
      std::cout << std::endl;
    }
  }
  if ((e.flags.test(EventFlag::kEOF)) || e.data == 0) {
    std::cout << "Connection closed." << std::endl;
    server_.removeClient(this_position_);
  }
}

int Client::handle(Event e) {
  switch (e.kind) {
    case EventKind::kRead:
      handleReadEvent(e);
      break;

    default:
      // XXX throw invalid_argument, logging
      break;
  }
  return 0;
}

int Client::close() { return ::close(sock_); }

ssize_t Client::recv(size_t length) {
  ssize_t ret = util::recv(sock_, buffer.begin(), length);
  if (ret == -1)
    return ret;
  buffer.seekg(0);
  buffer.seekp(ret);
  return ret;
}
