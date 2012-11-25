/*
  Copyright 1998-2003 Victor Wagner
  Copyright 2003 Alex Ott
  This file is released under the GPL.  Details can be
  found in the file COPYING accompanying this distribution.
*/
//#ifdef HAVE_CONFIG_H
#ifdef WIN32
#include "acconfig.h"
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Charsets.h"
#include "Analyze.h"
#include "Substmap.h"
#include "FileUtil.h"
#include "ConfUtil.h"

char Charsets::bad_char[] = UNKNOWN_CHAR;

Charsets::Charsets(Analyze* pAnalyze)
    : source_charset(NULL)
    , unknown_as_hex(0)
    , target_charset(NULL)
    , m_pAnalyze(pAnalyze)
{
    input_buffer = new char[FILE_BUFFER];
    this->source_charset = this->read_charset();
}

Charsets::~Charsets()
{
    delete[] input_buffer;
    free(this->source_charset);
}

/************************************************************************/
/* Converts char in input charset into unicode representation           */
/* Should be converted to macro                                         */
/************************************************************************/
int Charsets::to_unicode (short int *charset, int c) {
    return charset[c];
}

int Charsets::to_unicode (int c) {
    return source_charset[c];
}
/************************************************************************/
/* Search inverse charset record for given unicode char and returns     */
/* 0-255 char value if found, -1 otherwise                              */
/************************************************************************/
int Charsets::from_unicode (CHARSET charset, int u) {
    short int *p;
    /* This is really assignment, not comparation */
    if ((p=charset[(unsigned)u>>8])) {
        return p[u & 0xff];
    } else {
        return -1;
    }
}

/************************************************************************/
/*  Converts direct (charset -> unicode) to reverse map                 */
/************************************************************************/
Charsets::CHARSET Charsets::make_reverse_map(short int *charset) {
    CHARSET newmap=(CHARSET)calloc(sizeof(short int *), 256);
    int i,j,k,l;
    short int *p;   
    if (! charset) {
        return NULL;
    }	
    for (i=0;i<256;i++) {
        k= charset[i];
        j=  (unsigned)k>>8;
        if (!newmap[j]) {
            newmap[j] = (short*)malloc(sizeof(short int *)*256);
            if (!newmap[j]) {
                fprintf(stderr,"Insufficient memory for  charset\n");
                exit(1);
            }
            for (l=0,p=newmap[j];l<256;l++,p++) *p=-1;
        }
        p=newmap[j];
        p[k & 0xff]=i;
    }
    return newmap;
}

/************************************************************************/
/* Reads charset file (as got from ftp.unicode.org) and returns array of*/
/* 256 short ints (malloced) mapping from charset t unicode             */
/************************************************************************/
short int * Charsets::read_charset() {
    char *path;
    FILE *f;
    short int *pNew=(short int*)calloc(sizeof(short int),256);
    int c;
    long int uc;
    path=FileUtil::find_file(FileUtil::stradd(GlobalConf.source_csname,CHARSET_EXT),GlobalConf.charset_path);
    if (!path) {
        fprintf(stderr,"Cannot load charset %s - file not found\n",GlobalConf.charset_path);
        free(pNew);
        return NULL;
    }
    f=fopen(path,"rb");

    if (!f) {
        perror(path); 
        free(pNew);
        return NULL;
    }
    if (input_buffer)
        setvbuf(f,input_buffer,_IOFBF,FILE_BUFFER);
    /* defaults */
    for (c=0;c<32;c++) {
        pNew[c]=c;
    }
    while (!feof(f)) {
        if (fscanf(f,"%i %li",&c,&uc)==2) {
            if (c<0||c>255||uc<0||(uc>0xFEFE&& uc!=0xFFFE)) {
                fprintf(stderr,"Invalid charset file %s\n",path);
                fclose(f);
                free(pNew);
                return NULL;
            }
            pNew[c]=(short)uc;
        }
        while((fgetc(f)!='\n')&&!feof(f)) ;
    }
    fclose (f);
    free(path);
    return pNew;
}


