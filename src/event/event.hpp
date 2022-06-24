
#ifndef EVENT_EVENT_HPP
#define EVENT_EVENT_HPP

#include <sys/event.h>

#include <bitset>

#include "util/general/result.hpp"

struct EventKind {
  enum e { kNone = 0, kRead = (1 << 0), kWrite = (1 << 1) };
};

typedef std::bitset<8> EventFlagSet;

struct EventFlag {
  enum e { kEmpty = 0, kEOF = 1 };
};

// forward declearation
class EventPool;

struct Event {
  Event(EventPool &pool_);
  void setReadEvent(const struct kevent &kev);
  void setWriteEvent(const struct kevent &kev);

  EventPool &pool;
  EventKind::e kind;
  intptr_t data;
  EventFlagSet flags;
};

class IEventHandler {
 public:
  virtual result_t::e handle(Event e) = 0;
  virtual void handleError() = 0;
  virtual int getFd() const = 0;
};

class EventPool {
 public:
  EventPool();
  ~EventPool();

 private:
  EventPool(const EventPool &);             // = delete;
  EventPool &operator=(const EventPool &);  // = delete;

 public:
  result_t::e init(int max_event);

  int close();

  int addEvent(EventKind::e kind, IEventHandler *eh);

  int removeEvent(EventKind::e kind, IEventHandler *eh);

  int dispatchEvent(time_t sec);

  int dispatchEvent(const struct timespec &ts);

 private:
  int handleEvent(struct kevent &kev);

 private:
  result_t::e initKqueue();
  result_t::e initEventList();

 private:
  struct kevent *event_list_;
  int fd_;
  int max_event_;
  bool is_fd_open_;
};

#endif  // EVENT_EVENT_HPP
