#ifndef CLIENT_CLIENT_TPP
#define CLIENT_CLIENT_TPP

#include "Client.hpp"

template <typename Cont>
void Client::sendList(const Message list_base, const Cont &list,
                      const Message list_end) {
  std::string base_string;
  if (!list_base.prefix.empty()) {
    base_string += ":" + list_base.prefix + " ";
  }
  base_string += list_base.command + " ";
  typedef std::vector<std::string>::const_iterator vector_const_it;
  for (vector_const_it it = list_base.params.begin(),
                       end = list_base.params.end();
       it != end; ++it) {
    base_string += *it + " ";
  }
  base_string += ":";
  std::string payload = base_string;
  typedef typename Cont::const_iterator t_const_it;
  for (t_const_it it = list.begin(), end = list.end(); it != end; ++it) {
    if (payload.size() + it->size() < 510) {
      payload += " " + *it;
    } else {
      send(payload);
      payload = base_string + *it;
    }
  }
  // 보내지 못하고 남은 데이터가 있음
  if (base_string.size() != payload.size()) {
    send(payload);
  }
  send(list_end);
}

#endif  // CLIENT_CLIENT_TPP
