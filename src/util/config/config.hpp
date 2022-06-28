#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <stddef.h>

#include <ctime>
#include <fstream>
#include <map>
#include <string>

#include "util/FixedBuffer/array.hpp"
#include "util/general/map_get.hpp"
#include "util/general/time.hpp"
#include "util/strutil/strutil.hpp"

namespace util {
using std::map;
using std::pair;
using std::string;

map<string, string> read_config(const string &filename);

struct Config {
  string name;
  string create_time;
  string info;
  string motd;

  string oper_user;
  string oper_password;

  string address;
  string password;
  uint16_t port;

  uint16_t max_clients;
  uint16_t ping;
  uint16_t timeout;

  Config() {}
  Config(string name, string info, string motd, string oper_user,
         string oper_password, string address, string max_clients, string ping,
         string timeout)
      : name(name),
        create_time(get_current_time()),
        info(read_text(info)),
        motd(read_text(motd)),
        oper_user(oper_user),
        oper_password(oper_password),
        address(address),
        max_clients(convert_to<uint16_t>(max_clients)),
        ping(convert_to<uint16_t>(ping)),
        timeout(convert_to<uint16_t>(timeout)) {}

  static Config from(const string &filename) {
    map<string, string> m = read_config(filename);
    return Config(map_get(m, "name"), m["info"], m["motd"], m["oper_user"],
                  m["oper_password"], map_get(m, "address"),
                  map_get(m, "max_clients"), map_get(m, "ping"),
                  map_get(m, "timeout"));
  }
};

std::ostream &operator<<(std::ostream &os, const Config &config);

}  // namespace util

extern util::Config config;

#endif  // CONFIG_HPP
