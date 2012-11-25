#ifdef HAVE_CONFIG_H
#include "acconfig.h"
#endif

/* Win32 hack to get libexecdir at runtime */
/* Caller should free memory returned */
char *
exe_dir(void){
	char *fn;
	char *tr;
	int pos;

	fn = emalloc(MAX_PATH+1);
	/* get the full name of the executable */
	if(!GetModuleFileNameA(NULL,fn,MAX_PATH))
	{
		efree( fn );
		return(libexecdir);
	}
	
	/* get the base directory */
	tr = strrchr(fn, '\\');
	pos = tr - fn;
	fn[pos]='\0';
	
	/* get the prefix directory */
	tr = strrchr(fn, '\\');
	pos = tr - fn;

	/* if we're in bin we'll assume prefix is up one level */
	if(!strncasecmp(&fn[pos+1], "bin\0", 4))
		fn[pos]='\0';

	/* Tack on the libexecdir */
	strcpy(fn+strlen(fn), "\\lib\\swish-e");
	
	return(fn);
}