#include <util/strutil/strutil.hpp>
#include <vector>

namespace util {
using std::string;
using std::vector;

string format(const string& fmt, const vector<string>& args) {
  int arg_counter = 0;
  std::string result;

  for (size_t i = 0; i < fmt.size(); i++) {
    if (fmt[i] == '{') {
      if (fmt[i + 1] == '{') {
        result += '{';
        i++;
      } else {
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
    } else if (fmt[i] == '}') {
      if (fmt[i + 1] == '}') {
        result += '}';
        i++;
      } else {
        throw std::runtime_error("unmatched closing brace at " +
                                 util::to_string(i));
      }
    } else {
      result += fmt[i];
    }
  }
  return result;
}

}  // namespace util
