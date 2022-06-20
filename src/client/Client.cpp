#include "Client.hpp"

#include <unistd.h>

#include <iostream>

#include "command/returncode.hpp"

#include "ClientConn.hpp"
#include "server/Server.hpp"
#include "util/config/config.hpp"
#include "util/vargs/container_of.hpp"
#include "util/strutil/conversion.hpp"

using util::p;
const MPClientMap Client::map_ = container_of<MPClientMap, MPClientMap::value_type>(
  p("PING", &Client::ping)
);

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

Client::Client(ClientConn *conn) : conn_(conn), ident_(conn->moveIdent()) {}

int Client::getFd() const { return conn_->getFd(); }

// void Client::handleWriteEvent(Event &e) {
//   if (e.flags.test(EventFlag::kEOF)) {
//     delete conn_;
//     conn_ = NULL;
//   }
//   std::size_t write_size =
//       std::min(buffer_.size(), static_cast<std::size_t>(e.data));
//   write(1, buffer_.data(), write_size);
//   ssize_t send_length = util::send(getFd(), buffer_.data(), write_size);
//   if (send_length == -1) {
//     std::cerr << "failed to write" << std::endl;
//     // XXX LOG ...
//   } else {
//     buffer_.advance(send_length);
//   }
//   if (buffer_.empty())
//     e.pool.removeEvent(EventKind::kWrite, this);
// }

void Client::processMessage(const Message &m) {
  std::cout << "CMD: <" << m.command << ">";
  MPClientMap::const_iterator it = map_.find(m.command);
  if (it == map_.end()) {
    std::cout << ": Not found" << std::endl;
    return;
  }
  std::cout << ":Found" << std::endl;
  (this->*(it->second))(m);
}

void Client::handleReadEvent(Event &e) {
  if (conn_->handleReceive(e) == result_t::kError) {
    delete conn_;
    conn_ = NULL;
  } else {
    ParserResult::e result;
    while ((result = conn_->parse()) == ParserResult::kSuccess) {
      processMessage(conn_->getMessage());
    }
  }
}

int Client::handle(Event e) {
  switch (e.kind) {
    case EventKind::kRead:
      handleReadEvent(e);
      break;
    default:
      abort();
      break;
  }
  return 0;
}
 
void Client::ping(const Message &m) {
  if (m.params.size() != 1) {
    Message reply;
    reply.prefix = config.name;
    reply.command = util::pad_num(util::ERR_NOORIGIN);
    reply.params.push_back(":No origin specified");
    conn_->send(reply);
  }
  conn_->send(FMT(":{0} PONG {0} :{1}", (config.name, m.params[0])));
}
