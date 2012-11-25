#ifndef __TRFREADER_H
#define __TRFREADER_H

#include <string>

typedef enum {
	RTF_CODEPAGE,
	RTF_FONT_CHARSET,
	RTF_UC,
	RTF_UNICODE_CHAR,
	RTF_CHAR,
	RTF_PARA,
	RTF_TABLE_START,
	RTF_TABLE_END,
	RTF_ROW,
	RTF_CELL,
	RTF_UNKNOWN,
	RTF_OVERLAY,
	RTF_PICT,
	RTF_F,
	RTF_AUTHOR,
	RTF_FONTTBL,
	RTF_INFO,
	RTF_STYLESHEET,
	RTF_COLORTBL,
	RTF_LISTOVERRIDETABLE,
	RTF_LISTTABLE,
	RTF_RSIDTBL,
	RTF_GENERATOR,
	RTF_DATAFIELD,
	RTF_LANG,
	RTF_PARD,
	RTF_TAB,
	RTF_SPEC_CHAR,
	RTF_EMDASH,
	RTF_ENDASH,
	RTF_EMSPACE,
	RTF_ENSPACE,
 	RTF_BULLET, 
 	RTF_LQUOTE,
	RTF_RQUOTE,
	RTF_LDBLQUOTE,
	RTF_RDBLQUOTE,
	RTF_ZWNONJOINER,
} RTFTypes;

typedef struct {
    std::string name;
    RTFTypes type;
} RTFTypeMap;


#define RTFNAMEMAXLEN 32
#define RTFARGSMAXLEN 64

/**
 * Structure describing rtf command
 * 
 */
typedef struct {
	RTFTypes type;
	char name[RTFNAMEMAXLEN+1];
	signed int numarg;
/*	void *args; */
} RTFcommand;


#define MAXFONTNAME 64
/**
 * 
 * 
 */
typedef struct {
	int name;
	char fontname[MAXFONTNAME+1];
} RTFFont;

/**
 * Structure to describe style
 * 
 */
typedef struct {
	int codepage;
} RTFStyle;

/**
 * Structure to store values, local to rtf group
 * 
 */
typedef struct {
	int uc;						/**< How much symbols to skip */
	RTFStyle* style;			/**< curren style */
} RTFGroupData;

#define PARAGRAPH_BUFFER2 262144

class Analyze;
class RTFRead
{
public:
	RTFRead(Analyze* pAnalyze);
	~RTFRead(void);
public:
	void	add_to_buffer(int *bufptr,unsigned short int c);
	void	end_paragraph(int *bufptr) ;
	int		parse_rtf(FILE *f) ;
protected:
	signed long getNumber(FILE *f);

	int			getRtfCommand(FILE *f, RTFcommand *command );	
	RTFTypes	getCommandType(char *name);
	signed int	getCharCode(FILE *f);
	void		rtfSetCharset(short int **charset_ptr,unsigned int codepage);

	unsigned short int rtf_to_unicode(int code);
protected:
	short int *	current_charset;
	int			rtf_level;
	Analyze*	m_pAnalyze;

	unsigned short int buffer[PARAGRAPH_BUFFER2];
};

#endif
