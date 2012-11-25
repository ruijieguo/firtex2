#ifndef __MSOFFICETEXTSTRIPPTER_H
#define __MSOFFICETEXTSTRIPPTER_H

#include "firtex/common/StdHeader.h"
#include "Writer.h"
#include "Analyze.h"

FX_NS_DEF(plugin)

class MSOfficeTextStripper : public Writer
{
public:
    MSOfficeTextStripper(void);
    ~MSOfficeTextStripper(void);
public:	
    char* strip(const char* szFileName);

    char* getText(){return m_pTextBuffer;}
    size_t getLength(){return m_nTextPos;}

protected:
    virtual void output_paragraph(unsigned short int *buffer);

    void growBuffer(size_t nGrowLen);
protected:
    char* m_pTextBuffer;
    size_t m_nTextPos;
    size_t m_nTextSize;

    Analyze* m_pAnalyze;

    const static int DEFAULT_TEXTBUF_SIZE = 1024*512;//512kb
};

FX_NS_END

#endif
