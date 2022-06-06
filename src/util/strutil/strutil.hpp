#ifndef STRUTIL_STRUTIL_HPP
#define STRUTIL_STRUTIL_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace util {

using std::string;
using std::vector;

enum { MAXSPLIT_ALL = -1 };

extern const string white_space;

string& erase_from(string& line, const string& match);
string& ltrim(string& s, const string& match = white_space);
string& rtrim(string& s, const string& match = white_space);
string& trim(string& s, const string& match = white_space);

vector<string> split(const string& str, const string& sep = "");

template <typename T>
string to_string(const T& t) {
  std::stringstream ss;

  ss << t;
  return ss.str();
}

template <typename Container>
string join(const Container& c, const string& sep = "") {
  std::stringstream ss;

  for (size_t i = 0; i < c.size(); ++i) {
    if (i != 0)
      ss << sep;
    ss << c[i];
  }
  return ss.str();
}

template <typename T>
void print_vector(vector<T> v, bool oneline = true) {
  const string sep = oneline ? ", " : "\n";
  for (typename vector<T>::iterator it = v.begin(); it != v.end(); ++it) {
    std::cout << "'" << *it << "'" << sep;
  }
  std::cout << "\n";
}

}  // namespace util

#endif  // STRUTIL_STRUTIL_HPP
