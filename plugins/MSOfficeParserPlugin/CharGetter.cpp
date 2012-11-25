#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "CharGetter.h"

#include "Analyze.h"
#include "Ole.h"
#include "Charsets.h"


CharGetter::CharGetter()
	: m_pAnalyze(NULL)
	, m_charMode(CHAR_UNKNOW)
{
}

CharGetter::CharGetter(CHAR_MODE cm,Analyze* pAnalyze)
	: m_pAnalyze(pAnalyze)
	, m_charMode(cm)
{
}

CharGetter::~CharGetter(void)
{
}

void CharGetter::set(CHAR_MODE cm,Analyze* pAnalyze)
{
	m_pAnalyze = pAnalyze;
	m_charMode = cm;
}

/************************************************************************/
/* Reads 8-bit char and convers it from source charset                  */
/************************************************************************/

Char_8Bit::Char_8Bit(CHAR_MODE cm,Analyze* pAnalyze)
	: CharGetter(cm,pAnalyze)
{
}

int Char_8Bit::getChar(FILE *f,long *offset,long fileend)
{
	unsigned char buf;
	if (m_pAnalyze->getOle()->ole_read(&buf, 1, 1, f)==0) return EOF;
	(*offset)++;  
	return m_pAnalyze->getCharsets()->to_unicode(buf);
}


Char_UTF16LSB::Char_UTF16LSB(CHAR_MODE cm,Analyze* pAnalyze)
	: CharGetter(cm,pAnalyze)
{
}
/************************************************************************/
/* Reads 16-bit unicode value. MS-Word runs on LSB-first machine only,  */
/* so read lsb first always and don't care about proper bit order       */
/************************************************************************/

int Char_UTF16LSB::getChar (FILE *f,long *offset,long fileend) {
	unsigned char buf[2];
    int result;
	result=m_pAnalyze->getOle()->ole_read(buf, 1, 2, f);
	if (result<0) {
		perror("read:");
		//exit(1);
	}
	if (result !=2) {
		return EOF;
	}	
	(*offset)+=2;
	return ((int)buf[1])|(((int)buf[0])<<8);
}

////////////////////////////////////////////////////////////
///
Char_UTF16MSB::Char_UTF16MSB(CHAR_MODE cm,Analyze* pAnalyze)
	: CharGetter(cm,pAnalyze)
{
}

/************************************************************************/
/* Reads 16-bit unicode value written in MSB order. For processing 
 * non-word files            .                                          */
/************************************************************************/
int Char_UTF16MSB::getChar (FILE *f,long *offset,long fileend) {
	unsigned char buf[2];
    int result;
	result=m_pAnalyze->getOle()->ole_read(buf, 1, 2, f);
	if (result<0) {
		perror("read:");
		//exit(1);
	}
	if (result !=2) {
		return EOF;
	}	
	(*offset)+=2;
	return ((int)buf[0])|(((int)buf[1])<<8);
}

////////////////////////////////////////////////////////////
///
Char_UTF8::Char_UTF8(CHAR_MODE cm,Analyze* pAnalyze)
	: CharGetter(cm,pAnalyze)
{
}

int Char_UTF8::getChar (FILE *f,long *offset,long fileend) {
	unsigned char buf[3];
	int d,c;
    int result;
	result=m_pAnalyze->getOle()->ole_read(buf, 1, 1, f);
	if (result<0) {
		perror("read");
		//exit(1);
	}	
	if (result==0) return EOF;
	c=buf[0];
	d=0;
	if (c<0x80) 
		return c;
	if (c <0xC0) 
		return 0xfeff; /*skip corrupted sequebces*/
	if (c <0xE0) {
		if (m_pAnalyze->getOle()->ole_read(buf+1, 1, 1, f)<=0) return EOF;
		return ((c & 0x1F)<<6 | ((char)buf[1] & 0x3F));
	}
	if (c <0xF0) {
		if (m_pAnalyze->getOle()->ole_read(buf+1, 1, 2, f)<=2) return (int)EOF;
		return ((c & 0x0F)<<12)|
			((buf[1] & 0x3f)<<6)|
					 (buf[2] & 0x3f);
	}  
	return 0xFEFF; 
}

////////////////////////////////////////////////////////////
///
Char_Word8::Char_Word8(CHAR_MODE cm,Analyze* pAnalyze)
	: CharGetter(cm,pAnalyze)
{
}

/**********************************************************************/
/* Reads file from MS-Word 97 and above file. Takes in account strange*
 * situation that unicode and non-unicode 256-byte blocks could be    *
 * intermixed in word file                                            *
 *                                                                    *
 * Parameters:                                                        *
 *                                                                    *
 * f - file to read                                                   *
 * offset - position of the character inside file (to determine       * 
 * possible  block boundaries                                         *
 **********************************************************************/ 
int Char_Word8::getChar(FILE *f,long *offset,long fileend) {
	int count,i,u;
	char c;
	if ((i=(*offset)%256) ==0) {
		count=m_pAnalyze->getOle()->ole_read(read_buf,1,256,f);
		memset(read_buf+count,0,256-count);
		buf_is_unicode=0;
		if (*offset+(long)count>fileend) {
			count=fileend-*offset;
		}	
		while (i<count) {
			c=read_buf[i++];
			if ((c==0x20|| c==0x0D|| (c > 0 && ispunct(c)) )&&i<count&&read_buf[i]==0) {
				buf_is_unicode=1;
				break;
			}
			i++;
		}   
		i=0;
	}    
	if (buf_is_unicode) {
		u=read_buf[i] | read_buf[i+1]<<8;
		(*offset)+=2;
	} else {
		u=m_pAnalyze->getCharsets()->to_unicode(read_buf[i]);
		(*offset)++;
	}
	return u;
}  



CharGetter* CharGetterFactory::createGetter(CHAR_MODE cm,Analyze* pAnalyze)
{	
	CharGetter* pGetter = NULL;
	switch(cm)
	{
	case CHAR_8BIT:
		pGetter = new Char_8Bit(cm,pAnalyze);
		break;
	case UTF16LSB_CHAR:
		pGetter = new Char_UTF16LSB(cm,pAnalyze);
		break;
	case UTF16MSB_CHAR :
		pGetter = new Char_UTF16MSB(cm,pAnalyze);
		break;
	case UTF8_CHAR:
		pGetter = new Char_UTF8(cm,pAnalyze);
		break;	
	case WORD8_CHAR:
		pGetter = new Char_Word8(cm,pAnalyze);
		break;
	}	
	return pGetter;
}
