#ifndef __CONFUTIL_H
#define __CONFUTIL_H

class ConfUtil
{
public:
	ConfUtil();
	~ConfUtil(void);
public:
	void read_config_file(const char* filename) ;
#ifdef HAVE_LANGINFO
	void get_locale_charset() ;
#	ifndef __TURBOC__
	void set_time_locale() ;
#	endif
#endif

#ifndef HAVE_STRDUP
	char *strdup(const char *s) ;
#endif
public:
	char*	source_csname;
	char*	dest_csname;
	char*	format_name;
	char*	charset_path;
	char*	map_path;
	char	bad_char[2];
	int		runtime_locale_check;
	int		signature_check;
	int		forced_charset;
};

extern ConfUtil GlobalConf;

#endif
