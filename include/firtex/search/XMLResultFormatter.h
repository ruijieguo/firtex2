#ifndef __FX_XML_RESULT_FORMATTER_H
#define __FX_XML_RESULT_FORMATTER_H

#include "firtex/utility/XMLDocumentWrapper.h"
#include "firtex/search/ResultFormatter.h"
#include "firtex/search/QueryTracer.h"

FX_NS_DEF(search);

class XMLResultFormatter : public ResultFormatter
{
public:
    XMLResultFormatter();
    ~XMLResultFormatter();

public:
    /**
     * Format query Result
     */
    void format(const FX_NS(search)::QueryResult& result,
                std::stringstream& ss);

public:
    static void formatTracer(FX_NS(utility)::XMLNodeWrapperPtr& pTraceRootNode,
                             const QueryTracerPtr& pTracer);

protected:
    void formatError(FX_NS(utility)::XMLNodeWrapperPtr& pRootNode,
                     const std::string& sErrorMsg);

protected:
    FX_NS(utility)::XMLDocumentWrapper m_xmlDoc;
};

FX_NS_END

#endif //__FX_XML_RESULT_FORMATTER_H
