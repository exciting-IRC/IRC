#ifndef KQUEUE_KQUEUE_HPP
#define KQUEUE_KQUEUE_HPP

#include <sys/event.h>

namespace util {

ssize_t kevent_ctl(int kq, const struct kevent *changelist, int nchangs,
                   const struct timespec *timeout);

ssize_t kevent_wait(int kq, struct kevent *eventlist, int nevents,
                    const struct timespec *timeout);

using ::kevent;
using ::kqueue;

}  // namespace util

#endif  // KQUEUE_KQUEUE_HPP
