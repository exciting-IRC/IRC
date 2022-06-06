#include "strutil.hpp"

namespace util {
const string white_space = " \t\n\r\f\v";

// trim from end of string (right)
string& rtrim(string& s, const string& match) {
  s.erase(s.find_last_not_of(match) + 1);
  return s;
}

// trim from beginning of string (left)
string& ltrim(string& s, const string& match) {
  s.erase(0, s.find_first_not_of(match));
  return s;
}

// trim from both ends of string (right then left)
string& trim(string& s, const string& match) {
  return ltrim(rtrim(s, match), match);
}

string& erase_from(string& line, const string& match) {
  string::size_type comment_index = line.find(match);

  if (comment_index != string::npos)
    line.erase(comment_index);
  return line;
}
}  // namespace util
