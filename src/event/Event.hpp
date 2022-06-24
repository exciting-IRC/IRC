
#ifndef EVENT_EVENT_HPP
#define EVENT_EVENT_HPP

#include <sys/event.h>

#include <bitset>

#include "util/general/result.hpp"

struct TimerKind {
  enum e { kPing, kRegister, kPong };
};

struct EventKind {
  enum e { kNone = 0, kRead = (1 << 0), kWrite = (1 << 1), kTimer = (1 << 2) };
};

typedef std::bitset<8> EventFlagSet;

struct EventFlag {
  enum e { kEmpty = 0, kEOF = 1 };
};

class EventPool;

struct Event {
  Event(EventPool &pool_);
  void setReadEvent(const struct kevent &kev);
  void setWriteEvent(const struct kevent &kev);
  void setTimerEvent(const struct kevent &kev);

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

#endif  // EVENT_EVENT_HPP
