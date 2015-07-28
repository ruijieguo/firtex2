#include "AppendResultCombiner.h"
#include "firtex/search/QueryResult.h"
#include "firtex/search/XMLResultParser.h"
#include "firtex/search/XMLResultFormatter.h"
#include "firtex/utility/NumberFormatter.h"
#include <sstream>

using namespace std;
FX_NS_USE(utility);
FX_NS_USE(search);
FX_NS_USE(index);
FX_NS_USE(common);

FX_NS_DEF(app);

SETUP_STREAM_LOGGER(app, AppendResultCombiner);

void AppendResultCombiner::beginCombine()
{
    m_pXmlDoc.reset(new XMLDocumentWrapper());
    m_pRootNode = m_pXmlDoc->appendNode(XMLDocumentWrapper::NODE_ELEMENT, "result");
}

void AppendResultCombiner::combine(const std::string& sClusterId, 
                                   const std::string& sResult)
{
    FX_DEBUG("Combine result for cluster: [%s].", sClusterId.c_str());
    FX_TRACE("Result of cluster: [%s].", sResult.c_str());

    QueryResult queryResult;
    try
    {
        XMLResultParser parser;
        parser.parse(sResult, queryResult);
    }   
    catch(const BadXmlFormatException& e)
    {
        stringstream ss;
        ss << "Receive invalid result document: [" << e.what() << "]";
        FX_LOG(ERROR, "Invalid xml document: [%s]", ss.str().c_str());
        
        appendErrorResult(sClusterId, ss.str());
        return;
    } 

    if (queryResult.hasError())
    {
        appendErrorResult(sClusterId, queryResult.getErrorMsg());
    }
    else
    {
        XMLNodeWrapperPtr pHitsNode = m_pRootNode->appendNode(
                XMLDocumentWrapper::NODE_ELEMENT, "hits");
        string str;
        NumberFormatter::append(str, (int32_t)queryResult.size());
        pHitsNode->appendAttribute("number_hits", str);

        str.clear();
        NumberFormatter::append(str, queryResult.getTotalHits());
        pHitsNode->appendAttribute("total_hits", str);

        str.clear();
        NumberFormatter::append(str, queryResult.getTimeCost());
        pHitsNode->appendAttribute("cost", str);

        pHitsNode->appendAttribute("cluster", sClusterId);

        QueryResult::Iterator it = queryResult.iterator();
        while (it.hasNext())
        {
            XMLNodeWrapperPtr pHitNode = pHitsNode->appendNode(
                    XMLDocumentWrapper::NODE_ELEMENT, "hit");

            const ResultDocPtr& pResDoc = it.next();

            if (queryResult.hasShardId())
            {
                str.clear();
                NumberFormatter::append(str, pResDoc->getShardId());
                pHitNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                        "shardid", str);
            }

            if (queryResult.hasDocId())
            {
                str.clear();
                NumberFormatter::append(str, pResDoc->getDocId());
                pHitNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                        "docid", str);
            }

            if (queryResult.hasScore())
            {
                str.clear();
                NumberFormatter::append(str, pResDoc->getScore(), 2);
                pHitNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                        "score", str);
            }

            if (queryResult.hasFields())
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
        }//end while

        const QueryTracerPtr& pTracer = queryResult.getTracer();
        if (pTracer)
        {
            XMLNodeWrapperPtr pTraceNode = m_pRootNode->firstNode("tracer");
            if (!pTraceNode)
            {
                pTraceNode = m_pRootNode->appendNode(
                        XMLDocumentWrapper::NODE_ELEMENT, "tracer");
                pTraceNode->appendAttribute("path", "blender");
                pTraceNode->appendAttribute("level",
                        LoggingLevel::levelToStr(pTracer->getLevel()));

            }
            XMLResultFormatter::formatTracer(pTraceNode, pTracer);
        }
    }
}

void AppendResultCombiner::appendErrorResult(
        const string& sClusterId, const string& sErrorMsg)
{
    XMLNodeWrapperPtr pErrorNode = m_pRootNode->appendNode(
            XMLDocumentWrapper::NODE_ELEMENT, "error");
    pErrorNode->appendNode(XMLDocumentWrapper::NODE_CDATA,
                           "", sErrorMsg);
    pErrorNode->appendAttribute("cluster", sClusterId);
}

void AppendResultCombiner::endCombine(std::string& sResult)
{
    m_pXmlDoc->print(sResult);
    m_pXmlDoc->clear();
    m_pXmlDoc.reset();
    m_pRootNode.reset();
}

FX_NS_END
