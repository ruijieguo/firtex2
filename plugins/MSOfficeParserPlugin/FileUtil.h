#ifndef __FILEUTIL_H
#define __FILEUTIL_H

class FileUtil
{
public:
    static int prepare_path_buf(char *path_buf, const char *start, const char *end) ;
    static char *find_file(char *name, const char *path);
    static int check_charset(char **filename,const char *charset,const char* charset_path) ;
    static char *stradd(const char *s1,const char *s2) ;
    static char * exe_dir(void);
#if defined(__MSDOS__) || defined(_WIN32)
    static char *add_exe_path(const char *name) ;
#endif
    static void list_charsets(const char* charset_path) ;
};

#endif
