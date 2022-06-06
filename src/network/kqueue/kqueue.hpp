#ifndef KQUEUE_KQUEUE_HPP
#define KQUEUE_KQUEUE_HPP

#include <sys/event.h>

#ifdef EV_SET
#undef EV_SET
// 조금 더 나은 인텔리센스 지원을 위해서 새로 정의함
#define EV_SET(kev, ident, filter, flags, fflags, data, udata) \
  do {                                                         \
    struct kevent *__kevp__ = (kev);                           \
    __kevp__->ident = (ident);                                 \
    __kevp__->filter = (filter);                               \
    __kevp__->flags = (flags);                                 \
    __kevp__->fflags = (fflags);                               \
    __kevp__->data = (data);                                   \
    __kevp__->udata = (udata);                                 \
  } while (0)
#endif  // EV_SET

namespace util {

inline ssize_t kevent_ctl(int kq, const struct kevent *changelist, int nchangs,
                          const struct timespec *timeout);

inline ssize_t kevent_wait(int kq, struct kevent *eventlist, int nevents,
                           const struct timespec *timeout);

using ::kevent;
using ::kqueue;

}  // namespace util

#endif  // KQUEUE_KQUEUE_HPP