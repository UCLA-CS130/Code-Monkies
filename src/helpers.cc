#include <stdarg.h>
#include <stdio.h>

#include "helpers.h"

/*
 * Acts the same as printf, but with two benefits:
 *  1. We can toggle debug statements with a single ifdef, rather than around
 *    every single statement.
 *  2. We can change how all debug statements look/work down the road by
 *    changing one function.
 */
void debugf(const char *fmt, ...)
{
#ifdef DEBUG
  va_list args;
  va_start(args, fmt);
  vfprintf(stdout, fmt, args);
  va_end(args);
#else
  // suppress unused variable warning
  (void) fmt;
#endif
}
