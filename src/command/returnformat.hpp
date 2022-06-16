#ifndef COMMAND_REPLIES_HPP
#define COMMAND_REPLIES_HPP

#include <string>

#include "command/returncode.hpp"
#include "util/general/ConstMap.hpp"
#include "util/vargs/container_of.hpp"

namespace util {
using std::pair;
using std::string;

typedef ConstMap<returnCode, string> return_map;
typedef pair<returnCode, string> return_pair;

extern const return_map retfmt;
}  // namespace util
#endif  // COMMAND_REPLIES_HPP
