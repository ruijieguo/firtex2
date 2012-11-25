#ifndef __SUBSTMAP_H
#define __SUBSTMAP_H


/* On MS-DOS and WIN32 files have to have 3-char extension */
#if defined(__MSDOS__) || defined(_WIN32)
# ifndef SPEC_EXT
#  define SPEC_EXT ".spc"
# endif
# ifndef REPL_EXT 
#  define REPL_EXT ".rpl"
# endif
#else

/* On other system we'll rename them to something more readable */
# ifndef SPEC_EXT
#  define SPEC_EXT ".specchars"
# endif
# ifndef REPL_EXT
#  define REPL_EXT ".replchars"
# endif
#endif

class Substmap
{
public:
	typedef char *** SUBSTMAP;
public:
	Substmap();
	~Substmap(void);
public:
	void	read(const char* spec_file,const char* repl_file) ;
	char*	mapSpec (int uc) ;
	char*	mapRepl (int uc) ;
public:
	static int isstop(char  c, char stop) ;
protected:
	SUBSTMAP read_substmap(char* filename);
	void map_insert(SUBSTMAP map, int uc, const char *s) ;
protected:
	SUBSTMAP spec_chars, replacements;
	int longest_sequence;	
	char* input_buffer;
};

#endif
