#ifndef EVENT_EVENTPOOL_HPP
#define EVENT_EVENTPOOL_HPP

#include "Event.hpp"
#include "util/general/result.hpp"

class IEventHandler;

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
  int addTimer(TimerKind::e kind, IEventHandler *eh, unsigned int timeout);

  int removeEvent(EventKind::e kind, IEventHandler *eh);
  int removeTimer(TimerKind::e kind, IEventHandler *eh, unsigned int timeout);

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

#endif  // EVENT_EVENTPOOL_HPP
