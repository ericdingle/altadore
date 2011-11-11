#ifndef BONAVISTA_LOGGING_ASSERT_H_
#define BONAVISTA_LOGGING_ASSERT_H_

#include <stdio.h>
#include <stdlib.h>

#define ASSERT(cond) \
  do { \
    if (!(cond)) { \
      printf("Assertion failure at %s:%d: %s\n", __FILE__, __LINE__, #cond); \
      exit(1); \
    } \
  } while(0)

#if DEBUG
#define DASSERT ASSERT
#else
#define DASSERT(cond) \
  do { \
  } while(0)
#endif

#endif
