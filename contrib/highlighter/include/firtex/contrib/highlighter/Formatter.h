#ifndef _FORMATTER_H
#define _FORMATTER_H

#include <string>
#include "firtex/common/StdHeader.h"
#include "TokenGroup.h"

FX_NS_DEF(highlight)

/**
 * Processes terms found in the original text, typically by applying some form 
 * of mark-up to highlight terms in HTML search results pages.
 */
class Formatter
{
public:
    Formatter(void){}
    virtual ~Formatter(void){}

public:
    /**
     * @param originalText The section of text being considered for markup
     * @param tokenGroup contains one or several overlapping TokenView along with
     * their scores and positions.
     */
    virtual void highlightTerm(std::string& sMarkedText,
                               const char* szOriginalText, 
                               int32_t nStartPos, int32_t nEndPos) = 0;
};

FX_NS_END

#endif
