
#ifndef SANDY_COMMON_H
#define SANDY_COMMON_H

#if __STDC_VERSION__ >= 199901L
#define C99
#else
#endif

#include <stdint.h>

#ifdef C99
  #include <stdbool.h>
#else
  typedef unsigned char bool;
  #define true 1
  #define false 0
#endif

typedef unsigned char u8;
typedef uint32_t u32;

#endif /* SANDY_COMMON_H */
