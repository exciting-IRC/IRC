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

  FormatState(const string& fmt, const vector<string>& args)
      : fmt(fmt), args(args), arg_counter(0) {}
};

string format(const string& fmt, const vector<string>& args) {
  FormatState state(fmt, args);

  for (size_t i = 0; i < fmt.size(); i++) {
    if (fmt.substr(i, i + 1) == "{{") {
      state.result += '{';
      i++;
    } else if (fmt[i] == '{') {
      state.result += args.at(state.arg_counter++);
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
    } else if (fmt.substr(i, i + 1) == "}}") {
      state.result += '}';
      i++;
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
