#ifndef HELPERS_INCLUDED
#define HELPERS_INCLUDED

#include <cstdio>
#include <cstdarg>

namespace helper {

inline void debugf(const char *fmt, ...)
{
	#ifdef DEBUG
	  va_list args;
	  va_start(args, fmt);

	  // Add DEBUG: prefix to anything logged by this function
	  char *debug_fmt = new char[strlen(fmt) + 16];
	  const char *prefix = "DEBUG: "; 
	  strcat(debug_fmt, prefix);
	  strcat(debug_fmt, fmt);
	  vfprintf(stdout, debug_fmt, args);
	  va_end(args);
	  delete debug_fmt;
	#else
	  // suppress unused variable warning
	  (void) fmt;
	#endif
}

} // namespace helper

#endif
