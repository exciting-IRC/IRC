#include "command.hpp"

#include "command/returncode.hpp"
#include "command/returnformat.hpp"
#include "util/general/result.hpp"
#include "util/vargs/container_of.hpp"

#define REPLY(code, param) FMT(retfmt[code], param)

namespace util {

string Nick(User& user, const vector<LazyString>& args) {
  if (args.empty())
    return REPLY(ERR_NONICKNAMEGIVEN, ());

  string new_nickname = args[0];

  if (user.nickname == new_nickname)
    return "";

  if (new_nickname.length() > 9)
    return REPLY(ERR_ERRONEUSNICKNAME, (new_nickname));

  /* if nickname in use */
  REPLY(ERR_NICKNAMEINUSE, (new_nickname));

  user.nickname = new_nickname;
  return FMT("NICK {nick}", (new_nickname));
}

// string Pass(User& user, const vector<LazyString>& args) {
//   if (args.empty())
//     return retfmt[ERR_NEEDMOREPARAMS];
//   if (not user.password.empty())
//     return retfmt[ERR_ALREADYREGISTRED];
// }

const command_map commandsMap =
    container_of<command_map, command_pair>(p("NICK", &Nick));
// p("PASS", "pass"), p("NICK", "nick"), p("USER", "user"), p("JOIN", "join"),
// p("PART", "part"), p("PRIVMSG", "privmsg"), p("ADMIN", "admin"),
// p("KILL", "kill"), p("QUIT", "quit"));
}  // namespace util
