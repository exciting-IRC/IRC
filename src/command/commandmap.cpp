#include "commandmap.hpp"

#include "util/vargs/container_of.hpp"

namespace util {
const ConstMap<string, string> commandsMap =
    container_of<ConstMap<string, string>, std::pair<string, string> >(
        p("PASS", "pass"), p("NICK", "nick"), p("USER", "user"),
        p("JOIN", "join"), p("PART", "part"), p("PRIVMSG", "privmsg"),
        p("ADMIN", "admin"), p("KILL", "kill"), p("QUIT", "quit"));
}
