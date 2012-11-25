#ifndef _SIMPLEHTMLFORMATTER_H
#define _SIMPLEHTMLFORMATTER_H

#include <string>
#include "Formatter.h"
#include "firtex/utility/String.h"

FX_NS_DEF(highlight)

class SimpleHTMLFormatter :	public Formatter
{
public:
    SimpleHTMLFormatter(const std::string& szPreTag, 
                        const std::string& szPostTag);
    SimpleHTMLFormatter();
    virtual ~SimpleHTMLFormatter(void);

public:
    /**
     * @param szMarkedText marked text
     * @param szOriginalText The section of text being considered for markup
     * @param nStartPos start position in original text
     * @param nEndPos end position in original text
     */
    void highlightTerm(string& sMarkedText,
                       const char* szOriginalText, 
                       int32_t nStartPos, int32_t nEndPos);

    /**
     * set pre and post tags
     * @param sPreTag pre tag of highlight term
     * @param sPostTag post tag of highlight term
     */
    void setTags(const std::string& sPreTag, const std::string& sPostTag);

protected:
    std::string	m_sPreTag;
    std::string	m_sPostTag;
};

FX_NS_END

#endif
