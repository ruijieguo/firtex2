/*
  Copyright 1998-2003 Victor Wagner
  Copyright 2003 Alex Ott
  This file is released under the GPL.  Details can be
  found in the file COPYING accompanying this distribution.
*/
//#ifdef HAVE_CONFIG_H
#ifdef WIN32
#include <windows.h>
#include <acconfig.h>
#else
//#ifdef HAVE_UNISTD_H
#include <unistd.h>
//#endif
#endif

#ifdef HAVE_IO_H
#include <io.h>
#endif
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "FileUtil.h"
#include "Analyze.h"

#if defined(MSDOS) && !defined(__MSDOS__)
#define __MSDOS__
#endif
#if defined(__MSDOS__) || defined(_WIN32)
//#include <dir.h>
//#include <dos.h>
#else
#include <glob.h>
#endif
#ifdef _WIN32
#include <windows.h>
#define MAX_FNAME _MAX_FNAME
#define findfirst(a,b) _findfirst(a,b)
#define findnext(a,b) _findnext(a,b)
#define findclose(a) _findclose(a)
#define finddata_t _finddata_t
#endif

/************************************************************************/
/*  Copies component of string starting with p and ending one char      */
/*  before q into path_buf, expanding ~ if neccessary                   */
/************************************************************************/
int FileUtil::prepare_path_buf(char *path_buf, const char *start, const char *end) {
    if (*start == '~' && start[1] == DIR_SEP) {
        char *home=getenv("HOME");
        start++;
        if (!home) {
            if (end-start>PATH_BUF_SIZE) return 0;
            strncpy(path_buf,start,end-start);
            path_buf[end-start]=0;
        } else {
            int l = strlen(home);
            if (l+(end-start)>PATH_BUF_SIZE) return 0;
            strcpy(path_buf,home);
            strncpy(path_buf+l,start,end-start);
            path_buf[end-start+l]=0;
        }	
    } else {	
        if (end-start>PATH_BUF_SIZE) return 0;
        strncpy(path_buf,start,end-start);
        path_buf[end-start]=0;
    }
    /* Empty list element means current directory */
    if (!*path_buf) {
        path_buf[0]='.';
        path_buf[1]=0;
#if defined(__MSDOS__) || defined(_WIN32)
    } else {
        strcpy(path_buf,add_exe_path(path_buf)); /* safe, becouse
                                                    add_exe_path knows about PATH_BUF_SIZE */
#endif
    }
    return 1;

}	
/************************************************************************/
/* Searches for file name in specified list of directories. Sets        */
/* Returns dynamically allocated full path or NULL. if nothing          */
/* appropriate   Expects name to be dynamically allocated and frees it  */
/************************************************************************/
char *FileUtil::find_file(char *name, const char *path)
{ const char *p;
    const char *q;
    char path_buf[PATH_BUF_SIZE];
    char dir_sep[2]={DIR_SEP,0};
    for (p=path;p;p=q+1) {
        q=strchr(p,LIST_SEP);

        if (q) {
            if (!prepare_path_buf(path_buf,p,q)) continue;
        } else {
            q--;
            if (!prepare_path_buf(path_buf,p,p+strlen(p))) continue;
        }
        strcat(path_buf,dir_sep); /* always one char */
        if (strlen(path_buf)+strlen(name)>=PATH_BUF_SIZE) 
            continue; /* Ignore too deeply nested directories */
        strcat(path_buf,name);
        if (access(path_buf,0)==0) {
            free(name); 
            return strdup(path_buf);
        }
    }
    /* if we are here, nothing found */
    free(name); 
    return NULL;
}

/************************************************************************/
/* Searches for charset with given name and put pointer to malloced copy*/
/* of its name into first arg if found. Otherwise leaves first arg      */
/*  unchanged. Returns non-zero on success                              */ 
/************************************************************************/
int FileUtil::check_charset(char **filename,const char *charset,const char* charset_path) {
    char *tmppath;
    if (!strncmp(charset,"utf-8",6)) {
        *filename=strdup("utf-8");
        return 1;
    }   
    tmppath=find_file(stradd(charset,CHARSET_EXT),charset_path);
    if (tmppath&& *tmppath) {
        *filename=strdup(charset);
        free(tmppath);
        return 1;
    }
    return 0;
}

/**********************************************************************/
/*  Returns malloced string containing concatenation of two           */
/*  arguments                                                         */
/**********************************************************************/
char *FileUtil::stradd(const char *s1,const char *s2) 
{ char *res;
    res=(char*)malloc(strlen(s1)+strlen(s2)+1);
    if (!res) {
        fprintf (stderr,"Out of memory!");
        exit(1);
    }
    strcpy(res,s1);
    strcat(res,s2);
    return res;
}  


/*
 * In DOS, argv[0] contain full path to the program, and it is a custom
 * to keep configuration files in same directory as program itself
 */
#if defined(_WIN32)
/* Win32 hack to get libexecdir at runtime */
/* Caller should free memory returned */
char * FileUtil::exe_dir(void){
    char *fn;
    char *tr;
    int pos;

    fn = (char*)malloc(MAX_PATH+1);
    /* get the full name of the executable */
    if(!GetModuleFileNameA(NULL,fn,MAX_PATH))
    {
        free( fn );
        return(".");
    }
	
    /* get the base directory */
    tr = strrchr(fn, '\\');
    pos = tr - fn;
    fn[pos]='\0';
	
    /* get the prefix directory */
    tr = strrchr(fn, '\\');
    pos = tr - fn;

    /* if we're in bin we'll assume prefix is up one level */
    /* if(!strncasecmp(&fn[pos+1], "bin\0", 4))
       fn[pos]='\0';*/
	
    return(fn);
}
#elif defined(__MSDOS__)
char *FileUtil::exe_dir(void) {
    static char pathbuf[PATH_BUF_SIZE];
    char *q;
    strcpy(pathbuf,_argv[0]); /* DOS ensures, that our exe path is no
                                 longer than PATH_BUF_SIZE*/
    q=strrchr(pathbuf,DIR_SEP);
    if (q) {
        *q=0;
    } else {
        pathbuf[0]=0;
    }
    return pathbuf;
}
#endif

