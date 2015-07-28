#include "firtex/search/XMLResultParser.h"

FX_NS_USE(utility);
FX_NS_USE(index);
FX_NS_USE(common);

FX_NS_DEF(search);

SETUP_LOGGER(search, XMLResultParser);

XMLResultParser::XMLResultParser() 
{
}

XMLResultParser::~XMLResultParser() 
{
}

#define ASSERT_EXIST(node, tag)                                         \
    if (!node)                                                  \
    {                                                                   \
        FIRTEX_THROW_AND_LOG(BadXmlFormatException,                     \
                             "Missing " tag " tag in result string.");    \
    }


void XMLResultParser::parse(const std::string& str, 
                            QueryResult& result)
{
    XMLDocumentWrapper xmlDoc;
    try 
    {
        xmlDoc.parse(str);
    }
    catch(const BadXmlFormatException& e)
    {
        FX_LOG(ERROR, "Parse result string FAILED: [%s]",
               e.what().c_str());
        throw ;
    }

    XMLNodeWrapperPtr pResNode = xmlDoc.firstNode("result");
    ASSERT_EXIST(pResNode, "<result>");

    XMLNodeWrapperPtr pErrorNode = pResNode->firstNode("error");
    if (pErrorNode)
    {
        XMLNodeWrapperPtr pDataNode = pErrorNode->firstNode();
        ASSERT_EXIST(pDataNode, "<[CDATA[]]>");
        result.setError(pDataNode->getValue());
    }

    XMLNodeWrapperPtr pHitsNode = pResNode->firstNode("hits");
    if (pHitsNode)
    {
        XMLAttributeWrapperPtr pNumHits = pHitsNode->firstAttribute("number_hits");
        ASSERT_EXIST(pNumHits, "<number_hits>");
        int32_t nNumHits = NumberParser::parseInt32(pNumHits->getValue());

        XMLAttributeWrapperPtr pTotalHits = pHitsNode->firstAttribute("total_hits");
        ASSERT_EXIST(pNumHits, "<total_hits>");
        uint64_t nTotalHits = NumberParser::parseUInt64(pTotalHits->getValue());
        result.setTotalHits(nTotalHits);

        XMLAttributeWrapperPtr pCost = pHitsNode->firstAttribute("cost");
        ASSERT_EXIST(pCost, "<cost>");
        uint64_t uTimeCost = NumberParser::parseUInt64(pCost->getValue());
        result.setTimeCost(uTimeCost);

        int32_t nHitCount = 0;
        for (XMLNodeWrapperPtr pHitNode = pHitsNode->firstNode();
             pHitNode; pHitNode = pHitNode->nextSibling())
        {
            ResultDocPtr pResDoc;
            XMLNodeWrapperPtr pDocIdNode = pHitNode->firstNode("docid");
            if (pDocIdNode)
            {
                result.hasDocId(true);
                docid_t docId = NumberParser::parseInt32(pDocIdNode->getValue());
                pResDoc.reset(new ResultDoc(docId));
            }
            else
            {
                pResDoc.reset(new ResultDoc());
            }

            XMLNodeWrapperPtr pShardIdNode = pHitNode->firstNode("shardid");
            if (pShardIdNode)
            {
                shardid_t shardId = NumberParser::parseInt32(pShardIdNode->getValue());
                result.setShardId(shardId);
            }
        
            XMLNodeWrapperPtr pScoreNode = pHitNode->firstNode("score");
            if (pScoreNode)
            {
                score_t score = NumberParser::parseFloat(pScoreNode->getValue());
                pResDoc->setScore(score);
            }
        
            XMLNodeWrapperPtr pFieldsNode = pHitNode->firstNode("fields");
            if (pFieldsNode)
            {
                for (XMLNodeWrapperPtr pFieldNode = pFieldsNode->firstNode();
                     pFieldNode; pFieldNode = pFieldNode->nextSibling())
                {
                    XMLNodeWrapperPtr pDataNode = pFieldNode->firstNode();
                    ASSERT_EXIST(pDataNode, "<[CDATA[]]>");
                    pResDoc->addField(pFieldNode->getName(), pDataNode->getValue());
                }
            }
        
            result.addDoc(pResDoc);
            ++nHitCount;
        }

        if (nNumHits != nHitCount)
        {
            FX_LOG(WARN, "<number_hits> is invalid.");
        }
    }

    XMLNodeWrapperPtr pTraceRootNode = pResNode->firstNode("tracer");
    if (pTraceRootNode)
    {
        result.getTracer().reset(parseTraceNode(pTraceRootNode));
    }
}

QueryTracer* XMLResultParser::parseTraceNode(const XMLNodeWrapperPtr& pTraceRootNode)
{
    XMLAttributeWrapperPtr pPath = pTraceRootNode->firstAttribute("path");
    ASSERT_EXIST(pPath, "<path>");
    XMLAttributeWrapperPtr pLevel = pTraceRootNode->firstAttribute("level");
    ASSERT_EXIST(pLevel, "<level>");

    QueryTracer* pTracer = new QueryTracer(pPath->getValue(),
            LoggingLevel::strToLevel(pLevel->getValue()));
    for (XMLNodeWrapperPtr pMsgNode = pTraceRootNode->firstNode("message");
         pMsgNode; pMsgNode = pMsgNode->nextSibling())
    {
        XMLNodeWrapperPtr pDataNode = pMsgNode->firstNode();
        ASSERT_EXIST(pDataNode, "<[CDATA[]]>");
        pTracer->addTrace(pDataNode->getValue());
    }

    XMLNodeWrapperPtr pChildNode = pTraceRootNode->firstNode("tracer");
    if (pChildNode)
    {
        QueryTracerPtr pTmp(parseTraceNode(pChildNode));
        pTracer->addChildTracer(pTmp);
    }

    return pTracer;
}

#undef ASSERT_EXIST

FX_NS_END

