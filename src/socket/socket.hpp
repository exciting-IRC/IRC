#ifndef SOCKET_SOCKET_HPP
#define SOCKET_SOCKET_HPP

#include <arpa/inet.h>
#include <sys/socket.h>

namespace util {

using ::accept;
using ::bind;
using ::connect;

int socket(int domain, int type, int protocol = 0);
int listen(int socket, int backlog);
int bind_in(int socket, const struct sockaddr_in *addr);
int connect_in(int socket, const struct sockaddr_in *addr);
ssize_t recv(int socket, void *buffer, size_t length, int flags = 0);
ssize_t send(int socket, const void *buffer, size_t length, int flags = 0);
int setsockopt(int socket, int level, int option_name, const void *option_value,
               socklen_t option_len);

}  // namespace util

#endif  // SOCKET_SOCKET_HPP
