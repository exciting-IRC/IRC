#include "EventPool.hpp"

#include <unistd.h>

#include "kqueue/kqueue.hpp"
#include "util/general/logging.hpp"
#include "util/general/time.hpp"

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

    case EventKind::kTimer:
      return EVFILT_TIMER;

    default:
      util::debug_info("aborting due to unknown EventKind:", kind, false);
      abort();
  }
}

struct kevent create_kevent(EventKind::e kind, IEventHandler *eh,
                            uint16_t flag) {
  struct kevent ev = {eh->getFd(), convert_to_evfilt(kind), flag, 0,
                      0,           static_cast<void *>(eh)};

  return ev;
}

struct kevent getRegisterTimer(int timeout, unsigned int flag,
                               IEventHandler *eh) {
  struct kevent ev = {
      TimerKind::kRegister, EVFILT_TIMER, flag | EV_ONESHOT | EV_UDATA_SPECIFIC,
      NOTE_SECONDS,         timeout,      eh};
  return ev;
}

struct kevent getPingTimer(int timeout, unsigned int flag, IEventHandler *eh) {
  struct kevent ev = {TimerKind::kPing, EVFILT_TIMER, flag | EV_UDATA_SPECIFIC,
                      NOTE_SECONDS,     timeout,      eh};
  return ev;
}

struct kevent getPongTimer(int timeout, unsigned int flag, IEventHandler *eh) {
  struct kevent ev = {
      TimerKind::kPong, EVFILT_TIMER, flag | EV_ONESHOT | EV_UDATA_SPECIFIC,
      NOTE_SECONDS,     timeout,      eh};
  return ev;
}

struct kevent getTimer(TimerKind::e kind, int timeout, unsigned int flag,
                       IEventHandler *eh) {
  switch (kind) {
    case TimerKind::kPing:
      return getPingTimer(timeout, flag, eh);
      break;
    case TimerKind::kPong:
      return getPongTimer(timeout, flag, eh);
      break;
    case TimerKind::kRegister:
      return getRegisterTimer(timeout, flag, eh);
      break;
  }
}

int EventPool::addTimer(TimerKind::e kind, IEventHandler *eh,
                        unsigned int timeout) {
  struct kevent ev = getTimer(kind, timeout, EV_ADD, eh);

  return util::kevent_ctl(fd_, &ev, 1, NULL);
}

int EventPool::removeTimer(TimerKind::e kind, IEventHandler *eh,
                           unsigned int timeout) {
  struct kevent ev = getTimer(kind, timeout, EV_DELETE, eh);

  return util::kevent_ctl(fd_, &ev, 1, NULL);
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

    case EVFILT_TIMER:
      ev.setTimerEvent(kev);
      break;
  }
  IEventHandler *handler = static_cast<IEventHandler *>(kev.udata);
  if (handler->handle(ev) == result_t::kError) {
    handler->handleError();
    return -1;
  }
  return 0;
}

int EventPool::dispatchEvent(const struct timespec &ts) {
  ssize_t events = util::kevent_wait(fd_, event_list_, max_event_, &ts);
  if (events == -1)
    return -1;
  for (ssize_t i = 0; i < events; ++i) {
    if (handleEvent(event_list_[i]))
      return i;
  }
  return events;
}
