#include "util/config/config.hpp"

namespace util {

using std::map;
using std::pair;

static pair<string, string> getkv(string line) {
  using namespace util;

  vector<string> kv = split(line, ":");
  if (kv.size() != 2)
    throw std::invalid_argument("invalid line: " + line);
  return make_pair(trim(kv[0]), trim(kv[1]));
}

map<string, string> read_config(const string &filename) {
  std::stringstream ss;
  ss << read_text(filename);

  size_t i = 0;
  string line;
  map<string, string> m;
  while (std::getline(ss, line)) {
    i++;
    line = util::trim(util::erase_from(line, "#"));
    if (line.empty())
      continue;
    try {
      m.insert(getkv(line));
    } catch (const std::invalid_argument &e) {
      throw std::invalid_argument(FMT("on line {}: {}", (i, e.what())));
    }
  }
  return m;
}

}  // namespace util

std::ostream &util::operator<<(std::ostream &os, const Config &config) {
  return os << FMT(
             "name: {}\n"
             "create_time: {}\n"
             "oper_user: {}\n"
             "oper_password: {}\n"
             "info:\n{}\n"
             "motd:\n{}\n",
             (config.name, config.create_time, config.oper_user,
              config.oper_password, config.info, config.motd));
}