#if defined(_WIN32)
char *FileUtil::add_exe_path(const char *name) {
    static char path[PATH_BUF_SIZE];
    char *mypath=exe_dir();
    /* No snprintf in Turbo C 2.0 library, so just check by hand
       and exit if something goes wrong */
    if (strchr(name,'%')) {
        /* there is substitution */
        if (strlen(name)-1+strlen(mypath)>=PATH_BUF_SIZE) {
            fprintf(stderr,"Invalid config file. file name \"%s\" too long "
                    "after substitution\n",name);
            exit(1);
        }   
        char* tmp = exe_dir();
        sprintf(path,name,tmp);
        free(tmp);
        free(mypath);
        return path;
    } else {
        free(mypath);
        return (char*)name;
    }  
}
#elif defined (__MSDOS__)
char *FileUtil::add_exe_path(const char *name) {
    static char path[PATH_BUF_SIZE];
    char *mypath=exe_dir();
    /* No snprintf in Turbo C 2.0 library, so just check by hand
       and exit if something goes wrong */
    if (strchr(name,'%')) {
        /* there is substitution */
        if (strlen(name)-1+strlen(mypath)>=PATH_BUF_SIZE) {
            fprintf(stderr,"Invalid config file. file name \"%s\" too long "
                    "after substitution\n",name);
            exit(1);
        }   
        sprintf(path,name,exe_dir());
        return path;
    } else {
        return (char*)name;
    }  
}
#endif 
/*********************************************************************/
/* Prints out list of available charsets, i.e. names without extension *
 * of all .txt files in the charset path + internally-supported utf-8  *
 ************************************************************************/ 

void FileUtil::list_charsets(const char* charset_path) {
    const char *p;
    char *q;
    char path_buf[PATH_BUF_SIZE];
    char dir_sep[2]={DIR_SEP,0};
#if defined(__MSDOS__)
    struct ffblk ffblock;
    int res,col;
    char **ptr;
#elif defined(_WIN32)
    struct _finddata_t ffblock;
    long ffhandle;
#else
    glob_t glob_buf;
    int count,glob_flags=GLOB_ERR;
    char **ptr;
#endif
    for (p=charset_path;p;p=q+1) {
        q=(char*)strchr(p,LIST_SEP);

        if (q) {
            if (q-p>=PATH_BUF_SIZE) {
                /* Oops, dir name too long, perhabs broken config file */
                continue;
            }
            strncpy(path_buf,p,q-p);
            path_buf[q-p]=0;
        } else {
            q--;
            if (strlen(p)>=PATH_BUF_SIZE) continue;
            strcpy(path_buf,p);
        }
        /* Empty list element means current directory */
        if (!*path_buf) {
            path_buf[0]='.';
            path_buf[1]=0;
#if defined(__MSDOS__) || defined(_WIN32)
        } else {
            strcpy(path_buf,add_exe_path(path_buf)); /* safe, becouse
                                                        add_exe_path knows about PATH_BUF_SIZE */
#endif
        }
        strcat(path_buf,dir_sep); /* always one char */
        if (strlen(path_buf)+6>=PATH_BUF_SIZE)
            continue; /* Ignore too deeply nested directories */
        strcat(path_buf,"*.txt");
#if defined(__MSDOS__) || defined(_WIN32)
#ifdef _WIN32
        if( !(( ffhandle = findfirst(path_buf,&ffblock)) == -1L ) ){
            while ( _findnext( ffhandle, &ffblock ) == 0 ) {
                char name[MAX_FNAME],*src,*dest;
                dest=name;
                src=ffblock.name;
                for (dest=name,src=ffblock.name;*src && *src !='.';dest++,src++)
                    *dest=tolower(*src);
            }
        }
#else
        res=findfirst(path_buf,&ffblock,FA_RDONLY | FA_HIDDEN | FA_ARCH);
        col=1;
        printf("Available charsets:\n"); 
        while (!res) {
            char name[12],*src,*dest;
            dest=name;
            src=ffblock.ff_name;
            for (dest=name,src=ffblock.ff_name;*src && *src !='.';dest++,src++)
                *dest=tolower(*src);
            *dest++=(col<5)?'\t':'\n';
            if (++col>5) col=1;
            *dest=0;
            printf("%10s",name);
            res=findnext(&ffblock);
        }
#endif
#else
        switch (glob(path_buf,glob_flags,NULL,&glob_buf)) {
        case 0:
#ifdef GLOB_NOMATCH
        case GLOB_NOMATCH:
#endif
            break;
        default:
            perror("catdoc");
            exit(1);
        }
        glob_flags|=GLOB_APPEND;
#endif
    }
#if defined(__MSDOS__) || defined(_WIN32)
    fputs("utf-8\n",stdout);
#else
    count=0;printf("Available charsets:"); 
    for (ptr=glob_buf.gl_pathv;*ptr;ptr++) {
        printf("%c",(count++)%5?'\t':'\n');
        p=strrchr(*ptr,dir_sep[0]);
        if (!p) continue;
        p++;
        if ((q=(char*)strchr(p,'.'))) *q=0;
        fputs(p,stdout);
    }  
    printf("%c",(count++)%5?'\t':'\n');
    fputs("utf-8",stdout);
    printf("\n");
    globfree(&glob_buf);
#endif   
}    
