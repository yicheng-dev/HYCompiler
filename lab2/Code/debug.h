#ifndef _DEBUG_H
#define _DEBUG_H

#include "utils.h"

#define assert(cond) \
    do { \
      if (!(cond)) { \
        printf("\033[31mAssertion fail at %s:%d\n\033[0m", __FILE__, __LINE__); \
        exit(1); \
      } \
    } while (0)

#define TRACEME
#ifdef TRACEME
  #define TRACE_ENTRY \
    printf("\033[34m[trace]\033[0m %s:entry\n", __func__)
  #define TRACE_EXIT \
    printf("\033[34m[trace]\033[0m %s:exit\n", __func__)
#else
  #define TRACE_ENTRY ((void)0)
  #define TRACE_EXIT ((void)0)
#endif

#define Log(format, ...) \
  do { \
    printf("\33[1;34m[%s,%d,%s] " format "\33[0m\n", \
        __FILE__, __LINE__, __func__, ## __VA_ARGS__); \
  } while (0)

#define TestLog(format, ...) \
  do { \
    printf("\33[1;32m[%s,%d,%s] " format "\33[0m\n", \
        __FILE__, __LINE__, __func__, ## __VA_ARGS__); \
  } while (0)

#endif
