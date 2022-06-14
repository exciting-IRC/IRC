
#ifndef EVENT_EVENT_HPP
#define EVENT_EVENT_HPP

#include <sys/event.h>

#include "util/general/result.hpp"

struct EventKind {
  enum e { kRead = EVFILT_READ, kWrite = EVFILT_WRITE };
};

struct EventFlags {
  enum e { kEmpty = 0, kEOF = 1 };
};

// forward declearation
class EventPool;

struct Event {
  EventPool *ep;
  EventKind::e kind;
  intptr_t data;
  EventFlags::e flags;
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

  int addEvent(EventKind::e kind, EventHandler *eh);

  int dispatchEvent(time_t sec);

  int dispatchEvent(const struct timespec &ts);

 private:
  return_t::e initKqueue();
  return_t::e initEventList();

 private:
  struct kevent *event_list_;
  int fd_;
  bool ok_;
  int max_event_;
  bool is_fd_open_;
};

#endif  // EVENT_EVENT_HPP
