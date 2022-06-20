#ifndef SOCKET_SOCKET_HPP
#define SOCKET_SOCKET_HPP

#include <arpa/inet.h>
#include <sys/socket.h>

#include <bitset>

#include "util/general/result.hpp"
#include "util/general/types.hpp"

typedef fd_t socket_t;

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
socket_t socket(int domain, int type, int protocol = SockProtocol_t::kDefult);

/**
 * @brief 소켓을 연결 모드로 지정
 *
 * @param socket 연결 모드로 설정할 소켓
 * @param backlog 최대 연결 개수. 연결 가능한 개수보다 더 큰 값이나 0 이하의
 * 값이 들어오면 함수에서 적절한 값을 알아서 설정해준다.
 */
return_t::e listen(socket_t socket, int backlog);

/**
 * @brief 소켓을 실제 (ip|domain):port에 바인딩해서 외부에서 바인딩한 주소로
 * socket에 접근할 수 있게 한다.
 *
 * @param addr 바인딩할 주소
 */
return_t::e bind_in(socket_t socket, const struct sockaddr_in *addr);

/**
 * @brief 소켓 socket을 사용하여 addr에 연결을 시도한다
 *
 * @param socket 연결을 담을 소켓
 * @param addr 접속할 주소
 * @return return_t::e
 */
return_t::e connect_in(socket_t socket, const struct sockaddr_in &addr);

/**
 * @brief 소켓에서 데이터 읽기를 시도한다
 *
 * @param buffer 데이터를 담을 버퍼
 * @param length 읽을 데이터 바이트 수
 * @param flags  추가적인 플래그 (MSG_OOB, MSG_PEEK, MSG_WAITALL)
 * @return ssize_t 읽은 바이트 수 (실패시 -1)
 */
ssize_t recv(socket_t socket, void *buffer, size_t length,
             int flags = RecvFlag_t::kDefult);

/**
 * @brief 소켓에 데이터 쓰기를 시도한다
 *
 * @param buffer 쓸 데이터를 담는 버퍼
 * @param length 쓸 바이트 수
 * @param flags  추가적인 플래그 (MSG_OOB, MSG_DONTROUTE)
 * @return ssize_t 쓴 바이트 수 (실패시 -1)
 */
ssize_t send(socket_t socket, const void *buffer, size_t length,
             int flags = SendFlag_t::kDefult);

/**
 * @brief 소켓의 특성을 변경한다 (fcntl(2) 함수와 비슷함)
 *
 * @param level <sys/socket.h>에 정의되어있는 적용할 소켓의 종류 (소켓)
 * @param option_name 적용할 특성의 종류
 * @param option_value 적용할 특성의 값 (option_name에 따라 타입이 달라짐)
 * @param option_len   option_value 의 크기(byte 단위)
 * @return return_t::e
 */
return_t::e setsockopt(socket_t socket, int level, int option_name,
                       const void *option_value, socklen_t option_len);

}  // namespace util

#endif  // SOCKET_SOCKET_HPP
