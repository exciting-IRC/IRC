#include "socket.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>

#include "util/general/result.hpp"

namespace util {
socket_t socket(int domain, int type, int protocol) {
  int sock = ::socket(domain, type, protocol);
  if (sock == -1)
    return -1;
  if (fcntl(sock, F_SETFD, O_NONBLOCK) == -1)
    return -1;
  return sock;
}

result_t::e listen(socket_t socket, int backlog) {
  return static_cast<result_t::e>(::listen(socket, backlog));
}

result_t::e bind_in(socket_t socket, const struct sockaddr_in *addr) {
  return static_cast<result_t::e>(
      ::bind(socket, reinterpret_cast<const sockaddr *>(addr), sizeof(*addr)));
}

result_t::e connect_in(socket_t socket, const struct sockaddr_in *addr) {
  return static_cast<result_t::e>(::connect(
      socket, reinterpret_cast<const sockaddr *>(addr), sizeof(*addr)));
}

ssize_t recv(socket_t socket, void *buffer, size_t length, int flags) {
  return ::recv(socket, buffer, length, flags);
}

ssize_t send(socket_t socket, const void *buffer, size_t length, int flags) {
  return ::send(socket, buffer, length, flags);
}

result_t::e setsockopt(socket_t socket, int level, int option_name,
                       const void *option_value, socklen_t option_len) {
  return static_cast<result_t::e>(
      ::setsockopt(socket, level, option_name, option_value, option_len));
}

}  // namespace util
