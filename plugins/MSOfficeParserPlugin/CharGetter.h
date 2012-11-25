#ifndef __CHARGETTER_H
#define __CHARGETTER_H



typedef int CHAR_MODE;

#define CHAR_UNKNOW		0
#define CHAR_8BIT		1
#define UTF16LSB_CHAR	2
#define UTF16MSB_CHAR	3
#define UTF8_CHAR		4
#define WORD8_CHAR		5

class Analyze;
class CharGetter
{
public:
    CharGetter();
    CharGetter(CHAR_MODE cm,Analyze* pAnalyze);
    virtual ~CharGetter();

    void	set(CHAR_MODE cm,Analyze* pAnalyze);

    CHAR_MODE getMode(){return m_charMode;}
protected:
    Analyze*	m_pAnalyze;
    CHAR_MODE m_charMode;
public:
    virtual int getChar(FILE *f,long *offset,long fileend) = 0;
};

class Char_8Bit : public CharGetter
{
public:
    Char_8Bit(CHAR_MODE cm,Analyze* pAnalyze);
    int getChar(FILE *f,long *offset,long fileend);
};

class Char_UTF16LSB : public CharGetter
{
public:
    Char_UTF16LSB(CHAR_MODE cm,Analyze* pAnalyze);
    int getChar(FILE *f,long *offset,long fileend);
};

class Char_UTF16MSB : public CharGetter
{
public:
    Char_UTF16MSB(CHAR_MODE cm,Analyze* pAnalyze);
    int getChar(FILE *f,long *offset,long fileend);
};

class Char_UTF8 : public CharGetter
{
public:
    Char_UTF8(CHAR_MODE cm,Analyze* pAnalyze);
    int getChar(FILE *f,long *offset,long fileend);
};

class Char_Word8 : public CharGetter
{
public:
    Char_Word8(CHAR_MODE cm,Analyze* pAnalyze);
    int getChar(FILE *f,long *offset,long fileend);
protected:
    unsigned char read_buf[256];
    int buf_is_unicode;
};

class CharGetterFactory
{
public:
    static CharGetter* createGetter(CHAR_MODE cm,Analyze* pAnalyze);
};

#endif
