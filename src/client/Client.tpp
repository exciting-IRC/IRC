#ifndef CLIENT_CLIENT_TPP
#define CLIENT_CLIENT_TPP

#include "Client.hpp"
#include "ClientConn.hpp"
#include "util/general/time.hpp"
#include "util/strutil/format.hpp"

template <typename T>
inline void Client::send(const T &msg) {
  conn_->send(msg);
}

#endif  // CLIENT_CLIENT_TPP
