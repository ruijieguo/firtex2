#ifndef __ANALYZE_H
#define __ANALYZE_H
#include <stdio.h>

/* Default charsets */
#ifndef TARGET_CHARSET
#if defined(__MSDOS__) || defined(_WIN32)
#define TARGET_CHARSET "cp866"
#else
#define TARGET_CHARSET "koi8-r"
#endif
#endif

#ifndef SOURCE_CHARSET
#define SOURCE_CHARSET "cp1251"
#endif

#ifndef UNKNOWN_CHAR
#define UNKNOWN_CHAR "?"
#endif

# ifndef CHARSETPATH
#  define CHARSETPATH "./../plugins/charsets"
# endif

/* Charset files distributed with catdoc always have .txt extension*/
#ifndef CHARSET_EXT
# define CHARSET_EXT ".txt"
#endif

/* Buffers for 32-bit and more program */
#define PARAGRAPH_BUFFER 262144
#define FILE_BUFFER 262144
#define PATH_BUF_SIZE 1024

/* Separator of directories in list, such as PATH env var. */
# define LIST_SEP ';'
/* Separator of levels inside path */
# define DIR_SEP '/'

/*   Default value for wrap margin */
#ifndef WRAP_MARGIN
#define WRAP_MARGIN 72
#endif

/* Buffer for single line. Should be greater than wrap margin +
   longest substitution sequence */
#define LINE_BUF_SIZE 512

class Ole;
class Reader;
class RTFRead;
class Writer;
class Substmap;
class Charsets;
class CharGetter;
class PPTParse;
class XLSParse;

class Analyze
{
public:
    Analyze(void);
    Analyze(Writer* pWriter);
    ~Analyze(void);

public:
    int parseDOC(FILE* f);
    void parsePPT(FILE* f);
    void parseXLS(FILE* f);
    int parse_word_header(unsigned char * buffer,FILE *f,int offset,long curpos);	
public:
    Charsets* getCharsets(){return m_pCharsets;}
    Writer* getWriter(){return m_pWriter;};
    Reader* getReader(){return m_pReader;}
    RTFRead* getRTFRead(){return m_pRTFRead;}
    Ole* getOle(){return m_pOleFile;}
    Substmap* getSubstmap(){return m_pSubstmap;}

    void setCharGetter(int cm);
    CharGetter* getCharGetter(){return m_pCharGetter;}

protected:
    int	 m_nVerbose;
    Ole* m_pOleFile;
    Reader* m_pReader;
    RTFRead* m_pRTFRead;
    Writer* m_pWriter;
    Charsets* m_pCharsets;
    Substmap* m_pSubstmap;
    CharGetter* m_pCharGetter;

    PPTParse* m_pPPTParse;
    XLSParse* m_pXLSParse;

    bool m_bOwnWriter;
protected:
    static const int fDot = 0x0001;
    static const int fGlsy = 0x0002;
    static const int fComplex = 0x0004;
    static const int fPictures = 0x0008;
    static const int fEncrypted = 0x100;
    static const int fReadOnly = 0x400;
    static const int fReserved = 0x800;
    static const int fExtChar = 0x1000;
};


#endif
