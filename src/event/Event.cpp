#include "event/Event.hpp"

#include "event/kqueue/kqueue.hpp"
#include "util/general/logging.hpp"
#include "util/general/time.hpp"

Event::Event(EventPool &pool_) : pool(pool_), flags() {}

void Event::setReadEvent(const struct kevent &kev) {
  this->kind = EventKind::kRead;
  this->data = kev.data;
  if (kev.flags & EV_EOF)
    this->flags.set(EventFlag::kEOF);
}

void Event::setWriteEvent(const struct kevent &kev) {
  this->kind = EventKind::kWrite;
  this->data = kev.data;
  if (kev.flags & EV_EOF)
    this->flags.set(EventFlag::kEOF);
}

void Event::setTimerEvent(const struct kevent &kev) {
  this->kind = EventKind::kTimer;
  this->data = kev.ident;
}
