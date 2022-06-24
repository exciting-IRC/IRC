#ifndef CLIENT_CLIENT_TPP
#define CLIENT_CLIENT_TPP

#include "Client.hpp"

template <typename Cont>
void Client::sendList(const std::string &list_base, const Cont &list,
                      const std::string &list_end) {
  if (list.empty()) {
    send(list_base);
  } else {
    typedef typename Cont::const_iterator cont_const_it;
    cont_const_it it = list.begin();
    std::string payload = list_base + *it;
    ++it;
    for (cont_const_it end = list.end(); it != end; ++it) {
      if (payload.size() + it->size() < 509) {  // 512 - len(' \r\n')
        payload += " " + *it;
      } else {
        send(payload);
        payload = list_base + *it;
      }
    }
    // 보내지 못하고 남은 데이터가 있음
    if (list_base.size() != payload.size()) {
      send(payload);
    }
  }
  send(list_end);
}

#endif  // CLIENT_CLIENT_TPP
