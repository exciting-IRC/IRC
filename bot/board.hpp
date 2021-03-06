#ifndef BOARD_HPP
#define BOARD_HPP

#include <cstddef>

#include "util/FixedBuffer/array.hpp"
#include "vec2.hpp"

template <typename T, size_t width, size_t height>
class board {
 public:
  typedef T value_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef value_type* iterator;
  typedef const value_type* const_iterator;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  static const size_type size = width * height;

 private:
  util::array<value_type, size> data_;

 public:
  board() { data_.fill(value_type()); }
  board(const value_type& val) { data_.fill(val); }

  value_type at(size_type y, size_type x) const { return data_[y * width + x]; }
  reference at(size_type y, size_type x) { return data_[y * width + x]; }
  value_type at(pos p) const { return data_[p.y * width + p.x]; }
  reference at(pos p) { return data_[p.y * width + p.x]; }
  value_type at(size_type i) const { return data_[i]; }
  reference at(size_type i) { return data_[i]; }
  value_type random() const { return data_[util::randRange(size)]; }
  reference random() { return data_[util::randRange(size)]; }

  bool in_bounds(pos p) const { return p.y < height and p.x < width; }
  bool in_bounds(size_type y, size_type x) const {
    return y < height and x < width;
  }

  /// 이터레이터
  iterator begin() { return data_.begin(); }
  const_iterator begin() const { return data_.begin(); }
  iterator end() { return data_.end(); }
  const_iterator end() const { return data_.end(); }
  reference front() { return data_.front(); }
  const_reference front() const { return data_.front(); }
  reference back() { return data_.back(); }
  const_reference back() const { return data_.back(); }
  reverse_iterator rbegin() { return data_.rbegin(); }
  const_reverse_iterator rbegin() const { return data_.rbegin(); }
  reverse_iterator rend() { return data_.rend(); }
  const_reverse_iterator rend() const { return data_.rend(); }
  pointer data() { return data_.data(); }

  pos to_pos(iterator it) const {
    size_type offset = it - data_.begin();
    return pos(offset / width, offset % width);
  }
};

#endif  // BOARD_HPP
