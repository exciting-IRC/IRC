#ifndef FIXEDBUFFER_HPP
#define FIXEDBUFFER_HPP

#include <algorithm>
#include <cstddef>
#include <util/FixedBuffer/array.hpp>

namespace util {

template <typename T, std::size_t buffer_size>
class FixedBuffer : public array<T, buffer_size> {
 public:
  FixedBuffer();
  FixedBuffer(const FixedBuffer &other);
  FixedBuffer &operator=(const FixedBuffer &other);

 public:
  void seekg(std::size_t pos);

  void seekp(std::size_t pos);

  std::size_t tellg() const;

  std::size_t tellp() const;

  T peek() const;
  T pop();

 private:
  std::size_t i_cursor_;
  std::size_t o_cursor_;
};

}  // namespace util

#include "FixedBuffer.tpp"

#endif  // FIXEDBUFFER_HPP
