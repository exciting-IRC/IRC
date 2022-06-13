#include <iostream>
#include <vector>
#include <string>

#include "network/socket/socket.hpp"
#include "event/event.hpp"

class Client : public EventHandler {
 Client(int sock) : sock_(sock) {}
 public:
  int getFd() { return sock_; }
  int handle(Event e) {
    if (e.kind == EventKind::kRead) {
      std::vector<char> cv;
      
      cv.reserve(e.data);
      
      util::recv(sock_, cv.data(), e.data);

      std::string s(cv.begin(), cv.end());

      std::cout << "Read event: " << s << std::endl;
    }
  }
 private:
  int sock_;
};

class Server : public EventHandler {
  
  int getFd() { return sock_; }

  private:
    int sock_;
};

int main() {

  EventPool pool(64);

  pool.addEvent(EventKind::kRead, );

  return 0;
}