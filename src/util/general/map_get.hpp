#ifndef UTIL_GENERAL_MAP_GET_HPP
#define UTIL_GENERAL_MAP_GET_HPP

#include <map>

#include "util/strutil/conversion.hpp"

namespace util {
using std::map;

template <typename K, typename V>
const V& map_get(const map<K, V>& m, const K& k) {
  typename map<K, V>::const_iterator it = m.find(k);

  if (it == m.end())
    throw std::out_of_range("tried to get non-existent key" + to_string(k));

  return it->second;
}

template <typename K, typename V>
const V& map_get(const map<K, V>& m, const K& k, const V& default_value) {
  typename map<K, V>::const_iterator it = m.find(k);

  if (it == m.end())
    return default_value;

  return it->second;
}

}  // namespace util

#endif  // UTIL_GENERAL_MAP_GET_HPP
