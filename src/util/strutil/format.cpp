#include <util/strutil/strutil.hpp>
#include <vector>

namespace util {
using std::string;
using std::vector;

format::format(const string& fmt, const vector<string>& args)
    : fmt(fmt), args(args), arg_index(0), curs(0) {
  for (; curs < fmt.size(); curs++) {
    if (on_escaped_bracket("{{")) {
      advance_and_append("{");
    } else if (on_char('{')) {
      advance_to_closing_bracket();
    } else if (on_escaped_bracket("}}")) {
      advance_and_append("}");
    } else if (on_char('}')) {
      throw std::invalid_argument("unmatched closing brace at " +
                                  to_string(curs));
    } else {
      result += fmt[curs];
    }
  }
}

bool format::on_char(const char c) const { return (fmt[curs] == c); }

bool format::on_escaped_bracket(const string& match) {
  return fmt.substr(curs, 2) == match;
}

void format::advance_and_append(const string& to_add) {
  curs++;
  result += to_add;
}

void format::advance_to_closing_bracket() {
  result += args.at(arg_index++);
  for (size_t i = curs + 1; i < fmt.size(); i++) {
    if (fmt[i] == '{') {
      throw std::invalid_argument("unmatched opening brace at " + to_string(i));
    } else if (fmt[i] == '}') {
      curs = i;
      return;
    }
  }
  throw std::invalid_argument(
      "missing closing brace '}' at end of format string");
}

string format::str() const { return result; }
format::operator string() const { return result; }

std::ostream& operator<<(std::ostream& os, const format& f) {
  return os << static_cast<string>(f);
}

}  // namespace util
