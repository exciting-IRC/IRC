#include <arpa/inet.h>
#include <sys/socket.h>

namespace util {

inline int socket(int domain, int type, int protocol) {
  return ::socket(domain, type, protocol);
}

inline int listen(int socket, int backlog) {
  return ::listen(socket, backlog);
}

inline int bind_in(int socket, const struct sockaddr_in *addr) {
  return ::bind(socket, reinterpret_cast<const sockaddr *>(addr),
                sizeof(*addr));
}

inline int connect_in(int socket, const struct sockaddr_in *addr) {
  return ::connect(socket, reinterpret_cast<const sockaddr *>(addr),
                   sizeof(*addr));
}

inline ssize_t recv(int socket, void *buffer, size_t length, int flags) {
  return ::recv(socket, buffer, length, flags);
}

inline ssize_t send(int socket, const void *buffer, size_t length,
                    int flags) {
  return ::send(socket, buffer, length, flags);
}

}  // namespace util