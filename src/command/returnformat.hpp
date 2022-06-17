#ifndef COMMAND_REPLIES_HPP
#define COMMAND_REPLIES_HPP

#include <map>
#include <string>

#include "command/returncode.hpp"
#include "util/vargs/container_of.hpp"

namespace util {
using std::pair;
using std::string;

typedef std::map<returnCode, string> return_map;
typedef pair<returnCode, string> return_pair;

extern const return_map retfmt;
}  // namespace util
#endif  // COMMAND_REPLIES_HPP
