#include "firtex/utility/NumberFormatter.h"
#include "firtex/search/XMLResultFormatter.h"

FX_NS_USE(utility);
FX_NS_USE(document);
FX_NS_USE(common);
FX_NS_USE(index);

FX_NS_DEF(search);

XMLResultFormatter::XMLResultFormatter()
{
}

XMLResultFormatter::~XMLResultFormatter()
{
}

void XMLResultFormatter::format(const QueryResult& result,
                                std::stringstream& ss)
{
    m_xmlDoc.clear();

    m_xmlDoc.addDeclarationNode(result.getEncoding());
    XMLNodeWrapperPtr pRootNode = m_xmlDoc.appendNode(
            XMLDocumentWrapper::NODE_ELEMENT, "result");
    if (result.hasError())
    {
        formatError(pRootNode, result.getErrorMsg());
    }
    else 
    {
        XMLNodeWrapperPtr resultNode = pRootNode->appendNode(
                XMLDocumentWrapper::NODE_ELEMENT, "hits");
    
        string str;
        NumberFormatter::append(str, (int32_t)result.size());
        resultNode->appendAttribute("number_hits", str);

        str.clear();
        NumberFormatter::append(str, result.getTotalHits());
        resultNode->appendAttribute("total_hits", str);

        str.clear();
        NumberFormatter::append(str, result.getTimeCost());
        resultNode->appendAttribute("cost", str);

        QueryResult::Iterator it = result.iterator();
        while (it.hasNext())
        {
            XMLNodeWrapperPtr pHitNode = resultNode->appendNode(
                    XMLDocumentWrapper::NODE_ELEMENT, "hit");

            const ResultDocPtr& pResDoc = it.next();

            if (result.hasShardId())
            {
                str.clear();
                NumberFormatter::append(str, pResDoc->getShardId());
                pHitNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                        "shardid", str);
            }

            if (result.hasDocId())
            {
                str.clear();
                NumberFormatter::append(str, pResDoc->getDocId());
                pHitNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                        "docid", str);
            }

            if (result.hasScore())
            {
                str.clear();
                NumberFormatter::append(str, pResDoc->getScore(), 2);
                pHitNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                        "score", str);
            }

            if (result.hasFields())
            {
                XMLNodeWrapperPtr pFieldsNode = pHitNode->appendNode(
                        XMLDocumentWrapper::NODE_ELEMENT, "fields");

                ResultDoc::Iterator fieldIt = pResDoc->iterator();
                while (fieldIt.hasNext())
                {
                    const ResultDoc::Field& field = fieldIt.next();

                    XMLNodeWrapperPtr pCDataNode = pFieldsNode->appendNode(
                            XMLDocumentWrapper::NODE_ELEMENT, field.first);
                    pCDataNode->appendNode(XMLDocumentWrapper::NODE_CDATA,
                            "", field.second);
                }
            }
        } // end while 

        const QueryTracerPtr& pTracer = result.getTracer();
        if (pTracer)
        {
            formatTracer(pRootNode, pTracer);
        }
    }
    m_xmlDoc.print(ss);
}

void XMLResultFormatter::formatTracer(XMLNodeWrapperPtr& pTraceRootNode,
                                      const QueryTracerPtr& pTracer)
{
    XMLNodeWrapperPtr pTraceNode = pTraceRootNode->appendNode(
            XMLDocumentWrapper::NODE_ELEMENT, "tracer");
    pTraceNode->appendAttribute("path", pTracer->getPath());
    pTraceNode->appendAttribute("level",
                                LoggingLevel::levelToStr(pTracer->getLevel()));

    const QueryTracer::TraceInfo& traceInfo = pTracer->getTraceInfo();
    for (size_t i = 0; i < traceInfo.size(); ++i)
    {
        XMLNodeWrapperPtr pCDataNode = pTraceNode->appendNode(
                XMLDocumentWrapper::NODE_ELEMENT, "message");
        pCDataNode->appendNode(XMLDocumentWrapper::NODE_CDATA,
                               "", traceInfo[i]);
    }

    const QueryTracer::TracerVector& tracers = pTracer->getChildTracers();
    for (size_t i = 0; i < tracers.size(); ++i)
    {
        const QueryTracerPtr& pChildTracer = tracers[i];
        if (pChildTracer)
        {
            formatTracer(pTraceNode, pChildTracer);
        }
    }
}

void XMLResultFormatter::formatError(XMLNodeWrapperPtr& pRootNode, const string& sErrorMsg)
{
    XMLNodeWrapperPtr pErrorNode = pRootNode->appendNode(
            XMLDocumentWrapper::NODE_ELEMENT, "error");
    pErrorNode->appendNode(XMLDocumentWrapper::NODE_CDATA, "", sErrorMsg);
}

FX_NS_END
