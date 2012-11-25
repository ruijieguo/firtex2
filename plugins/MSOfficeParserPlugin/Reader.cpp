/*****************************************************************/
/* Reading routines for MS-Word, MS-Write and text files         */
/*                                                               */
/* This file is part of catdoc project                           */
/* (c) Victor Wagner 1996-2003, (c) Alex Ott 2003	             */
/*****************************************************************/
//#ifdef HAVE_CONFIG_H
#ifdef WIN32
#include <acconfig.h>
#endif
#include <string.h>
#include <stdio.h>
#include "Reader.h"
#include "Analyze.h"
#include "Charsets.h"
#include "CharGetter.h"
#include "Ole.h"
#include "Writer.h"

//static unsigned char read_buf[256];
//static int buf_is_unicode;

Reader::Reader(Analyze* pAnalyze)
	: m_pAnalyze(pAnalyze)	
{
}

Reader::~Reader()
{	
}

/**************************************************************************/
/* Just prints out content of input file. Called when file is not OLE     */
/* stream                                                                 */
/* Parameters - f - file to copy out. header - first few bytes of file,   */
/*  which have been already read by format recognition code, but should   */
/*  be output anyway                                                      */
/**************************************************************************/
void Reader::copy_out (FILE *f,char *header) {
	char *buf=(char *)buffer;
	int count,i;
	long offset;
	if (m_pAnalyze->getCharGetter() && m_pAnalyze->getCharGetter()->getMode() == WORD8_CHAR) {
		/* non-word file and -u specified. Trying to guess which kind of
		 * unicode is used
		 */
		if ((unsigned char)header[0]==0xFE && (unsigned char)header[1]==0xFF) {
			m_pAnalyze->setCharGetter(UTF16MSB_CHAR);
			fputs(m_pAnalyze->getCharsets()->convert_char(header[2]<<8|header[3]),stdout); 
			fputs(m_pAnalyze->getCharsets()->convert_char(header[4]<<8|header[5]),stdout); 
			fputs(m_pAnalyze->getCharsets()->convert_char(header[6]<<8|header[7]),stdout); 
		} else if ((unsigned char)header[0]!=0xFF ||
				(unsigned char)header[1]!=0xFE) {
			int c,j,d;
			/* if it is not utf16, assume it is UTF8. We are told -u,
			 * aren't we */
			m_pAnalyze->setCharGetter(UTF8_CHAR);
			i=0;
			while (i<8) {
				c=(unsigned char)header[i++];		
				if (c >=0x80) {
					if ( c<0xE0) {
						c=(c & 0x1F);
						count =1;
					} else {
						c=(c & 0xF);
						count = 2;
					}
					for (j=0;j<count;j++) {
						if (i<7) {
							d=(unsigned char) header[i++];
						} else {
							d=fgetc(f);
						}
						c=c<<6 | (d & 0x3F);
					}
				}
				fputs (m_pAnalyze->getCharsets()->convert_char(c),stdout);
			}
		} else {
			m_pAnalyze->setCharGetter(UTF16LSB_CHAR);
			fputs(m_pAnalyze->getCharsets()->convert_char(header[3]<<8|header[2]),stdout); 
			fputs(m_pAnalyze->getCharsets()->convert_char(header[5]<<8|header[4]),stdout); 
			fputs(m_pAnalyze->getCharsets()->convert_char(header[7]<<8|header[6]),stdout); 
		}	    
		while (!m_pAnalyze->getOle()->ole_eof(f)) {
			i=m_pAnalyze->getCharGetter()->getChar(f,&offset,0x7FFFFFFF); 
			if (i!=EOF) fputs(m_pAnalyze->getCharsets()->convert_char(i),stdout);
		}    
	} else {
		for (i=0;i<8;i++) {
			fputs(m_pAnalyze->getCharsets()->convert_char(m_pAnalyze->getCharsets()->to_unicode((unsigned char)header[i])),stdout);
		}			 
		/* Assuming 8-bit input text */
		while ((count = m_pAnalyze->getOle()->ole_read(buf,1,PARAGRAPH_BUFFER,f))) {
			for (i=0;i<count;i++) {
				fputs(m_pAnalyze->getCharsets()->convert_char(m_pAnalyze->getCharsets()->to_unicode((unsigned char)buf[i])),stdout);
			}		       
		}
	} 
} 
/**************************************************************************/
/*  process_file - main process engine. Reads word file using function,   */
/*  pointed by get_unicode_char, searches for things which looks like     */
/*  paragraphs and print them out                                         */
/**************************************************************************/
int Reader::process_file(FILE *f,long stop) {
	int bufptr;
	int tabmode=0;
	long offset=0;
	int hyperlink_mode = 0;
	unsigned short c;
	/* Now we are starting to read with get_unicode_char */
	while (!m_pAnalyze->getOle()->ole_eof(f) && offset<stop) {
		bufptr = -1;
		do {
			c=m_pAnalyze->getCharGetter()->getChar(f,&offset,stop);
			/* Following symbols below 32 are allowed inside paragraph:
			   0x0002 - footnote mark
			   0x0007 - table separator (converted to tabmode)
			   0x0009 - Horizontal tab ( printed as is)
			   0x000B - hard return
			   0x000C - page break
			   0x000D - return - marks an end of paragraph
			   0x001E - IS2 for some reason means short defis in Word.
			   0x001F - soft hyphen in Word
			   0x0013 - start embedded hyperlink
			   0x0014 - separate hyperlink URL from text
			   0x0015 - end embedded hyperlink
			   */
			if (tabmode) {
				tabmode=0;
				if (c==0x007) {
					buffer[++bufptr]=0x1E;
					continue;
				} else {
					buffer[++bufptr]=0x1C;
				}  
			}   	 
			if (c<32) {
				switch (c) {
					case 0x007:
						tabmode = 1;
						break;
					case 0x000D:
					case 0x000B:
						buffer[++bufptr]=0x000A;
						break;
					case 0x000C:
						buffer[++bufptr]=c;
						break;
					case 0x001E:
						buffer[++bufptr]='-';
						break;
					case 0x0002: break;

					case 0x001F:
								 buffer[++bufptr]=0xAD;/* translate to Unicode
														  soft hyphen */
								 break;						  
					case 0x0009:
								 buffer[++bufptr]=c;
								 break;
					case 0x0013:
								 hyperlink_mode=1;
								 buffer[++bufptr]=' ';
								 break;
					case 0x0014:
								 hyperlink_mode = 0;
								 /*fall through */
					case 0x0015:
								 /* just treat hyperlink separators as
								  * space */
								 buffer[++bufptr]=' ';
								 break;
					case 0x0001: if (hyperlink_mode) 
									 	break;
								 /* else fall through */
					default:
								 bufptr=-1; /* Any other control char - discard para*/
				}
			} else if (c != 0xfeff) {
				/* skip ZERO-WIDTH-UNBREAKABLE-SPACE. Output anything
				 * else*/
				buffer[++bufptr]=c;
			}
		} while (bufptr<PARAGRAPH_BUFFER-2 &&
			!m_pAnalyze->getOle()->ole_eof(f) &&
				 buffer[bufptr]!=0x000a);
		if (bufptr>0) {
			buffer[++bufptr]=0;
			m_pAnalyze->getWriter()->output_paragraph(buffer);
		}
	}
	return 0;
}


