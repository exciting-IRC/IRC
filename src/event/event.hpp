
#ifndef EVENT_EVENT_HPP
#define EVENT_EVENT_HPP

#include <sys/event.h>

enum EventKind { kRead = EVFILT_READ, kWrite = EVFILT_WRITE};

enum EventFlags {
  kEOF = 1
};

// forward declearation
class EventPool;

struct Event {
  EventPool *ep;
  EventKind kind;
  intptr_t data;
  unsigned int flags;
};

class EventHandler {
 public:
  virtual int handle(Event e) = 0;
  virtual int getFd() const = 0;
};

class EventPool {
 public:
  EventPool(int max_event);
  ~EventPool();

 private:
  EventPool(const EventPool &);             // = delete;
  EventPool &operator=(const EventPool &);  // = delete;

 public:
  int close();

  bool ok();

  int addEvent(EventKind kind, EventHandler *eh);

  int dispatchEvent(time_t sec);

  int dispatchEvent(const struct timespec &ts);

 private:
  struct kevent *event_list_;
  int fd_;
  bool ok_;
  int max_event_;
};

#endif  // EVENT_EVENT_HPP
