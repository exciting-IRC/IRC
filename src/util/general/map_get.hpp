#ifndef UTIL_GENERAL_MAP_GET_HPP
#define UTIL_GENERAL_MAP_GET_HPP

#include <map>

#include "util/strutil/format.hpp"

namespace util {
using std::map;

template <typename C>
const typename C::mapped_type& map_get(const C& m,
                                       const typename C::key_type& k) {
  typename C::const_iterator it = m.find(k);

  if (it == m.end())
    throw std::out_of_range(FMT("tried to get non-existent key {}", (k)));

  return it->second;
}

template <typename C>
const typename C::mapped_type& map_get(
    const C& m, const typename C::key_type& k,
    const typename C::mapped_type& default_value) {
  typename C::const_iterator it = m.find(k);

  if (it == m.end())
    return default_value;

  return it->second;
}

}  // namespace util

#endif  // UTIL_GENERAL_MAP_GET_HPP
