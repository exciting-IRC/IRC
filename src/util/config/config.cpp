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
  // TODO: use format
  os << "name: " << config.name << "\n";
  os << "info: " << config.info << "\n";
  os << "motd: " << config.motd << "\n";
  os << "oper_user: " << config.oper_user << "\n";
  os << "oper_password: " << config.oper_password << "\n";
  os << "max_clients: " << config.max_clients << "\n";
  return os;
}
