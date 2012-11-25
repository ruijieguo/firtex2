/*
  Copyright 1998-2003 Victor Wagner
  Copyright 2003 Alex Ott
  This file is released under the GPL.  Details can be
  found in the file COPYING accompanying this distribution.
*/
//#ifdef HAVE_CONFIG_H
#ifdef WIN32
#include <acconfig.h>
#endif
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "Analyze.h"
#include "Ole.h"
#include "NumUtils.h"
#include "Reader.h"
#include "CharGetter.h"
#include "RTFRead.h"
#include "Writer.h"
#include "Charsets.h"
#include "Substmap.h"
#include "ConfUtil.h"
#include "PPTParse.h"
#include "XLSParse.h"

static unsigned char ole_sign[]={0xD0,0xCF,0x11,0xE0,0xA1,0xB1,0x1A,0xE1,0};
static char rtf_sign[]="{\\rtf";
static unsigned char old_word_sign[]={0xdb,0xa5,0};
static unsigned char write_sign[]={0x31,0xBE,0};

Analyze::Analyze()
    : m_nVerbose(0)
    , m_pOleFile(new Ole())
    , m_pCharGetter(NULL)
    , m_pPPTParse(NULL)
    , m_pXLSParse(NULL)
    , m_bOwnWriter(true)
{
    m_pWriter = new Writer(this);
    m_pCharsets = new Charsets(this);
    m_pReader = new Reader(this);
    m_pRTFRead = new RTFRead(this);
    m_pSubstmap = new Substmap();
}

Analyze::Analyze(Writer* pWriter)
    : m_nVerbose(0)
    , m_pOleFile(new Ole())
    , m_pWriter(pWriter)
    , m_pCharGetter(NULL)
    , m_pPPTParse(NULL)
    , m_pXLSParse(NULL)
    , m_bOwnWriter(false)
{
    m_pCharsets = new Charsets(this);
    m_pReader = new Reader(this);
    m_pRTFRead = new RTFRead(this);
    m_pSubstmap = new Substmap();
}

Analyze::~Analyze()
{
    delete m_pOleFile;
    delete m_pReader;
    if(m_bOwnWriter)
        delete m_pWriter;
    if(m_pCharGetter)
    {
        delete m_pCharGetter;
        m_pCharGetter = NULL;
    }
    delete m_pSubstmap;
    delete m_pCharsets;
    delete m_pRTFRead;
    if( m_pPPTParse )
    {
        delete m_pPPTParse;
        m_pPPTParse = NULL;
    }
    if( m_pXLSParse )
    {
        delete m_pXLSParse;
        m_pXLSParse = NULL;
    }
}

/********************************************************************* 
 * Determines format of input file and calls parse_word_header or 
 * m_pReader->process_file if
 * it is word processor file or copy_out if it is plain text file
 * return not 0 when error
 ********************************************************************/ 
int Analyze::parseDOC(FILE *f) 
{
    unsigned char buffer[129];
    long offset=0;
    FILE *new_file, *ole_file;
    int ret_code=69;

    if (!GlobalConf.signature_check) {
        /* forced parsing */
        /* no autodetect possible. Assume 8-bit if not overriden on
         * command line */ 
        if (!getCharGetter()) 
            setCharGetter(CHAR_8BIT);
        return m_pReader->process_file(f,LONG_MAX);
    }
    m_pOleFile->ole_read(buffer,4,1,f);
    buffer[4]=0;
    if (strncmp((const char*)buffer,(const char*)write_sign,2)==0) {
        printf("[Windows Write file. Some garbage expected]\n");
        setCharGetter(CHAR_8BIT);
        return m_pReader->process_file(f,LONG_MAX);
    } else if (strncmp((const char*)buffer,(const char*)rtf_sign,4)==0) {
        return m_pRTFRead->parse_rtf(f);
    } else if (strncmp((const char*)buffer,(const char*)old_word_sign,2)==0) {
        size_t readLen = fread(buffer+4,1,124,f);
        (void)readLen;
        return parse_word_header(buffer,f,128,0);
    }
    size_t readLen = fread(buffer+4,1,4,f);
    (void)readLen;
    if (strncmp((const char*)buffer,(const char*)ole_sign,8)==0) {
        if ((new_file=m_pOleFile->ole_init(f, buffer, 8)) != NULL) {
            m_pOleFile->set_ole_func();
            while((ole_file=m_pOleFile->ole_readdir(new_file)) != NULL) {
                int res=m_pOleFile->ole_open(ole_file);
                if (res >= 0) {
                    if (strcmp(((oleEntry*)ole_file)->name , "WordDocument") == 0) {
                        offset=m_pOleFile->ole_read(buffer, 1, 128, ole_file);
                        ret_code=parse_word_header(buffer,ole_file,-offset,offset);
                    }
                } 
                m_pOleFile->ole_close(ole_file);
            }
            m_pOleFile->set_std_func();
            m_pOleFile->ole_finish();
        } else {
            fprintf(stderr,"Broken OLE file. Try using -b switch");
            exit(1);
        }	
    } else {
        m_pOleFile->set_std_func();
        m_pReader->copy_out(f,(char*)buffer);
        return 0;
    }

    return ret_code;
}   

