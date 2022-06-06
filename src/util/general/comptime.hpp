#ifndef UTIL_COMPTIME_HPP
#define UTIL_COMPTIME_HPP

#define COMPTIME_ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define COMPTIME_ARRAY_END(a) (a + COMPTIME_ARRAY_SIZE(a))
#define COMPTIME_CONTAINER_INIT(a) (a, COMPTIME_ARRAY_END(a))

#endif  // UTIL_COMPTIME_HPP
