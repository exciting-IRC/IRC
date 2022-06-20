
#include "command.hpp"

#include <algorithm>
#include <cctype>

#include "command/returncode.hpp"
#include "command/returnformat.hpp"
#include "util/FixedBuffer/array.hpp"
#include "util/algorithm/algorithm.hpp"
#include "util/general/map_get.hpp"
#include "util/general/result.hpp"
#include "util/irctype/irctype.hpp"
#include "util/strutil/format.hpp"
#include "util/vargs/container_of.hpp"

#define REPLY(code, param) FMT(map_get(util::retfmt, code), param)
#define NUMERIC_REPLY(code, param) \
  FMT(to_string(code) + " " + map_get(util::retfmt, code), param)

namespace util {

bool nick_first_char_valid(const char ch) {
  return isLetter(ch) or isSpecial(ch);
}

bool nick_other_char_valid(const char ch) {
  return isLetter(ch) or isDigit(ch) or isSpecial(ch);
}

bool nick_is_valid(const string& nick) {
  if (nick.empty() or nick.length() > 9 or
      not nick_first_char_valid(nick.front()))  // XXX: front()는 c++11
    return false;
  if (nick.length() == 1)
    return true;
  else
    return util::all_of(nick.begin() + 1, nick.end(), nick_other_char_valid);
}
// TODO: #51에서 추가된 Handler로 옮기기
// string Nick(User& user, const vector<LazyString>& args) {
//   if (args.empty())
//     return NUMERIC_REPLY(ERR_NONICKNAMEGIVEN, ());

//   string new_nickname = args[0];

//   if (user.nickname == new_nickname)
//     return "";

//   if (not(nick_is_valid(new_nickname)))
//     return NUMERIC_REPLY(ERR_ERRONEUSNICKNAME, (new_nickname));

//   if (false /* if nickname in use */)
//     REPLY(ERR_NICKNAMEINUSE, (new_nickname));

//   user.nickname = new_nickname;
//   return FMT("NICK {nick}", (new_nickname));
// }

// string Pass(User& user, const vector<LazyString>& args) {
//   if (args.empty())
//     return retfmt[ERR_NEEDMOREPARAMS];
//   if (not user.password.empty())
//     return retfmt[ERR_ALREADYREGISTRED];
// }

// const command_map commandsMap =
//     container_of<command_map, command_pair>(p("NICK", &Nick));
// p("PASS", "pass"), p("NICK", "nick"), p("USER", "user"), p("JOIN", "join"),
// p("PART", "part"), p("PRIVMSG", "privmsg"), p("ADMIN", "admin"),
// p("KILL", "kill"), p("QUIT", "quit"));
}  // namespace util
