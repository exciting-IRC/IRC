#include "util/config/config.hpp"

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
