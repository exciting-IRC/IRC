#ifndef UTIL_STRINGBUFFER_HPP
#define UTIL_STRINGBUFFER_HPP

#include <string>

class StringBuffer {
 public:
  StringBuffer();
  StringBuffer(const std::string &str);
  // StringBuffer(const StringBuffer &) = default;
  // StringBuffer &operator=(const StringBuffer &other) = default;
  // ~StringBuffer() = default;

 public:
  void reset();

  void reset(const std::string &str);

  bool empty() const;

  const char *data() const;

  std::size_t size() const;

  void advance(std::size_t len);

 private:
  std::string data_;
  std::size_t cursor_;
};

#endif  // UTIL_STRINGBUFFER_HPP
