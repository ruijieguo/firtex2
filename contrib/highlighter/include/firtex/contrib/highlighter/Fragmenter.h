#ifndef _FRAGMENTER_H
#define _FRAGMENTER_H

#include <string>
#include "firtex/common/StdHeader.h"

FX_NS_DEF(analyzer)
class Token;
FX_NS_END

/**
 * Encodes original text. The Encoder works with the Formatter to generate the output.
 *
 * @author Nicko Cadell
 */
FX_NS_DEF(highlight);

class Fragmenter
{
public:
    Fragmenter(void){}
    virtual ~Fragmenter(void){}
public:
    /**
     * Initializes the Fragmenter
     * @param szOrgText
     */
    virtual void start(const char* szOrgText, size_t length) = 0;

    /**
     * Test to see if this token from the stream should be held in a new TextFragment
     * @param nextToken
     */
    virtual bool isNewFragment(const FX_NS(analyzer)::Token* pNextToken) = 0;
};

FX_NS_END

#endif
