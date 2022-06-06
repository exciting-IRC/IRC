#include <sys/event.h>

#include <cstddef>

namespace util {

inline ssize_t kevent_ctl(int kq, const struct kevent *changelist, int nchangs,
                          const struct timespec *timeout) {
  return kevent(kq, changelist, nchangs, NULL, 0, timeout);
}

inline ssize_t kevent_wait(int kq, struct kevent *eventlist, int nevents,
                           const struct timespec *timeout) {
  return kevent(kq, NULL, 0, eventlist, nevents, timeout);
}

}  // namespace util