void Analyze::parsePPT(FILE* f)
{
    if(!m_pPPTParse)
        m_pPPTParse = new PPTParse(this);

    FILE *new_file, *ole_file;

    if ((new_file=m_pOleFile->ole_init(f, NULL, 0)) != NULL) 
    {
        m_pOleFile->set_ole_func();
        while((ole_file = m_pOleFile->ole_readdir(new_file)) != NULL) 
        {
            int res = m_pOleFile->ole_open(ole_file);
/* 				fprintf(stderr, "name = %s\n", ((oleEntry*)ole_file)->name); */
            if (res >= 0) 
            {
                if (strcasecmp(((oleEntry*)ole_file)->name , "PowerPoint Document") == 0) 
                {
                    m_pPPTParse->do_ppt(ole_file);
                }
            } 
            m_pOleFile->ole_close(ole_file);
        }
        m_pOleFile->set_std_func();
        m_pOleFile->ole_finish();
        fclose(new_file);
    } 
    else 
    {
        fprintf(stderr, "The file is not OLE file or Error\n");
    }
}

void Analyze::parseXLS(FILE* f)
{
    if(!m_pXLSParse)
        m_pXLSParse = new XLSParse(this);

    FILE *new_file, *ole_file;
    if ((new_file=m_pOleFile->ole_init(f, NULL, 0)) != NULL) 
    {
        m_pOleFile->set_ole_func();
        while((ole_file = m_pOleFile->ole_readdir(new_file)) != NULL) 
        {
            int res = m_pOleFile->ole_open(ole_file);
            /* fprintf(stderr, "name = %s\n", ((oleEntry*)ole_file)->name); */
            if (res >= 0) 
            {
                if (strcasecmp(((oleEntry*)ole_file)->name , "Workbook") == 0
                    || strcasecmp(((oleEntry*)ole_file)->name,"Book") == 0) 
                {
                    m_pXLSParse->do_table(ole_file);
                }
            } 
            m_pOleFile->ole_close(ole_file);
        }
        m_pOleFile->set_std_func();
        m_pOleFile->ole_finish();
        fclose(new_file);
    } 
    else 
    {
        fprintf(stderr, "%s is not OLE file or Error\n", "XLS");//filename);
    }
}

/*******************************************************************/
/* parses word file info block passed in buffer.  
 * Determines actual size of text stream and calls m_pReader->process_file  
 ********************************************************************/
int Analyze::parse_word_header(unsigned char * buffer,FILE *f,int offset,long curpos) {
    int flags,charset, ret_code=0;
    long textstart,textlen,i;
    char buf[2];
	
    if (m_nVerbose) {
        printf("File Info block version %d\n",NumUtils::getshort(buffer,2));
        printf("Found at file offset %ld (hex %lx)\n",curpos,curpos);
        printf("Written by product version %d\n",NumUtils::getshort(buffer,4));
        printf("Language %d\n",NumUtils::getshort(buffer,6));
    }
    flags = NumUtils::getshort(buffer,10);
    if (m_nVerbose) {
        if ((flags & fDot)) {
            printf("This is template (DOT) file\n");
        } else {
            printf("This is document (DOC) file\n");
        }
        if (flags & fGlsy) {
            printf("This is glossary file\n");
        }
    }
    if (flags & fComplex) {
        fprintf(stderr,"[This was fast-saved %2d times. Some information is lost]\n",
                (flags & 0xF0)>>4);
/*		ret_code=69;*/
    }
    if (m_nVerbose) {
        if (flags & fReadOnly) {
            printf("File is meant to be read-only\n");
        }
        if (flags & fReserved) {
            printf("File is write-reserved\n");
        }
    }
    if (flags & fExtChar) {
        if (m_nVerbose) {
            printf ("File uses extended character set\n");
        }
        if (!getCharGetter()) 
            setCharGetter(WORD8_CHAR);

    } else if (!getCharGetter()) 
        setCharGetter(CHAR_8BIT);

    if (m_nVerbose) {
        if (buffer[18]) {
            printf("File created on Macintosh\n");
        } else {
            printf("File created on Windows\n");
        } 
    }
    if (flags & fEncrypted) {
        fprintf(stderr,"[File is encrypted. Encryption key = %08lx]\n",
                NumUtils::getlong(buffer,14));
        return 69;
    }
    if (m_nVerbose) {
        charset=NumUtils::getshort(buffer,20);
        if (charset&&charset !=256) {
            printf("Using character set %d\n",charset);
        } else {
            printf("Using default character set\n");
        }
    }
    /* skipping to textstart and computing textend */
    textstart=NumUtils::getlong(buffer,24);
    textlen=NumUtils::getlong(buffer,28)-textstart;
    textstart+=offset;
    if (m_nVerbose) {
        printf ("Textstart = %ld (hex %lx)\n",textstart+curpos,textstart+curpos);
        printf ("Textlen =   %ld (hex %lx)\n",textlen,textlen);
    }   
    for (i=0;i<textstart;i++) {
        m_pOleFile->ole_read(buf, 1, 1, f);
        if (m_pOleFile->ole_eof(f)) {
            fprintf(stderr,"File ended before textstart. Probably it is broken. Try -b switch\n");
            exit(1);
        }
    }    
    return m_pReader->process_file(f,textlen) || ret_code;
}   


void Analyze::setCharGetter(int cm)
{
    if(m_pCharGetter && m_pCharGetter->getMode() == cm)
        return ;
    if(m_pCharGetter)
        delete m_pCharGetter;
    m_pCharGetter = CharGetterFactory::createGetter(cm,this);
}