/**************************************************************************/
/*  Converts unicode char to output charset sequence. Coversion have      */
/*  three steps: 1. Replacement map is searched for the character in case */
/* it is not allowed for output format (% in TeX, < in HTML               */
/* 2. target charset is searched for this unicode char, if it wasn't      */
/*  replaced. If not found, then 3. Substitution map is searched          */
/**************************************************************************/
char *Charsets::convert_char(int uc) {
    static char plain_char[]="a"; /*placeholder for one-char sequences */
    static char hexbuf[8];
    char *mapped;
    int c;
    if ((mapped=m_pAnalyze->getSubstmap()->mapSpec(uc))) return mapped;
    if (target_charset) { 
        c =from_unicode(target_charset,uc);
        if (c>=0) {
            *plain_char=c;
            return plain_char;
        }
        if ((mapped = m_pAnalyze->getSubstmap()->mapRepl(uc))) return mapped;
        if (unknown_as_hex) {
            sprintf(hexbuf,"\\x%04X",(unsigned)uc);
            /* This sprintf is safe, becouse uc is unicode character code,
               which cannot be greater than 0xFFFE. It is ensured by routines
               in reader.c
            */
            return hexbuf;
        }   
        return  bad_char;
    } else {
        /* NULL target charset means UTF-8 output */
        return to_utf8(uc);
    }  
}
/******************************************************************/
/* Converts given unicode character to the utf-8 sequence         */
/* in the static string buffer. Buffer wouldbe overwritten upon   */
/* next call                                                      */
/******************************************************************/ 
char *Charsets::to_utf8(unsigned int uc) {
    static char utfbuffer[4]; /* it shouldn't overflow becouse we never deal
                                 with chars greater than 65535*/
    int count=0;
    if (uc< 0x80) {
        utfbuffer[0]=uc;
        count=1;
    } else  {
        if (uc < 0x800) {
            utfbuffer[count++]=0xC0 | (uc >> 6);
        } else {
            utfbuffer[count++]=0xE0 | (uc >>12);
            utfbuffer[count++]=0x80 | ((uc >>6) &0x3F);
        }	    
        utfbuffer[count++]=0x80 | (uc & 0x3F);
    }  
    utfbuffer[count]=0;
    return utfbuffer;
}    

struct cp_map {
    int codepage;
    char charset_name[20];
};

struct cp_map cp_to_charset [] = {
    {10000,"mac-roman"},
    {10001,"mac-japanese"},
    {10002,"mac-tchinese"},
    {10003,"mac-korean"},
    {10004,"mac-arabic"},
    {10005,"mac-hebrew"},
    {10006,"mac-greek1"},
    {10007,"mac-cyrillic"},
    {10008,"mac-schinese"},
    {10010,"mac-romania"},
    {10017,"mac-ukraine"},
    {10021,"mac-thai"},
    {10029,"mac-centeuro"},
    {10079,"mac-iselandic"},
    {10081,"mac-turkish"},
    {10082,"mac-croatia"},
    {20866,"koi8-r"},
    {28591,"8859-1"},
    {28592,"8859-2"},
    {28593,"8859-3"},
    {28594,"8859-4"},
    {28595,"8859-5"},
    {28596,"8859-6"},
    {28597,"8859-7"},
    {28598,"8859-8"},
    {28599,"8859-9"},
    {28605,"8859-15"},
    {65001,"utf-8"},
    {0, ""},
};

const char *Charsets::charset_from_codepage(unsigned int codepage) {
    static char buffer[7];
    struct cp_map *cp;
    if (codepage==1200||codepage==1201) {
        /* For UCS2 */
        return "";
    } else if (codepage<10000) {
        sprintf(buffer,"cp%d",codepage);
        return buffer;
    } else {
        for (cp = cp_to_charset;cp->codepage!=0&& (unsigned int)cp->codepage!=codepage;cp++);
        return cp->charset_name;
    }
}
