#include "Client.hpp"

#include "ClientConn.hpp"
#include "util/config/config.hpp"
#include <iostream>
#include <unistd.h>

/*STRING_BUFFER===============================*/

StringBuffer::StringBuffer() : data_(), cursor_() {}

StringBuffer::StringBuffer(const std::string &str) : data_(str), cursor_(0) {}

void StringBuffer::reset() { cursor_ = 0; }

void StringBuffer::reset(const std::string &str) {
  data_ = str;
  reset();
}

bool StringBuffer::empty() const { return data_.size() == cursor_; }

const char *StringBuffer::data() const { return data_.data() + cursor_; }

std::size_t StringBuffer::size() const { return data_.size() - cursor_; }

void StringBuffer::advance(std::size_t len) { cursor_ += len; }

/*CLIENT===============================*/

Client::Client(ClientConn *conn)
    : conn_(conn), ident_(conn->moveIdent()) {}

int Client::getFd() const { return conn_->getFd(); }

void Client::handleWriteEvent(Event &e) {
  if (e.flags.test(EventFlag::kEOF)) {
    delete conn_;
    conn_ = NULL;
  }
  std::size_t write_size =
      std::min(buffer_.size(), static_cast<std::size_t>(e.data));
  write(1, buffer_.data(), write_size);
  ssize_t send_length = util::send(getFd(), buffer_.data(), write_size);
  if (send_length == -1) {
    std::cerr << "failed to write" << std::endl;
    // XXX LOG ...
  } else {
    buffer_.advance(send_length);
  }
  if (buffer_.empty())
    e.pool.removeEvent(EventKind::kWrite, this);
}

void Client::handleReadEvent(Event &e) {
  exit(99);
  std::cout << "RDEV: " << e.data << std::endl;
}

int Client::handle(Event e) {
  switch (e.kind) {
    case EventKind::kRead:
      handleReadEvent(e);
      break;

    case EventKind::kWrite:
      handleWriteEvent(e);
      break;
  }
  return 0;
}

void Client::pong() {
  std::string reply = FMT(":{0} PONG {0} {0}", (config.name));
  std::cout << reply << "\n";
}

void Client::setMessageBuffer(const std::string &msg) { buffer_.reset(msg); }
