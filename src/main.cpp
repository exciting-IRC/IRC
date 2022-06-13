#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <util/FixedBuffer/FixedBuffer.hpp>
#include <util/strutil/strutil.hpp>
#include <utility>
#include <vector>

#include "container_of.hpp"
using namespace std;

string read_text(const string &filename) {
  std::ifstream ifs(filename);
  std::stringstream buffer;

  buffer << ifs.rdbuf();
  return buffer.str();
}

template <typename T>
T convert_to(string from) {
  T val;
  std::istringstream iss(from);

  iss >> val;
  return val;
}

pair<string, string> getkv(string line) {
  using namespace util;

  vector<string> kv = split(line, ":");
  if (kv.size() != 2)
    throw runtime_error("invalid line: " + line);
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
};

std::ostream &operator<<(std::ostream &os, const Config &config) {
  os << "name: " << config.name << endl;
  os << "info: " << config.info << endl;
  os << "motd: " << config.motd << endl;
  os << "oper_user: " << config.oper_user << endl;
  os << "oper_password: " << config.oper_password << endl;
  os << "max_clients: " << config.max_clients << endl;
  return os;
}

int main(int argc, const char *argv[]) {
  (void)argc;
  (void)argv;
  // if (argc != 3) {
  //   std::cout << "Usage: " << argv[0] << " <input_file> <output_file>"
  //             << std::endl;
  //   return 1;
  // }
  // util::print_vector(V(("hello", "world", "bye", "world")));
  std::ifstream configfile("config.yml");
  std::string line;
  map<string, string> m;
  while (std::getline(configfile, line)) {
    line = util::erase_from(line, "#");
    if (line.empty())
      continue;
    m.insert(getkv(line));
  }
  // for (map<string, string>::iterator it = m.begin(); it != m.end(); ++it) {
  //   std::cout << "key: " << it->first << " \nvalue: " << it->second
  //             << std::endl;
  // }
  Config config(m["name"], m["info"], m["motd"], m["oper_user"],
                m["oper_password"], m["max_clients"], m["ping"], m["timeout"]);
  cout << config << endl;
}
