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
#define MAX_PATH 128
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#ifdef HAVE_LANGINFO
#include <langinfo.h>
#ifndef __TURBOC__
#include <locale.h>
#endif
#endif
#include "ConfUtil.h"
#include "FileUtil.h"
#include "Analyze.h"

ConfUtil GlobalConf;

ConfUtil::ConfUtil()	
    : source_csname(new char[128])
    , dest_csname(new char[128])
    , format_name(new char[128])
    , charset_path(new char[MAX_PATH])
    , map_path(new char[MAX_PATH])	
    , runtime_locale_check(0)
    , signature_check(1)
    , forced_charset(0)
{	
    strncpy(source_csname,SOURCE_CHARSET,128);
    strncpy(dest_csname,TARGET_CHARSET,128);
    strncpy(format_name,"ascii",128);
    strncpy(charset_path,CHARSETPATH,MAX_PATH);
    strncpy(map_path,CHARSETPATH,MAX_PATH);
    strncpy(bad_char,UNKNOWN_CHAR,2);
}

ConfUtil::~ConfUtil()
{
    delete[] source_csname;
    delete[] dest_csname;
    delete[] format_name;
    delete[] charset_path;
    delete[] map_path;
}
/********************************************************************/
/*  Reads configuration file                                        */
/*                                                                  */
/********************************************************************/
void ConfUtil::read_config_file(const char* filename) { 
#ifdef WIN32
    FILE *f=fopen(FileUtil::add_exe_path(filename),"rb");
#else 
    FILE *f=fopen(filename,"rb");
#endif
    char *name,*value,line[1024],*c;
    int lineno=0;
    if (!f) return;
    while (!feof(f)) {
        fgets(line,1024,f);
        if (feof(f)) break;
        lineno++;
        if ((c=strchr(line,'#'))) *c='\0';
        name=line;
        while (*name&&isspace(*name)) name++;
        if (!*name) continue;
        for (value=name;*value&&(isalnum(*value)||*value=='_'); value++);  
        if (*value=='=') {
            *value=0;value++;
        } else {
            *value=0;value++;
            while(*value&&isspace(*value)) value++;
            if (*value++ != '=' ) {
                fprintf(stderr,"Error %s(%d): name = value syntax expected\n",
                        filename,lineno);
                //exit(1);
            }
            while(*value&&isspace(*value)) value++;
        }
        for (c=value;*c&&!isspace(*c);c++);
        if (value==c) {
            fprintf(stderr,"Error %s(%d): name = value syntax expected\n",
                    filename,lineno);
            //exit(1);
        }
        *c=0;
        if (!strcmp(name,"source_charset")) {
            strncpy(source_csname,value,128);
        } else if (!strcmp(name,"target_charset")) {
            strncpy(dest_csname,value,128);			
        } else if (!strcmp(name,"format")) {
            strncpy(format_name,value,128);			
        } else if (!strcmp(name,"charset_path")) {
            strncpy(charset_path,value,MAX_PATH);			
        } else if (!strcmp(name,"map_path")) {
            strncpy(map_path,value,MAX_PATH);			
        } else if (!strcmp(name,"unknown_char")) {
            if (*value=='"' && value[1] && value[2]=='"') value++;	
            if (*value=='\'' && value[1] && value[2]=='\'') value++;	
            bad_char[0] = *value;
        } else if (!strcmp(name,"use_locale")) {
            if (tolower(value[0])=='n') {
                runtime_locale_check=0;
            } else if (tolower(value[0])=='y') {
                runtime_locale_check=1;
            } else {
                fprintf(stderr,"Error %s(%d): use_locale requires 'yes' or 'no'\n",
                        filename,lineno);
                //exit(1);
            }	

        } else {
            fprintf(stderr,"Invalid configuration directive in %s(%d):,%s = %s\n",
                    filename,lineno,name,value);		
            //exit(1);
        }	
    }
    fclose(f);
}
#ifdef HAVE_LANGINFO
static char *locale_charset = NULL;
/*********************************************************************/
/*  Determines  output character set from current locale and puts it *
 *  into global variable dest_csname                                 *
 *********************************************************************/
void ConfUtil::get_locale_charset() {
    char *codeset;
    if (!runtime_locale_check) return;
#ifndef __TURBOC__	
    if (!setlocale(LC_CTYPE,"")) return;
#endif
#ifndef _WIN32
    codeset = nl_langinfo(CODESET);
#else
    /* Get ANSI Code Page, Windows equivalent of nl_langinfo */
    static char buf[2 + 10 + 1];
    sprintf (buf, "CP%u", GetACP ());
    codeset = buf;
#endif
    if (!strncmp(codeset,"ISO",3)||!strncmp(codeset,"iso",3)) {
        codeset+=3;
        if (*codeset=='-') codeset++;
        if (!strncmp(codeset,"646",3)) {
            /* ISO 646 is another name for us=ascii */
            check_charset(&dest_csname,"us-ascii") ;
        }	else {	 
            if (check_charset(&dest_csname,codeset)) {
                locale_charset = dest_csname;
            }
        }
    } else if (!strcmp(codeset,"ANSI_X3.4-1968")) {
        check_charset(&dest_csname,"us-ascii");
    } else if (!strncmp(codeset,"ANSI",4)||!strncmp(codeset,"ansi",4)) {
        char *newstr;
        if (*codeset=='-') {
            codeset++;
        }	
        newstr	= malloc(strlen(codeset)-4+2+1);
        strcpy(newstr,"cp");
        strcpy(newstr+2,codeset+4);
        if (check_charset(&dest_csname,newstr)) {
            locale_charset = dest_csname;
        }
        free(newstr);
    } else if (!strncmp(codeset,"IBM",3)) {
        char *newstr;
        codeset+=3;
        if (*codeset == '-') codeset++;
        newstr=malloc(strlen(codeset)+2+1);
        strcpy(newstr,"cp");
        strcpy(newstr+2,codeset);
        if (check_charset(&dest_csname, newstr)) {
            locale_charset=dest_csname;
        }
        free(newstr);
    } else {
        char *i,*newstr = strdup(codeset);
        for (i=newstr;*i;i++) {
            *i=tolower(*i);
        }	
        if (check_charset(&dest_csname,newstr)) {
            locale_charset = dest_csname;
        }	
    }	

}	
#ifndef __TURBOC__
void ConfUtil::set_time_locale() {
    if (!runtime_locale_check) return;
    if (!locale_charset) return;
    if (strcmp(locale_charset,dest_csname)!=0) return;
    setlocale(LC_TIME,"");		
}	
#endif
#endif
#ifndef HAVE_STRDUP
/* Implementation of strdup for systems which don't have it */
char *ConfUtil::strdup(const char *s) {
    int size=strlen(s);
    char *newstr;
    newstr=(char*)malloc(size+1);
    return strcpy(newstr,s);
}
#endif
