#include "util/general/logging.hpp"

#include "util/FixedBuffer/array.hpp"
#include "util/color.hpp"
#include "util/general/time.hpp"
#include "util/strutil/format.hpp"

namespace util {

void debug(const string& msg, bool ok) {
  static const array<string, 2> result = {BHRED " ERR!", ""};

  COUT_FMT("{time}{result} {msg}" END,
           (get_current_time("[%H:%M:%S]"), result[ok], msg));
}

void debug_input(const string& command, bool ok) {
  debug(FMT(BHCYN "-> " HRED "\"{}\"", (command)), ok);
}

void debug_output(const string& payload, bool ok) {
  debug(FMT(BHGRN "<- " HBLU "\"{}\"", (payload)), ok);
}
}  // namespace util
