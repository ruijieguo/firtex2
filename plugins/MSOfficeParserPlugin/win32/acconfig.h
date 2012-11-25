#undef VERSION
#undef PACKAGE


#include "rint.h"

/*
#define SYSTEMRC       $(SYSTEMRC)
#define USERRC         $(USERRC)
#define CHARSETPATH    $(CHARSETPATH)
#define SPEC_EXT       $(SPEC_EXT)
#define REPL_EXT       $(REPL_EXT)
#define UNKNOWN_CHAR   $(UNKNOWN_CHAR) 
#define SOURCE_CHARSET $(SOURCE_CHARSET)
#define TARGET_CHARSET $(TARGET_CHARSET)
*/

#undef  HAVE_UNISTD_H 
#define HAVE_STDLIB_H  1
#define HAVE_IO_H      1
#define HAVE_WINDOWS_H 1

#ifndef MAX_PATH
#define MAX_PATH 1024
#endif
#define isatty _isatty
#define strncasecmp  strnicmp
#define strcasecmp  stricmp