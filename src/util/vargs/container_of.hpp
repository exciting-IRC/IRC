#ifndef SRC_UTIL_VARGS_CONTAINER_OF_HPP
#define SRC_UTIL_VARGS_CONTAINER_OF_HPP

// do not try to directly edit this file.
// generate using container_of.py instead

#define MAP_OF(K, V, param) container_of<std::map<K, V>, std::pair<K, V> > param
#define VEC_OF(T, param) container_of<std::vector<T> > param
#define V(param) VEC_OF(std::string, param)

namespace util {
template <typename K, typename V>
std::pair<K, V> p(K k, V v) {
  return std::make_pair(k, v);
}
}  // namespace util

template <typename C>
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

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10) {
  const T args[11] = {arg0, arg1, arg2, arg3, arg4, arg5,
                      arg6, arg7, arg8, arg9, arg10};
  return C(args, args + 11);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11) {
  const T args[12] = {arg0, arg1, arg2, arg3, arg4,  arg5,
                      arg6, arg7, arg8, arg9, arg10, arg11};
  return C(args, args + 12);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12) {
  const T args[13] = {arg0, arg1, arg2, arg3,  arg4,  arg5, arg6,
                      arg7, arg8, arg9, arg10, arg11, arg12};
  return C(args, args + 13);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13) {
  const T args[14] = {arg0, arg1, arg2, arg3,  arg4,  arg5,  arg6,
                      arg7, arg8, arg9, arg10, arg11, arg12, arg13};
  return C(args, args + 14);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14) {
  const T args[15] = {arg0, arg1, arg2,  arg3,  arg4,  arg5,  arg6, arg7,
                      arg8, arg9, arg10, arg11, arg12, arg13, arg14};
  return C(args, args + 15);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15) {
  const T args[16] = {arg0, arg1, arg2,  arg3,  arg4,  arg5,  arg6,  arg7,
                      arg8, arg9, arg10, arg11, arg12, arg13, arg14, arg15};
  return C(args, args + 16);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16) {
  const T args[17] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,
                      arg6,  arg7,  arg8,  arg9,  arg10, arg11,
                      arg12, arg13, arg14, arg15, arg16};
  return C(args, args + 17);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16, T arg17) {
  const T args[18] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,
                      arg6,  arg7,  arg8,  arg9,  arg10, arg11,
                      arg12, arg13, arg14, arg15, arg16, arg17};
  return C(args, args + 18);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16, T arg17, T arg18) {
  const T args[19] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,  arg6,
                      arg7,  arg8,  arg9,  arg10, arg11, arg12, arg13,
                      arg14, arg15, arg16, arg17, arg18};
  return C(args, args + 19);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16, T arg17, T arg18,
                      T arg19) {
  const T args[20] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,  arg6,
                      arg7,  arg8,  arg9,  arg10, arg11, arg12, arg13,
                      arg14, arg15, arg16, arg17, arg18, arg19};
  return C(args, args + 20);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16, T arg17, T arg18,
                      T arg19, T arg20) {
  const T args[21] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,  arg6,
                      arg7,  arg8,  arg9,  arg10, arg11, arg12, arg13,
                      arg14, arg15, arg16, arg17, arg18, arg19, arg20};
  return C(args, args + 21);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16, T arg17, T arg18,
                      T arg19, T arg20, T arg21) {
  const T args[22] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,  arg6,  arg7,
                      arg8,  arg9,  arg10, arg11, arg12, arg13, arg14, arg15,
                      arg16, arg17, arg18, arg19, arg20, arg21};
  return C(args, args + 22);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16, T arg17, T arg18,
                      T arg19, T arg20, T arg21, T arg22) {
  const T args[23] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,  arg6,  arg7,
                      arg8,  arg9,  arg10, arg11, arg12, arg13, arg14, arg15,
                      arg16, arg17, arg18, arg19, arg20, arg21, arg22};
  return C(args, args + 23);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16, T arg17, T arg18,
                      T arg19, T arg20, T arg21, T arg22, T arg23) {
  const T args[24] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,  arg6,  arg7,
                      arg8,  arg9,  arg10, arg11, arg12, arg13, arg14, arg15,
                      arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23};
  return C(args, args + 24);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16, T arg17, T arg18,
                      T arg19, T arg20, T arg21, T arg22, T arg23, T arg24) {
  const T args[25] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,  arg6,
                      arg7,  arg8,  arg9,  arg10, arg11, arg12, arg13,
                      arg14, arg15, arg16, arg17, arg18, arg19, arg20,
                      arg21, arg22, arg23, arg24};
  return C(args, args + 25);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16, T arg17, T arg18,
                      T arg19, T arg20, T arg21, T arg22, T arg23, T arg24,
                      T arg25) {
  const T args[26] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,  arg6,
                      arg7,  arg8,  arg9,  arg10, arg11, arg12, arg13,
                      arg14, arg15, arg16, arg17, arg18, arg19, arg20,
                      arg21, arg22, arg23, arg24, arg25};
  return C(args, args + 26);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16, T arg17, T arg18,
                      T arg19, T arg20, T arg21, T arg22, T arg23, T arg24,
                      T arg25, T arg26) {
  const T args[27] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,  arg6,
                      arg7,  arg8,  arg9,  arg10, arg11, arg12, arg13,
                      arg14, arg15, arg16, arg17, arg18, arg19, arg20,
                      arg21, arg22, arg23, arg24, arg25, arg26};
  return C(args, args + 27);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16, T arg17, T arg18,
                      T arg19, T arg20, T arg21, T arg22, T arg23, T arg24,
                      T arg25, T arg26, T arg27) {
  const T args[28] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,  arg6,
                      arg7,  arg8,  arg9,  arg10, arg11, arg12, arg13,
                      arg14, arg15, arg16, arg17, arg18, arg19, arg20,
                      arg21, arg22, arg23, arg24, arg25, arg26, arg27};
  return C(args, args + 28);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16, T arg17, T arg18,
                      T arg19, T arg20, T arg21, T arg22, T arg23, T arg24,
                      T arg25, T arg26, T arg27, T arg28) {
  const T args[29] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,  arg6,  arg7,
                      arg8,  arg9,  arg10, arg11, arg12, arg13, arg14, arg15,
                      arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23,
                      arg24, arg25, arg26, arg27, arg28};
  return C(args, args + 29);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16, T arg17, T arg18,
                      T arg19, T arg20, T arg21, T arg22, T arg23, T arg24,
                      T arg25, T arg26, T arg27, T arg28, T arg29) {
  const T args[30] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,  arg6,  arg7,
                      arg8,  arg9,  arg10, arg11, arg12, arg13, arg14, arg15,
                      arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23,
                      arg24, arg25, arg26, arg27, arg28, arg29};
  return C(args, args + 30);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16, T arg17, T arg18,
                      T arg19, T arg20, T arg21, T arg22, T arg23, T arg24,
                      T arg25, T arg26, T arg27, T arg28, T arg29, T arg30) {
  const T args[31] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,  arg6,  arg7,
                      arg8,  arg9,  arg10, arg11, arg12, arg13, arg14, arg15,
                      arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23,
                      arg24, arg25, arg26, arg27, arg28, arg29, arg30};
  return C(args, args + 31);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16, T arg17, T arg18,
                      T arg19, T arg20, T arg21, T arg22, T arg23, T arg24,
                      T arg25, T arg26, T arg27, T arg28, T arg29, T arg30,
                      T arg31) {
  const T args[32] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,  arg6,  arg7,
                      arg8,  arg9,  arg10, arg11, arg12, arg13, arg14, arg15,
                      arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23,
                      arg24, arg25, arg26, arg27, arg28, arg29, arg30, arg31};
  return C(args, args + 32);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16, T arg17, T arg18,
                      T arg19, T arg20, T arg21, T arg22, T arg23, T arg24,
                      T arg25, T arg26, T arg27, T arg28, T arg29, T arg30,
                      T arg31, T arg32) {
  const T args[33] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,  arg6,
                      arg7,  arg8,  arg9,  arg10, arg11, arg12, arg13,
                      arg14, arg15, arg16, arg17, arg18, arg19, arg20,
                      arg21, arg22, arg23, arg24, arg25, arg26, arg27,
                      arg28, arg29, arg30, arg31, arg32};
  return C(args, args + 33);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16, T arg17, T arg18,
                      T arg19, T arg20, T arg21, T arg22, T arg23, T arg24,
                      T arg25, T arg26, T arg27, T arg28, T arg29, T arg30,
                      T arg31, T arg32, T arg33) {
  const T args[34] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,  arg6,
                      arg7,  arg8,  arg9,  arg10, arg11, arg12, arg13,
                      arg14, arg15, arg16, arg17, arg18, arg19, arg20,
                      arg21, arg22, arg23, arg24, arg25, arg26, arg27,
                      arg28, arg29, arg30, arg31, arg32, arg33};
  return C(args, args + 34);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16, T arg17, T arg18,
                      T arg19, T arg20, T arg21, T arg22, T arg23, T arg24,
                      T arg25, T arg26, T arg27, T arg28, T arg29, T arg30,
                      T arg31, T arg32, T arg33, T arg34) {
  const T args[35] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,  arg6,
                      arg7,  arg8,  arg9,  arg10, arg11, arg12, arg13,
                      arg14, arg15, arg16, arg17, arg18, arg19, arg20,
                      arg21, arg22, arg23, arg24, arg25, arg26, arg27,
                      arg28, arg29, arg30, arg31, arg32, arg33, arg34};
  return C(args, args + 35);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16, T arg17, T arg18,
                      T arg19, T arg20, T arg21, T arg22, T arg23, T arg24,
                      T arg25, T arg26, T arg27, T arg28, T arg29, T arg30,
                      T arg31, T arg32, T arg33, T arg34, T arg35) {
  const T args[36] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,  arg6,  arg7,
                      arg8,  arg9,  arg10, arg11, arg12, arg13, arg14, arg15,
                      arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23,
                      arg24, arg25, arg26, arg27, arg28, arg29, arg30, arg31,
                      arg32, arg33, arg34, arg35};
  return C(args, args + 36);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16, T arg17, T arg18,
                      T arg19, T arg20, T arg21, T arg22, T arg23, T arg24,
                      T arg25, T arg26, T arg27, T arg28, T arg29, T arg30,
                      T arg31, T arg32, T arg33, T arg34, T arg35, T arg36) {
  const T args[37] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,  arg6,  arg7,
                      arg8,  arg9,  arg10, arg11, arg12, arg13, arg14, arg15,
                      arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23,
                      arg24, arg25, arg26, arg27, arg28, arg29, arg30, arg31,
                      arg32, arg33, arg34, arg35, arg36};
  return C(args, args + 37);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16, T arg17, T arg18,
                      T arg19, T arg20, T arg21, T arg22, T arg23, T arg24,
                      T arg25, T arg26, T arg27, T arg28, T arg29, T arg30,
                      T arg31, T arg32, T arg33, T arg34, T arg35, T arg36,
                      T arg37) {
  const T args[38] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,  arg6,  arg7,
                      arg8,  arg9,  arg10, arg11, arg12, arg13, arg14, arg15,
                      arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23,
                      arg24, arg25, arg26, arg27, arg28, arg29, arg30, arg31,
                      arg32, arg33, arg34, arg35, arg36, arg37};
  return C(args, args + 38);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16, T arg17, T arg18,
                      T arg19, T arg20, T arg21, T arg22, T arg23, T arg24,
                      T arg25, T arg26, T arg27, T arg28, T arg29, T arg30,
                      T arg31, T arg32, T arg33, T arg34, T arg35, T arg36,
                      T arg37, T arg38) {
  const T args[39] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,  arg6,  arg7,
                      arg8,  arg9,  arg10, arg11, arg12, arg13, arg14, arg15,
                      arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23,
                      arg24, arg25, arg26, arg27, arg28, arg29, arg30, arg31,
                      arg32, arg33, arg34, arg35, arg36, arg37, arg38};
  return C(args, args + 39);
}

template <typename C, typename T>
inline C container_of(T arg0, T arg1, T arg2, T arg3, T arg4, T arg5, T arg6,
                      T arg7, T arg8, T arg9, T arg10, T arg11, T arg12,
                      T arg13, T arg14, T arg15, T arg16, T arg17, T arg18,
                      T arg19, T arg20, T arg21, T arg22, T arg23, T arg24,
                      T arg25, T arg26, T arg27, T arg28, T arg29, T arg30,
                      T arg31, T arg32, T arg33, T arg34, T arg35, T arg36,
                      T arg37, T arg38, T arg39) {
  const T args[40] = {arg0,  arg1,  arg2,  arg3,  arg4,  arg5,  arg6,  arg7,
                      arg8,  arg9,  arg10, arg11, arg12, arg13, arg14, arg15,
                      arg16, arg17, arg18, arg19, arg20, arg21, arg22, arg23,
                      arg24, arg25, arg26, arg27, arg28, arg29, arg30, arg31,
                      arg32, arg33, arg34, arg35, arg36, arg37, arg38, arg39};
  return C(args, args + 40);
}

#endif  // SRC_UTIL_VARGS_CONTAINER_OF_HPP
