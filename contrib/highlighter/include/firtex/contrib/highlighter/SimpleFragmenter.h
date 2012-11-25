#ifndef _SIMPLEFRAGMENTER_H
#define _SIMPLEFRAGMENTER_H

#include "Fragmenter.h"
#include "firtex/common/StdHeader.h"

FX_NS_DEF(highlight)

class SimpleFragmenter : public Fragmenter
{
public:
    SimpleFragmenter(void);
    SimpleFragmenter(size_t nFragmentSize);
    virtual ~SimpleFragmenter(void);
public:
    /**
     * Initializes the Fragmenter
     * @param sOrgText
     */
    void start(const char* szOrgText, size_t length);

    /**
     * Test to see if this token from the stream should be held in a new TextFragment
     * @param nextToken
     */
    bool isNewFragment(const FX_NS(analyzer)::Token* pNextToken);

    /**
     * @return size in bytes of each fragment
     */
    size_t getFragmentSize(){return m_nFragmentSize;}

    /**
     * @param size size in bytes of each fragment
     */
    void setFragmentSize(int32_t size){m_nFragmentSize = size;}
protected:
    const static int DEFAULT_FRAGMENT_SIZE = 100;

    int32_t m_nCurrentNumFrags;
    size_t m_nFragmentSize;
};

FX_NS_END

#endif
