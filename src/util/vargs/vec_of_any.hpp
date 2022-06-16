#ifndef SRC_UTIL_VARGS_VEC_OF_ANY_HPP
#define SRC_UTIL_VARGS_VEC_OF_ANY_HPP

// do not try to directly edit this file.
// generate using vec_of_any.py instead
#include <sstream>

#define VA(param) util::vec_of_any param

namespace util {

using std::string;
using std::stringstream;
using std::vector;

template <typename T>
static string s(T t) {
  stringstream ss;
  ss << t;
  return ss.str();
}

inline vector<string> vec_of_any() { return vector<string>(); }

template <class T0>
inline vector<string> vec_of_any(T0 arg0) {
  const string args[1] = {s(arg0)};
  return vector<string>(args, args + 1);
}

template <class T0, class T1>
inline vector<string> vec_of_any(T0 arg0, T1 arg1) {
  const string args[2] = {s(arg0), s(arg1)};
  return vector<string>(args, args + 2);
}

template <class T0, class T1, class T2>
inline vector<string> vec_of_any(T0 arg0, T1 arg1, T2 arg2) {
  const string args[3] = {s(arg0), s(arg1), s(arg2)};
  return vector<string>(args, args + 3);
}

template <class T0, class T1, class T2, class T3>
inline vector<string> vec_of_any(T0 arg0, T1 arg1, T2 arg2, T3 arg3) {
  const string args[4] = {s(arg0), s(arg1), s(arg2), s(arg3)};
  return vector<string>(args, args + 4);
}

template <class T0, class T1, class T2, class T3, class T4>
inline vector<string> vec_of_any(T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4) {
  const string args[5] = {s(arg0), s(arg1), s(arg2), s(arg3), s(arg4)};
  return vector<string>(args, args + 5);
}

template <class T0, class T1, class T2, class T3, class T4, class T5>
inline vector<string> vec_of_any(T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                                 T5 arg5) {
  const string args[6] = {s(arg0), s(arg1), s(arg2), s(arg3), s(arg4), s(arg5)};
  return vector<string>(args, args + 6);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6>
inline vector<string> vec_of_any(T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                                 T5 arg5, T6 arg6) {
  const string args[7] = {s(arg0), s(arg1), s(arg2), s(arg3),
                          s(arg4), s(arg5), s(arg6)};
  return vector<string>(args, args + 7);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6,
          class T7>
inline vector<string> vec_of_any(T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                                 T5 arg5, T6 arg6, T7 arg7) {
  const string args[8] = {s(arg0), s(arg1), s(arg2), s(arg3),
                          s(arg4), s(arg5), s(arg6), s(arg7)};
  return vector<string>(args, args + 8);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6,
          class T7, class T8>
inline vector<string> vec_of_any(T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                                 T5 arg5, T6 arg6, T7 arg7, T8 arg8) {
  const string args[9] = {s(arg0), s(arg1), s(arg2), s(arg3), s(arg4),
                          s(arg5), s(arg6), s(arg7), s(arg8)};
  return vector<string>(args, args + 9);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6,
          class T7, class T8, class T9>
inline vector<string> vec_of_any(T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                                 T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9) {
  const string args[10] = {s(arg0), s(arg1), s(arg2), s(arg3), s(arg4),
                           s(arg5), s(arg6), s(arg7), s(arg8), s(arg9)};
  return vector<string>(args, args + 10);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6,
          class T7, class T8, class T9, class T10>
inline vector<string> vec_of_any(T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                                 T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9,
                                 T10 arg10) {
  const string args[11] = {s(arg0), s(arg1), s(arg2), s(arg3), s(arg4), s(arg5),
                           s(arg6), s(arg7), s(arg8), s(arg9), s(arg10)};
  return vector<string>(args, args + 11);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6,
          class T7, class T8, class T9, class T10, class T11>
inline vector<string> vec_of_any(T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                                 T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9,
                                 T10 arg10, T11 arg11) {
  const string args[12] = {s(arg0), s(arg1), s(arg2),  s(arg3),
                           s(arg4), s(arg5), s(arg6),  s(arg7),
                           s(arg8), s(arg9), s(arg10), s(arg11)};
  return vector<string>(args, args + 12);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6,
          class T7, class T8, class T9, class T10, class T11, class T12>
inline vector<string> vec_of_any(T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                                 T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9,
                                 T10 arg10, T11 arg11, T12 arg12) {
  const string args[13] = {s(arg0),  s(arg1),  s(arg2), s(arg3), s(arg4),
                           s(arg5),  s(arg6),  s(arg7), s(arg8), s(arg9),
                           s(arg10), s(arg11), s(arg12)};
  return vector<string>(args, args + 13);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6,
          class T7, class T8, class T9, class T10, class T11, class T12,
          class T13>
inline vector<string> vec_of_any(T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                                 T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9,
                                 T10 arg10, T11 arg11, T12 arg12, T13 arg13) {
  const string args[14] = {s(arg0),  s(arg1),  s(arg2),  s(arg3), s(arg4),
                           s(arg5),  s(arg6),  s(arg7),  s(arg8), s(arg9),
                           s(arg10), s(arg11), s(arg12), s(arg13)};
  return vector<string>(args, args + 14);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6,
          class T7, class T8, class T9, class T10, class T11, class T12,
          class T13, class T14>
inline vector<string> vec_of_any(T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                                 T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9,
                                 T10 arg10, T11 arg11, T12 arg12, T13 arg13,
                                 T14 arg14) {
  const string args[15] = {s(arg0),  s(arg1),  s(arg2),  s(arg3),  s(arg4),
                           s(arg5),  s(arg6),  s(arg7),  s(arg8),  s(arg9),
                           s(arg10), s(arg11), s(arg12), s(arg13), s(arg14)};
  return vector<string>(args, args + 15);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6,
          class T7, class T8, class T9, class T10, class T11, class T12,
          class T13, class T14, class T15>
inline vector<string> vec_of_any(T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                                 T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9,
                                 T10 arg10, T11 arg11, T12 arg12, T13 arg13,
                                 T14 arg14, T15 arg15) {
  const string args[16] = {s(arg0),  s(arg1),  s(arg2),  s(arg3),
                           s(arg4),  s(arg5),  s(arg6),  s(arg7),
                           s(arg8),  s(arg9),  s(arg10), s(arg11),
                           s(arg12), s(arg13), s(arg14), s(arg15)};
  return vector<string>(args, args + 16);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6,
          class T7, class T8, class T9, class T10, class T11, class T12,
          class T13, class T14, class T15, class T16>
inline vector<string> vec_of_any(T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                                 T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9,
                                 T10 arg10, T11 arg11, T12 arg12, T13 arg13,
                                 T14 arg14, T15 arg15, T16 arg16) {
  const string args[17] = {s(arg0),  s(arg1),  s(arg2),  s(arg3),  s(arg4),
                           s(arg5),  s(arg6),  s(arg7),  s(arg8),  s(arg9),
                           s(arg10), s(arg11), s(arg12), s(arg13), s(arg14),
                           s(arg15), s(arg16)};
  return vector<string>(args, args + 17);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6,
          class T7, class T8, class T9, class T10, class T11, class T12,
          class T13, class T14, class T15, class T16, class T17>
inline vector<string> vec_of_any(T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                                 T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9,
                                 T10 arg10, T11 arg11, T12 arg12, T13 arg13,
                                 T14 arg14, T15 arg15, T16 arg16, T17 arg17) {
  const string args[18] = {s(arg0),  s(arg1),  s(arg2),  s(arg3),  s(arg4),
                           s(arg5),  s(arg6),  s(arg7),  s(arg8),  s(arg9),
                           s(arg10), s(arg11), s(arg12), s(arg13), s(arg14),
                           s(arg15), s(arg16), s(arg17)};
  return vector<string>(args, args + 18);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6,
          class T7, class T8, class T9, class T10, class T11, class T12,
          class T13, class T14, class T15, class T16, class T17, class T18>
inline vector<string> vec_of_any(T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                                 T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9,
                                 T10 arg10, T11 arg11, T12 arg12, T13 arg13,
                                 T14 arg14, T15 arg15, T16 arg16, T17 arg17,
                                 T18 arg18) {
  const string args[19] = {s(arg0),  s(arg1),  s(arg2),  s(arg3),  s(arg4),
                           s(arg5),  s(arg6),  s(arg7),  s(arg8),  s(arg9),
                           s(arg10), s(arg11), s(arg12), s(arg13), s(arg14),
                           s(arg15), s(arg16), s(arg17), s(arg18)};
  return vector<string>(args, args + 19);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6,
          class T7, class T8, class T9, class T10, class T11, class T12,
          class T13, class T14, class T15, class T16, class T17, class T18,
          class T19>
inline vector<string> vec_of_any(T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4,
                                 T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9,
                                 T10 arg10, T11 arg11, T12 arg12, T13 arg13,
                                 T14 arg14, T15 arg15, T16 arg16, T17 arg17,
                                 T18 arg18, T19 arg19) {
  const string args[20] = {s(arg0),  s(arg1),  s(arg2),  s(arg3),  s(arg4),
                           s(arg5),  s(arg6),  s(arg7),  s(arg8),  s(arg9),
                           s(arg10), s(arg11), s(arg12), s(arg13), s(arg14),
                           s(arg15), s(arg16), s(arg17), s(arg18), s(arg19)};
  return vector<string>(args, args + 20);
}

}  // namespace util

#endif  // SRC_UTIL_VARGS_VEC_OF_ANY_HPP
