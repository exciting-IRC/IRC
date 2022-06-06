#ifndef FIXEDBUFFER_TPP
#define FIXEDBUFFER_TPP

#include "FixedBuffer.hpp"

namespace util {

template <typename T, std::size_t S>
FixedBuffer<T, S>::FixedBuffer() : i_cursor_(0), o_cursor_(0) {}

template <typename T, std::size_t S>
FixedBuffer<T, S>::FixedBuffer(const FixedBuffer &other)
    : i_cursor_(other.i_cursor_), o_cursor_(other.o_cursor_) {}

template <typename T, std::size_t S>
FixedBuffer<T, S> &FixedBuffer<T, S>::operator=(const FixedBuffer &other) {
  i_cursor_ = other.i_cursor_;
  o_cursor_ = other.o_cursor_;
  return *this;
}

template <typename T, std::size_t S>
void FixedBuffer<T, S>::seekg(std::size_t pos) {
  o_cursor_ = pos;
}

template <typename T, std::size_t S>
void FixedBuffer<T, S>::seekp(std::size_t pos) {
  i_cursor_ = pos;
}

template <typename T, std::size_t S>
std::size_t FixedBuffer<T, S>::tellg() const {
  return o_cursor_;
}

template <typename T, std::size_t S>
std::size_t FixedBuffer<T, S>::tellp() const {
  return i_cursor_;
}

template <typename T, std::size_t S>
T FixedBuffer<T, S>::peek() const {
  return this->elems[o_cursor_];
}

template <typename T, std::size_t S>
T FixedBuffer<T, S>::pop() {
  return this->elems[o_cursor_++];
}

}  // namespace util
#endif  // FIXEDBUFFER_TPP
