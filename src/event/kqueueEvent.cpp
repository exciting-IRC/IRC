
#ifndef EVENT_EVENT_HPP
#define EVENT_EVENT_HPP

#include <unistd.h>

#include <cstring>
#include <new>
#include <stdexcept>

#include "event/kqueue/kqueue.hpp"

enum EventKind { kRead = EVFILT_READ, kWrite = EVFILT_WRITE, kEOF = EV_EOF };

struct Event {
  EventPool *ep;
  EventKind kind;
  intptr_t data;
};

class EventHandler {
 public:
  virtual int handle(Event e) = 0;
  virtual int getFd() = 0;
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

EventPool::EventPool(int max_event)
    : event_list_(NULL), ok_(false), max_event_(max_event) {
  fd_ = util::kqueue();
  if (fd_ == -1)
    return;
  event_list_ = new (std::nothrow) struct kevent[max_event_];
  if (event_list_ == NULL)
    return;
  ok_ = true;
}

EventPool::~EventPool() {
  close();
  delete event_list_;
}

int EventPool::close() { return ::close(fd_); }

bool EventPool::ok() { return ok_; }

int EventPool::addEvent(EventKind kind, EventHandler *eh) {
  struct kevent ev;

  EV_SET(&ev, eh->getFd(), kind, EV_ADD, 0, 0, static_cast<void *>(eh));
  return util::kevent_ctl(fd_, &ev, 1, NULL);
}

int EventPool::dispatchEvent(time_t sec) {
  struct timespec ts;

  ts.tv_nsec = 0;
  ts.tv_sec = sec;
  return dispatchEvent(ts);
}

int EventPool::dispatchEvent(const struct timespec &ts) {
  ssize_t events = util::kevent_wait(fd_, event_list_, max_event_, &ts);
  if (events == -1)
    return -1;
  for (ssize_t i = 0; i < events; ++i) {
    struct kevent &kev = event_list_[i];
    EventHandler *eh = static_cast<EventHandler *>(kev.udata);

    struct Event ev;
    ev.ep = this;
    if (kev.flags & EV_EOF) {
      ev.kind = EventKind::kEOF;
      ev.data = kev.ident;
    } else if (kev.filter == EVFILT_READ) {
      ev.kind = EventKind::kRead;
      ev.data = kev.data;
    } else if (kev.filter == EVFILT_WRITE) {
      ev.kind = EventKind::kWrite;
      ev.data = kev.data;
    } else {
      // XXX add log (unknown event)
      throw std::invalid_argument("kqueue: unknown Event occred");
    }
    eh->handle(ev);
  }
  return events;
}

#endif  // EVENT_EVENT_HPP