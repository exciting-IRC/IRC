#ifndef FIXEDBUFFER_HPP_
#define FIXEDBUFFER_HPP_

#include <cstddef>
#include <algorithm>

template <typename T, std::size_t buffer_size>
class FixedBuffer {
 public:
  typedef T *iterator;
  typedef const T *const_iterator;
  const static std::size_t capacity_ = buffer_size;

 public:
  FixedBuffer();
  FixedBuffer(const FixedBuffer &other);
  FixedBuffer &operator=(const FixedBuffer &other);
  ~FixedBuffer();

 public:
  iterator begin();
  iterator end();

  const_iterator begin() const;
  const_iterator end() const;

  std::size_t capacity() const;

  T &operator[](std::size_t index);
  const T &operator[](std::size_t index) const;

  void seekg(std::size_t pos);

  void seekp(std::size_t pos);

  std::size_t tellg() const;

  std::size_t tellp() const;

  T peek() const;
  T pop();

 private:
  T data_[buffer_size];
  std::size_t i_cursor_;
  std::size_t o_cursor_;
};

template <typename T, std::size_t S>
FixedBuffer<T, S>::FixedBuffer() : i_cursor_(0), o_cursor_(0) {}

template <typename T, std::size_t S>
FixedBuffer<T, S>::FixedBuffer(const FixedBuffer &other) : size_(other.size_) {
  std::copy(other.begin(), other.end(), begin());
}

template <typename T, std::size_t S>
FixedBuffer<T, S> &FixedBuffer<T, S>::operator=(const FixedBuffer &other) {
  std::copy(other.begin(), other.end(), begin());
  size_ = other.size_;
  return *this;
}

template <typename T, std::size_t S>
FixedBuffer<T, S>::~FixedBuffer() {}

template <typename T, std::size_t S>
FixedBuffer<T, S>::iterator FixedBuffer<T, S>::begin() {
  return data_;
}

template <typename T, std::size_t S>
FixedBuffer<T, S>::iterator FixedBuffer<T, S>::end() {
  return data_ + capacity_;
}

template <typename T, std::size_t S>
FixedBuffer<T, S>::const_iterator FixedBuffer<T, S>::begin() const {
  return data_;
}

template <typename T, std::size_t S>
FixedBuffer<T, S>::const_iterator FixedBuffer<T, S>::end() const {
  return data_ + capacity_;
}

template <typename T, std::size_t S>
std::size_t FixedBuffer<T, S>::capacity() const {
  return capacity_;
}

template <typename T, std::size_t S>
void FixedBuffer<T, S>::seekg(std::size_t pos) {
  i_cursor_ = pos;
}

template <typename T, std::size_t S>
void FixedBuffer<T, S>::seekp(std::size_t pos) {
  o_cursor_ = pos;
}

template <typename T, std::size_t S>
std::size_t FixedBuffer<T, S>::tellg() const {
  return i_cursor_;
}

template <typename T, std::size_t S>
std::size_t FixedBuffer<T, S>::tellp() const {
  return o_cursor_;
}

template <typename T, std::size_t S>
T &FixedBuffer<T, S>::operator[](std::size_t index) {
  return data_[index];
}

template <typename T, std::size_t S>
const T &FixedBuffer<T, S>::operator[](std::size_t index) const {
  return data_[index];
}

template <typename T, std::size_t S>
T FixedBuffer<T, S>::peek() const {
  return data_[o_cursor_];
}

template <typename T, std::size_t S>
T FixedBuffer<T, S>::pop() {
  return data_[o_cursor_++];
}

#endif // FIXEDBUFFER_HPP_