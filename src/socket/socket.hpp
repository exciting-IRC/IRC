#ifndef SOCKET_SOCKET_HPP
#define SOCKET_SOCKET_HPP

#include <arpa/inet.h>
#include <sys/socket.h>

#include <bitset>

namespace util {

using ::accept;
using ::bind;
using ::connect;

struct SockProtocol_t {
  enum e { kDefult = 0 };
};

struct RecvFlag_t {
  enum e { kDefult = 0 };
};

struct SendFlag_t {
  enum e { kDefult = 0 };
};

/**
 * @brief
 *
 * @param domain 이 소켓이 사용되는 영역 (예: 유닉스 소켓, 인터넷 소켓)
 * @param type 생성될 소켓의 종류 (예: 연결 지향, 비-연결 지향)
 * @param protocol 사용할 실제 프로토콜 (예: TCP, UDP), 0일 시 type에 대한 기본
 * 프로토콜 사용
 * @return int fd 또는 오류시 -1
 */
int socket(int domain, int type, int protocol = SockProtocol_t::kDefult);

/**
 * @brief 소켓을 연결
 *
 * @param socket 연결 모드로 설정할 소켓
 * @param backlog 최대 연결 개수. 연결 가능한 개수보다 더 큰 값이나 0 이하의
 * 값이 들어오면 함수에서 적절한 값을 알아서 설정해준다.
 */
return_t::e listen(int socket, int backlog);
int bind_in(int socket, const struct sockaddr_in *addr);
int connect_in(int socket, const struct sockaddr_in *addr);
ssize_t recv(int socket, void *buffer, size_t length,
             int flags = RecvFlag_t::kDefult);
ssize_t send(int socket, const void *buffer, size_t length,
             int flags = SendFlag_t::kDefult);
int setsockopt(int socket, int level, int option_name, const void *option_value,
               socklen_t option_len);

}  // namespace util

#endif  // SOCKET_SOCKET_HPP
