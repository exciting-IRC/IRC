#ifndef COMMAND_COMMAND_HPP
#define COMMAND_COMMAND_HPP

#include <map>
#include <string>
#include <vector>

#include "util/LazyString/LazyString.hpp"

namespace util {
using std::string;
using std::vector;

struct User {
  string username;
  string nickname;
  string password;
};

typedef string (*command)(User& user, const vector<LazyString>& args);
typedef std::pair<string, command> command_pair;
typedef std::map<string, command> command_map;

extern const command_map commandsMap;
}  // namespace util
#endif  // COMMAND_COMMANDMAP_HPP
