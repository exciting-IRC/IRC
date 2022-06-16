#include "returnformat.hpp"

namespace util {
using util::p;

const return_map retfmt = container_of<return_map, return_pair>(
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
