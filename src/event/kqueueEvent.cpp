#include <unistd.h>

#include <cstring>
#include <iostream>
#include <new>
#include <stdexcept>

#include "event/event.hpp"
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

result_t::e EventPool::initKqueue() {
  fd_ = util::kqueue();
  if (fd_ == -1)
    return result_t::kError;
  is_fd_open_ = true;
  return result_t::kOK;
}

result_t::e EventPool::initEventList() {
  event_list_ = new (std::nothrow) struct kevent[max_event_];
  if (event_list_ == NULL)
    return result_t::kError;
  return result_t::kOK;
}

result_t::e EventPool::init(int max_event) {
  max_event_ = max_event;
  if (initKqueue() == result_t::kError)
    return result_t::kError;
  if (initEventList() == result_t::kError)
    return result_t::kError;
  return result_t::kOK;
}

EventPool::EventPool() : event_list_(NULL), max_event_(0), is_fd_open_(false) {}

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

int16_t convert_to_evfilt(EventKind::e kind) {
  switch (kind) {
    case EventKind::kRead:
      return EVFILT_READ;

    case EventKind::kWrite:
      return EVFILT_WRITE;

    default:
      abort();
  }
}

struct kevent create_kevent(EventKind::e kind, IEventHandler *eh,
                            uint16_t flag) {
  struct kevent ev = {eh->getFd(), convert_to_evfilt(kind), flag, 0,
                      0,           static_cast<void *>(eh)};

  return ev;
}

int EventPool::addEvent(EventKind::e kind, IEventHandler *eh) {
  struct kevent ev = create_kevent(kind, eh, EV_ADD);

  return util::kevent_ctl(fd_, &ev, 1, NULL);
}

int EventPool::removeEvent(EventKind::e kind, IEventHandler *eh) {
  struct kevent ev = create_kevent(kind, eh, EV_DELETE);

  return util::kevent_ctl(fd_, &ev, 1, NULL);
}

int EventPool::dispatchEvent(time_t sec) {
  struct timespec ts = util::create_timespec_in_sec(sec);

  return dispatchEvent(ts);
}

int EventPool::handleEvent(struct kevent &kev) {
  if (kev.flags & EV_ERROR) {
    util::debug("kevent flags got EV_ERROR", false);
    return -1;
  }
  Event ev(*this);
  switch (kev.filter) {
    case EVFILT_READ:
      ev.setReadEvent(kev);
      break;

    case EVFILT_WRITE:
      ev.setWriteEvent(kev);
      break;
    default:
      util::debug_info("Unknown kevent filter:", kev.filter, false);
      break;
  }
  IEventHandler *handler = static_cast<IEventHandler *>(kev.udata);
  if (handler->handle(ev) == result_t::kError)
    handler->handleError();
  return 0;
}

int EventPool::dispatchEvent(const struct timespec &ts) {
  ssize_t events = util::kevent_wait(fd_, event_list_, max_event_, &ts);
  if (events == -1)
    return -1;
  for (ssize_t i = 0; i < events; ++i) {
    handleEvent(event_list_[i]);
  }
  return events;
}
