#ifndef COMMAND_REPLIES_HPP
#define COMMAND_REPLIES_HPP

#include <string>

#include "command/returncode.hpp"
#include "util/general/ConstMap.hpp"
#include "util/vargs/container_of.hpp"

namespace util {
using std::pair;
using std::string;
using util::p;

typedef ConstMap<returnCode, string> return_map;
typedef pair<returnCode, string> return_pair;

return_map retfmt = container_of<return_map, return_pair>(
    p(RPL_WELCOME,
      "Welcome to the Internet Relay Network {nick}!{user}@{host}"),
    p(RPL_YOURHOST, "Your host is {servername}, running version {ver}"),
    p(RPL_CREATED, "This server was created {date}"),
    p(RPL_MYINFO,
      "{servername} {version} {available user modes} {available channel "
      "modes}"),
    p(ERR_NONICKNAMEGIVEN, ":No nickname given"),
    p(ERR_NOSUCHNICK, "{nick} :No such nick/channel"),
    p(ERR_ERRONEUSNICKNAME, "{nick} :Erroneous nickname"));
}  // namespace util

#endif  // COMMAND_REPLIES_HPP
