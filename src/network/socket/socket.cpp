#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>

namespace util {
int socket(int domain, int type, int protocol) {
  int sock = ::socket(domain, type, protocol);
  if (sock == -1)
    return -1;
  if (fcntl(sock, F_SETFD, O_NONBLOCK) == -1)
    return -1;
  return sock;
}

int listen(int socket, int backlog) {
  return ::listen(socket, backlog);
}

int bind_in(int socket, const struct sockaddr_in *addr) {
  return ::bind(socket, reinterpret_cast<const sockaddr *>(addr),
                sizeof(*addr));
}

int connect_in(int socket, const struct sockaddr_in *addr) {
  return ::connect(socket, reinterpret_cast<const sockaddr *>(addr),
                   sizeof(*addr));
}

ssize_t recv(int socket, void *buffer, size_t length, int flags) {
  return ::recv(socket, buffer, length, flags);
}

ssize_t send(int socket, const void *buffer, size_t length,
                    int flags) {
  return ::send(socket, buffer, length, flags);
}

int setsockopt(int socket, int level, int option_name, const void *option_value,
               socklen_t option_len) {
  return ::setsockopt(socket, level, option_name, option_value, option_len);
}

}  // namespace util