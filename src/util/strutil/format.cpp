#include <util/strutil/strutil.hpp>
#include <vector>

namespace util {
using std::string;
using std::vector;

struct FormatState {
  string fmt;
  vector<string> args;
  std::string result;
  int arg_counter;

  // 생성자
  FormatState(const string& fmt, const vector<string>& args)
      : fmt(fmt), args(args), arg_counter(0) {}

  // 멤버 함수
  bool on_escaped_bracket(size_t i, const string& match) {
    return fmt.substr(i, 2) == match;
  }

  void advance_and_append(size_t& i, const string& to_add) {
    i++;
    result += to_add;
  }

  void advance_to_closing_bracket(size_t& i) {
    result += args.at(arg_counter++);
    size_t j = i + 1;
    for (; j < fmt.size(); j++) {
      if (fmt[j] == '{') {
        throw std::runtime_error("unmatched opening brace at " +
                                 util::to_string(j));
      } else if (fmt[j] == '}') {
        i = j;
        break;
      }
    }
    if (j == fmt.size()) {
      throw std::runtime_error(
          "missing closing brace '}' at end of format string");
    }
  }
};

string format(const string& fmt, const vector<string>& args) {
  FormatState state(fmt, args);

  for (size_t i = 0; i < fmt.size(); i++) {
    if (state.on_escaped_bracket(i, "{{")) {
      state.advance_and_append(i, "{");
    } else if (fmt[i] == '{') {
      state.advance_to_closing_bracket(i);
    } else if (state.on_escaped_bracket(i, "}}")) {
      state.advance_and_append(i, "}");
    } else if (fmt[i] == '}') {
      throw std::runtime_error("unmatched closing brace at " +
                               util::to_string(i));
    } else {
      state.result += fmt[i];
    }
  }
  return state.result;
}

}  // namespace util
