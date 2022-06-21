#include "StringBuffer.hpp"

StringBuffer::StringBuffer() : data_(), cursor_() {}

StringBuffer::StringBuffer(const std::string &str) : data_(str), cursor_(0) {}

void StringBuffer::reset() { cursor_ = 0; }

void StringBuffer::reset(const std::string &str) {
  data_ = str;
  reset();
}

bool StringBuffer::empty() const { return data_.size() == cursor_; }

const char *StringBuffer::data() const { return data_.data() + cursor_; }

std::size_t StringBuffer::size() const { return data_.size() - cursor_; }

void StringBuffer::advance(std::size_t len) { cursor_ += len; }
