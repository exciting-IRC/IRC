#include "strutil.hpp"

namespace util {

const string white_space = " \t\n\r\f\v";

string& rtrim(string& s, const string& match) {
  s.erase(s.find_last_not_of(match) + 1);
  return s;
}

string& ltrim(string& s, const string& match) {
  s.erase(0, s.find_first_not_of(match));
  return s;
}

string& trim(string& s, const string& match) {
  return ltrim(rtrim(s, match), match);
}

string& erase_from(string& line, const string& match) {
  string::size_type index = line.find(match);

  if (index != string::npos)
    line.erase(index);
  return line;
}

string& erase_before(string& line, const string& match) {
  string::size_type index = line.rfind(match);

  if (index != string::npos)
    line.erase(0, index + match.size());
  return line;
}
}  // namespace util
