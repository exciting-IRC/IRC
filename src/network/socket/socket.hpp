#ifndef SOCKET_SOCKET_HPP
#define SOCKET_SOCKET_HPP

#include <arpa/inet.h>
#include <sys/socket.h>

namespace util {

using ::accept;
using ::bind;
using ::connect;

inline int socket(int domain, int type, int protocol);
inline int listen(int socket, int backlog);
inline int bind_in(int socket, const struct sockaddr_in *addr);
inline int connect_in(int socket, const struct sockaddr_in *addr);
inline ssize_t recv(int socket, void *buffer, size_t length, int flags = 0);
inline ssize_t send(int socket, const void *buffer, size_t length,
                    int flags = 0);

}  // namespace util

#endif  // SOCKET_SOCKET_HPP
