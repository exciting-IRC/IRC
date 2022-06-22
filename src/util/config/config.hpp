#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <stddef.h>

#include <ctime>
#include <fstream>
#include <map>
#include <string>

#include "util/FixedBuffer/array.hpp"
#include "util/strutil/strutil.hpp"
using std::string;

namespace util {
using std::map;
using std::pair;

pair<string, string> getkv(string line);

#define EIRC_VERSION "0.0.1"

struct Config {
  const string name;
  const string info;
  const string motd;

  const string oper_user;
  const string oper_password;

  const uint32_t max_clients;
  const uint32_t ping;
  const uint32_t timeout;
  char create_time[32];

  Config(string name, string info, string motd, string oper_user,
         string oper_password, string max_clients, string ping, string timeout)
      : name(name),
        info(read_text(info)),
        motd(read_text(motd)),
        oper_user(oper_user),
        oper_password(oper_password),
        max_clients(convert_to<uint32_t>(max_clients)),
        ping(convert_to<uint32_t>(ping)),
        timeout(convert_to<uint32_t>(timeout)) {
    time_t now;
    time(&now);
    strftime(create_time, sizeof(create_time), "%FT%TZ", gmtime(&now));
  }

  static Config from(const string &filename) {
    std::ifstream configfile(filename.c_str());
    if (configfile.fail())
      throw std::invalid_argument("config file not found: " + filename);

    std::string line;
    map<string, string> m;
    while (std::getline(configfile, line)) {
      line = util::trim(util::erase_from(line, "#"));
      if (line.empty())
        continue;
      m.insert(getkv(line));
    }
    return Config(m.at("name"), m["info"], m["motd"], m["oper_user"],
                  m["oper_password"], m.at("max_clients"), m.at("ping"),
                  m.at("timeout"));
  }
};

std::ostream &operator<<(std::ostream &os, const Config &config);

}  // namespace util

extern util::Config config;

#endif  // CONFIG_HPP
