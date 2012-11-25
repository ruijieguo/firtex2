/*****************************************************************/
/* Substitution maps to replace some unicode characters with     */
/* multicharacter sequences                                      */
/*                                                               */
/* This file is part of catdoc project                           */
/* (c) Victor Wagner 1998-2003, (c) Alex Ott 2003	             */
/*****************************************************************/
//#ifdef HAVE_CONFIG_H
#ifdef WIN32
#include <acconfig.h>
#endif
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "Substmap.h"
#include "FileUtil.h"
#include "ConfUtil.h"
#include "Analyze.h"

Substmap::Substmap()
	: spec_chars(NULL)
	, replacements(NULL)
	, longest_sequence(6)
{
	input_buffer = new char[FILE_BUFFER];
	spec_chars = read_substmap(FileUtil::stradd(GlobalConf.format_name,SPEC_EXT));
	replacements = read_substmap(FileUtil::stradd(GlobalConf.format_name,REPL_EXT));	
}

Substmap::~Substmap()
{
	if(spec_chars)
	{
		for(size_t i = 0;i < 256;i++)
		{
			if(spec_chars[i])
			{
				for(size_t j = 0;j<256;j++)
					if(spec_chars[i][j])
						free(spec_chars[i][j]);
				free(spec_chars[i]);
			}
		}
		free(spec_chars);
	}
	if(replacements)
	{
		for(size_t i = 0;i < 256;i++)
			if(replacements[i])
			{
				for(size_t j = 0;j < 256;j++)
					if(replacements[i][j])
						free(replacements[i][j]);
				free(replacements[i]);
			}
		free(replacements);
	}
	delete[] input_buffer;
}

/******************************************************************/
/* checks for terminator of character sequence. If stop is something
 * like quote - check for same char. If it is \n, check for any space
 ********************************************************************/ 
int Substmap::isstop(char  c, char stop) 
{
	if (stop=='\n')
		return isspace(c);
	else 
		return c==stop;
}
/************************************************************************/
/* Reads substitution map file.                                         */
/************************************************************************/
Substmap::SUBSTMAP Substmap::read_substmap(char* filename) 
{
	FILE *f;
	SUBSTMAP map = (SUBSTMAP)calloc(sizeof(char **),256);
	memset(map,0,256 * sizeof(char**));

	char *path, line[1024], *p, *q;
	char s[256];
	char stopchar;
	int escaped, lineno=0, i;
	unsigned int uc;
#ifdef WIN32
	path = FileUtil::find_file( filename,FileUtil::add_exe_path(GlobalConf.map_path) );
#else
	path = FileUtil::find_file( filename, GlobalConf.map_path);
#endif
	if (!path) 
	{
		free(map);
		return(NULL);
	}   
	if (!map) 
	{
		fprintf(stderr,"Insufficient memory\n");
		exit(1);
	}
	f = fopen(path,"rb");
	if (!f)
	{
		perror("catdoc");
		return NULL;
	}
	if (input_buffer)
		setvbuf(f,input_buffer,_IOFBF,FILE_BUFFER);

	while (!feof(f)) 
	{
		if ( !fgets(line,1024,f) ) continue;
		lineno++;
		/* parse line */

		/* skip leading space */
		for(p = line;*p && isspace(*p);p++);
		/* if #, it is comment */
		if (!*p ||
#ifdef  __MSDOS__
			*p==0x1A || /* DOS have strange habit of using ^Z as eof */
#endif
			*p=='#') continue;
		/* read hexadecimal code */
		uc = strtol(p,&p,16);
		if (!isspace(*p)|| uc<0 || uc>0xfffd) {
			fprintf(stderr,"Error parsing %s(%d)\n",path,lineno);
			continue;
		}
		/* skip space between  code and sequence */
		for(;*p && isspace(*p);p++);
		if (!p) continue;
		switch (*p) {
			case '\'':
			case '"':
				stopchar=*p;
				break;
			case '(':
				stopchar=')';
				break;
			case '[':
				stopchar=']';
				break;
			case '{':
				stopchar='}';
				break;
			default:
				p--;
				stopchar='\n';
		}
		p++;
		q=p;
		escaped=0;
		while (*q && (!isstop(*q,stopchar) || escaped)) {
			if (escaped) {
				escaped=0;
			} else {
				escaped= (*q=='\\');
			}
			q++;
		}
		if (*q!=stopchar && !(isspace(*q) && stopchar=='\n')) {
			fprintf(stderr,"Error parsing %s(%d): unterminated sequence\n",
				path,lineno);
			continue;
		}
		/* HERE SHOULD BE BACKSLASH ESCAPE PROCESSING !!!*/
		*q = 0;
		for (q = s,i = 0;*p && i < 256;q++,i++) 
		{
			if (*p!='\\') 
			{
				*q=*p++;
			} 
			else 
			{
				switch (*(++p)) 
				{
					case 'n': *q='\n'; break;
					case 'r': *q='\r'; break;
					case 't': *q='\t'; break;
					case 'b': *q='\b'; break;
					case '\"': *q='\"'; break;
					case '\'': *q='\''; break;
					case '0': *q=strtol(p,&p,8); p--; break;
					case '\\':
					default:
						*q=*p;
				}
				p++;
			}
		}
		*q=0;
		if (i > longest_sequence)
			longest_sequence = i;
		map_insert(map,uc,s);
	}
	fclose(f);
	free(path);
	return map;
}

/*************************************************************************/
/*  inserts string + unicode code into map                               */
/*************************************************************************/
void Substmap::map_insert(Substmap::SUBSTMAP map, int uc, const char *s) 
{
	SUBSTMAP p = map + ((unsigned)uc >> 8);

	if (!*p) 
	{
		*p= (char**)calloc(sizeof(char*),256);		
		if (!*p)
		{ 
			fprintf(stderr,"Insufficient memory\n");
			exit(1);
		}
		memset( *p,0,256 * sizeof(char*) );
	}
	(*p)[uc & 0xff] = strdup(s);
}

void Substmap::read(const char* spec_file,const char* repl_file) 
{
	spec_chars = read_substmap(FileUtil::stradd(spec_file,SPEC_EXT));
	if (!spec_chars) 
	{
		fprintf(stderr,"Cannot read substitution map %s%s\n",GlobalConf.format_name,
				SPEC_EXT);
		exit(1);
	}  

	replacements = read_substmap(FileUtil::stradd(repl_file,REPL_EXT));
	if (!replacements)
	{
		fprintf(stderr,"Cannot read replacement map %s%s\n",GlobalConf.format_name,
				REPL_EXT);
		exit(1);
	}  
}

char * Substmap::mapSpec (int uc) 
{
	char **p = spec_chars[(unsigned)uc >> 8];
	if (!p) return NULL;
	return p[uc & 0xff];
}

char * Substmap::mapRepl (int uc) 
{
	char **p=replacements[(unsigned)uc >> 8];
	if (!p) return NULL;
	return p[uc & 0xff];
}
