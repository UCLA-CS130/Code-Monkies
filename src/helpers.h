#ifndef HELPERS_INCLUDED
#define HELPERS_INCLUDED

#include <cstdio>
#include <cstdarg>

namespace helpers {

/*
 * Print debugging statement out to terminal. 'scope' is intended to be used to
 * indicate the calling function/method. Useful for primitive stack tracing.
 */
inline void debugf(const char *scope, const char *fmt, ...)
{
	#ifdef DEBUG
    fprintf(stdout, "DEBUG: %s: ", scope);

	  va_list args;
	  va_start(args, fmt);

	  vfprintf(stdout, fmt, args);
	  va_end(args);
	#else
	  // suppress unused variable warning
	  (void) scope;
	  (void) fmt;
	#endif
}

} // namespace helpers

#endif
