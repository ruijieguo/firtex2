#ifndef __FX_PORTABILITY_H
#define __FX_PORTABILITY_H

#include "firtex/common/Platform.h"
#if !defined(FX_WINDOWS)
#include "config.h"
#endif
#include <stdarg.h>

#if defined(FX_WINDOWS)

#define VSNPRINTF _vsnprintf
#define SNPRINTF _snprintf
#define STRTOLL _strtoi64 
#define STRTOULL _strtoui64

#else

#if defined(HAVE_VPRINTF) || defined(FX_POSIX)
#define VSNPRINTF vsnprintf
#define SNPRINTF snprintf
#else
/* use alternative snprintf() from http://www.ijs.si/software/snprintf/ */

#define HAVE_VPRINTF
#define PREFER_PORTABLE_SNPRINTF

#include <stdlib.h>
#include <stdarg.h>

extern int portable_snprintf(char *str, size_t str_m, const char *fmt, /*args*/ ...);
extern int portable_vsnprintf(char *str, size_t str_m, const char *fmt, va_list ap);

#define VSNPRINTF portable_vsnprintf
#define SNPRINTF portable_snprintf

#endif // HAVE_VPRINTF

#define STRTOLL strtoll 
#define STRTOULL strtoull

#endif // FX_WINDOWS

#endif
