#include "LazyString.hpp"

namespace util {

LazyString::LazyString() : start_(NULL), end_(NULL) {}

void LazyString::apply() {
  if (start_ && end_) {
    append(start_, end_);
    start_ = NULL;
    end_ = NULL;
  }
}

void LazyString::setStart(const char *start) { start_ = start; }

void LazyString::setEnd(const char *end) { end_ = end; }

}
