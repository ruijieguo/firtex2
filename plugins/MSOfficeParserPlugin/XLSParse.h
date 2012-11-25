#ifndef __XLSPARSE_H
#define __XLSPARSE_H

#include <stdio.h>
#include "xltypes.h"
#include <math.h>
#include <time.h>
#include <string>
/* types of quoting */
#define QUOTE_NEVER 0
#define QUOTE_SPACES_ONLY 1
#define QUOTE_ALL_STRINGS 2
#define QUOTE_EVERYTHING 3

struct rowdescr {
    int last, end;
    unsigned char **cells;
};	

class Analyze;
class XLSParse
{
public:
    XLSParse(Analyze* pAnalyze);
    ~XLSParse(void);
public:
    void parse_sst(char *sstbuf,int bufsize);

    void do_table(FILE *input);

protected:
    time_t float2date(double f);

    char* format_rk(char *rec,short int format_code);
    char *format_int(int value,int format_code);

    char *format_double(char *rec,int offset,int format_code);
    char *number2string(double d,short int format_code) ;
    const char *isDateFormat(int format_code);

    static void SetFormatIdxUsed(int format_code);
    static void CleanUpFormatIdxUsed();
    static const char* GetBuiltInDateFormat(int dateindex);
    static char IsFormatIdxUsed(int format_code);
    static int BuiltInDateFormatIdx (int index) ;

    char* copy_unicode_string (unsigned char **src);

    void process_item (int rectype, int reclen, char *rec);
protected:
    void print_sheet(void) ;
    void print_value(unsigned char *value) ;
    void free_sheet(void) ;
    unsigned char **allocate (int row,int col) ;

protected:
    inline void appendChar(unsigned short int u);
    inline void appendString(const char* szStr);

    void	flush();
protected:
    unsigned char **sst;/* Shared string table parsed into array of strings in
                           output encoding*/
    int sstsize; /*Number of strings in SST*/
    unsigned char *sstBuffer; /*Unparsed sst to accumulate all its parts*/
    int sstBytes; /*Size of SST Data, already accumulated in the buffer */
    int codepage; /*default*/
    int prev_rectype;
    /* holds a pointer to formula value, becouse value itself would be in
     * next biff record
     */
    unsigned char **saved_reference;


    static unsigned char rec[MAX_MS_RECSIZE];

    int biff_version;
    short int *formatTable;
    char *forced_date_format;
    size_t formatTableIndex;
    size_t formatTableSize;
    double date_shift;	
    char number_format[8];

    struct rowdescr *rowptr;
    int startrow,lastrow;
    char cell_separator ;
    int quote_mode;
    std::string sheet_separator;

    Analyze* m_pAnalyze;

    unsigned short int m_buffer[512];
    size_t m_pos;
};

inline void XLSParse::appendChar(unsigned short int u)
{
    if(m_pos > 511)
        flush();
    m_buffer[m_pos++] = u;
}

inline void XLSParse::appendString(const char* szStr)
{
    char* p = (char*)szStr;
    while( *p )
    {
        if(m_pos > 511)
            flush();
        m_buffer[m_pos++] = *p++;
    }
}

#endif
