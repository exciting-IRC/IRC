#ifndef COMMAND_COMMAND_HPP
#define COMMAND_COMMAND_HPP

#include <string>

#include "util/LazyString/LazyString.hpp"
#include "util/general/ConstMap.hpp"
namespace util {

struct User {
  string username;
  string nickname;
  string password;
};

typedef string (*command)(User& user, const vector<LazyString>& args);
typedef std::pair<string, command> command_pair;
typedef ConstMap<string, command> command_map;

extern const command_map commandsMap;
}  // namespace util
#endif  // COMMAND_COMMANDMAP_HPP
