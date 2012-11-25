#ifndef __CHARSETS_H
#define __CHARSETS_H



class Analyze;
class Charsets
{
public:
    /* Structure to store UNICODE -> target charset mappings */
/* array of 256 pointers (which may be null) to arrays of 256 short ints
   which contain 8-bit character codes or -1 if no matching char */
    typedef short int  ** CHARSET;
public:
    Charsets(Analyze* pAnalyze);
    ~Charsets();
public:
    static int to_unicode (short int *charset, int c);
    int to_unicode (int c);
    static int from_unicode (CHARSET charset, int u) ;
    short int * read_charset() ;

    char *convert_char(int uc) ;
    static char *to_utf8(unsigned int uc) ;
    static const char *charset_from_codepage(unsigned int codepage) ;

    short int* getSourceCharset(){return source_charset;}	
protected:
    CHARSET make_reverse_map(short int *charset) ;

protected:
    short int * source_charset;
    int unknown_as_hex;
    static char bad_char[];
    CHARSET target_charset;

    char* input_buffer;

    Analyze* m_pAnalyze;
};
#endif
