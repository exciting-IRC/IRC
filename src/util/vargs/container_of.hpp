#ifndef SRC_UTIL_VARGS_CONTAINER_OF_HPP
#define SRC_UTIL_VARGS_CONTAINER_OF_HPP

// do not try to directly edit this file.
// generate using container_of.py instead

#define VEC_OF(T, param) container_of<std::vector<T> > param
#define V(param) VEC_OF(std::string, param)

namespace util {
template <typename K, typename V>
pair<K, V> p(K k, V v) {
  return std::make_pair(k, v);
}
}  // namespace util

template <typename C, typename T>
inline C container_of() {
  return C();
}

template <typename C, typename T>
inline C container_of(T arg0) {
  const T args[1] = {arg0};
  return C(args, args + 1);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1) {
  const T args[2] = {arg0, arg1};
  return C(args, args + 2);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2) {
  const T args[3] = {arg0, arg1, arg2};
  return C(args, args + 3);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3) {
  const T args[4] = {arg0, arg1, arg2, arg3};
  return C(args, args + 4);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4) {
  const T args[5] = {arg0, arg1, arg2, arg3, arg4};
  return C(args, args + 5);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5) {
  const T args[6] = {arg0, arg1, arg2, arg3, arg4, arg5};
  return C(args, args + 6);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6) {
  const T args[7] = {arg0, arg1, arg2, arg3, arg4, arg5, arg6};
  return C(args, args + 7);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7) {
  const T args[8] = {arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7};
  return C(args, args + 8);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8) {
  const T args[9] = {arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8};
  return C(args, args + 9);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9) {
  const T args[10] = {arg0, arg1, arg2, arg3, arg4,
                      arg5, arg6, arg7, arg8, arg9};
  return C(args, args + 10);
}

#endif  // SRC_UTIL_VARGS_CONTAINER_OF_HPP
