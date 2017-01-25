#include <stdarg.h>
#include <stdio.h>

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
