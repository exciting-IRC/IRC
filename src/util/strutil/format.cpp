#include <util/strutil/strutil.hpp>
#include <vector>

namespace util {
using std::string;
using std::vector;

class FormatState {
 private:
  string fmt;
  vector<string> args;
  std::string result;
  int arg_index;
  size_t curs;

 public:
  // 생성자
  FormatState(const string& fmt, const vector<string>& args)
      : fmt(fmt), args(args), arg_index(0), curs(0) {
    for (; curs < fmt.size(); curs++) {
      if (on_escaped_bracket("{{")) {
        advance_and_append("{");
      } else if (on_char('{')) {
        advance_to_closing_bracket();
      } else if (on_escaped_bracket("}}")) {
        advance_and_append("}");
      } else if (on_char('}')) {
        throw std::runtime_error("unmatched closing brace at " +
                                 util::to_string(curs));
      } else {
        result += fmt[curs];
      }
    }
  }

  // 멤버 함수
  bool on_char(const char c) { return (fmt[curs] == c); }

  bool on_escaped_bracket(const string& match) {
    return fmt.substr(curs, 2) == match;
  }

  void advance_and_append(const string& to_add) {
    curs++;
    result += to_add;
  }

  void advance_to_closing_bracket() {
    result += args.at(arg_index++);
    for (size_t i = curs + 1; i < fmt.size(); i++) {
      if (fmt[i] == '{') {
        throw std::runtime_error("unmatched opening brace at " + to_string(i));
      } else if (fmt[i] == '}') {
        curs = i;
        return;
      }
    }
    throw std::runtime_error(
        "missing closing brace '}' at end of format string");
  }

  operator string() { return result; }
};

string format(const string& fmt, const vector<string>& args) {
  return FormatState(fmt, args);
}

}  // namespace util
