#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <stddef.h>

#include <map>
#include <string>
#include <util/FixedBuffer/array.hpp>
#include <util/strutil/strutil.hpp>

using std::string;

namespace util {
using std::map;
using std::pair;

pair<string, string> getkv(string line) {
  using namespace util;

  vector<string> kv = split(line, ":");
  if (kv.size() != 2)
    throw std::invalid_argument("invalid line: " + line);
  return make_pair(trim(kv[0]), trim(kv[1]));
}

struct Config {
  const string name;
  const string info;
  const string motd;

  const string oper_user;
  const string oper_password;

  const uint32_t max_clients;
  const uint32_t ping;
  const uint32_t timeout;

  Config(string name, string info, string motd, string oper_user,
         string oper_password, string max_clients, string ping, string timeout)
      : name(name),
        info(read_text(info)),
        motd(read_text(motd)),
        oper_user(oper_user),
        oper_password(oper_password),
        max_clients(convert_to<uint32_t>(max_clients)),
        ping(convert_to<uint32_t>(ping)),
        timeout(convert_to<uint32_t>(timeout)) {}

  static Config from(const string &filename) {
    std::ifstream configfile(filename);
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

std::ostream &operator<<(std::ostream &os, const Config &config) {
  os << "name: " << config.name << "\n";
  os << "info: " << config.info << "\n";
  os << "motd: " << config.motd << "\n";
  os << "oper_user: " << config.oper_user << "\n";
  os << "oper_password: " << config.oper_password << "\n";
  os << "max_clients: " << config.max_clients << "\n";
  return os;
}

}  // namespace util
#endif  // CONFIG_HPP
