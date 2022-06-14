#include <unistd.h>

#include <cstring>
#include <iostream>
#include <new>
#include <stdexcept>

#include "event/event.hpp"
#include "event/kqueue/kqueue.hpp"
#include "util/general/time.hpp"

return_t::e EventPool::initKqueue() {
  fd_ = util::kqueue();
  if (fd_ == -1)
    return return_t::kError;
  is_fd_open_ = true;
  return return_t::kOK;
}

return_t::e EventPool::initEventList() {
  event_list_ = new (std::nothrow) struct kevent[max_event_];
  if (event_list_ == NULL)
    return return_t::kError;
  return return_t::kOK;
}

EventPool::EventPool(int max_event)
    : event_list_(NULL), ok_(false), max_event_(max_event), is_fd_open_(false) {
  if (initKqueue() == return_t::kError)
    return;
  if (initEventList() == return_t::kError)
    return;
  ok_ = true;
}

EventPool::~EventPool() {
  close();
  delete event_list_;
}

int EventPool::close() {
  if (!is_fd_open_)
    return 0;
  int ret = ::close(fd_);
  if (ret == 0)
    is_fd_open_ = false;
  return ret;
}

bool EventPool::ok() { return ok_; }

static struct kevent create_kevent(EventKind::e kind, EventHandler *eh) {
  struct kevent ev = {eh->getFd(), kind, EV_ADD, 0, 0, static_cast<void *>(eh)};
  return ev;
}

int EventPool::addEvent(EventKind::e kind, EventHandler *eh) {
  struct kevent ev = create_kevent(kind, eh);
  return util::kevent_ctl(fd_, &ev, 1, NULL);
}

int EventPool::dispatchEvent(time_t sec) {
  struct timespec ts = util::create_timespec_in_sec(sec);
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
    ev.flags = EventFlag::kEmpty;
    ev.ep = this;
    if (kev.filter == EVFILT_READ) {
      ev.kind = EventKind::kRead;
      ev.data = kev.data;
      if (kev.flags & EV_EOF)
        ev.flags = static_cast<EventFlag::e>(ev.flags | EventFlag::kEOF);
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
