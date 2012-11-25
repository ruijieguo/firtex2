#include "HTTPSearchService.h"
#include "../common/SnippetGenerator.h"
#include "firtex/search/XMLResultFormatter.h"
#include "firtex/index/IndexReader.h"
#include "firtex/index/Index.h"
#include "firtex/search/IndexSearcher.h"
#include "firtex/utility/MiscUtil.h"

using namespace std;
FX_NS_USE(utility);
FX_NS_USE(network);
FX_NS_USE(index);
FX_NS_USE(search);
FX_NS_USE(analyzer);
FX_NS_USE(queryparser);

FX_NS_DEF(app);

SETUP_STREAM_LOGGER(app, HTTPSearchService);

const std::string HTTPSearchService::URLENCODE_PRIFIX = "urlencode=";

HTTPSearchService::HTTPSearchService(const SearchResource& searchRes)
    : m_searchRes(searchRes)
{
    IndexReaderPtr pIndexReader = m_searchRes.getIndexReader();
    FIRTEX_ASSERT2(pIndexReader.isNotNull());
    m_sEncoding = pIndexReader->getEncoding();
}

std::string HTTPSearchService::requestCanHandle() const
{
    return "/search";
}

void HTTPSearchService::handleRequest(EvHttpRequestContext* pCtx)
{
    if (!m_sEncoding.empty())
    {
        std::string sDecodedStr;
        int32_t ret = MiscUtil::convertRequest(sDecodedStr, pCtx->getQuery(),
                m_sEncoding, URLENCODE_PRIFIX);
        if (ret == 1)
        {
            return doSearch(sDecodedStr, pCtx);
        }
        else if (ret == 0)
        {
            return doSearch(pCtx->getQuery(), pCtx);
        }
        else
        {
            sendErrorMessage("Invalid request", pCtx);
            return;
        }
    }
    return doSearch(pCtx->getQuery(), pCtx);
}

void HTTPSearchService::handleQuery(const Statement& state,
                                EvHttpRequestContext* pCtx) const
{
    IndexReaderPtr pIndexReader = m_searchRes.getIndexReader();
    FIRTEX_ASSERT2(pIndexReader.isNotNull());

    try
    {
        TimeProbe probe;
        probe.start();

        QueryParser parser(pIndexReader->getAnalyzerMapper(),
                           m_searchRes.getDefaultField());

        IndexSearcher searcher(pIndexReader);
        QueryHitsPtr pHits = searcher.search(state, parser);

        QueryResult result;

        if (pHits.isNotNull())
        {
            FieldSelectClausePtr pFieldClause = state.getFieldSelectClause();
            QueryClausePtr pQueryClause = state.getQueryClause();
            if (pFieldClause.isNotNull() && pQueryClause.isNotNull())
            {
                QueryPtr pQuery = parser.parse(pQueryClause->getQueryString());
                FIRTEX_ASSERT2(pQuery.isNotNull());

                FieldSelector selector(pIndexReader->getDocSchema());
                
                for (size_t i = 0; i < pFieldClause->getFieldCount(); ++i)
                {
                    const FieldSelectClause::SnippetParam& param = pFieldClause->getField(i);
                    FieldFilterPtr pFieldFilter;
                    if (param.snippet)
                    {
                        SnippetGenerator* pSnippetGen = new SnippetGenerator();
                        pFieldFilter.reset(pSnippetGen);
                        
                        if (!pSnippetGen->init(pQuery, parser.getAnalyzerMapper(), param.field,
                                        param.preTag, param.postTag, param.separator))
                        {
                            FX_LOG(ERROR, "Init snippet generator for field: [%s] FAILED", param.field.c_str());
                            sendErrorMessage("Init snippet generator for field: " + 
                                    param.field + " FAILED", pCtx);
                            return;
                        }                        
                    }

                    if (!selector.addField(param.field, pFieldFilter))
                    {
                        FX_LOG(ERROR, "Invalid field: [%s]", param.field.c_str());
                    }
                }
                result.init(selector, pIndexReader, *pHits);
            }
            else
            {
                result.init(pIndexReader, *pHits);
            }
        }

        probe.stop();
        result.setTimeCost(probe.elapsed() / 1000);
        FX_QUERY_TRACE(INFO, result.getTracer(), "search phase time [%d]",
                       (int32_t)result.getTimeCost());

        stringstream ss;
        XMLResultFormatter formatter;
        formatter.format(result, ss);
        sendResponse(ss.str(), pCtx);
    }
    catch(const FirteXException& e)
    {
        FX_LOG(ERROR, "Handle request FAILED: [%s], reason: [%s]",
               pCtx->getQuery().c_str(), e.what().c_str());
        sendErrorMessage("Handle request failed", pCtx);
    }
}

void HTTPSearchService::sendErrorMessage(const string& sErrorMsg,
                                     EvHttpRequestContext* pCtx) const
{
    stringstream ss;
    QueryResult result;
    result.setError(sErrorMsg);

    XMLResultFormatter formatter;
    formatter.format(result, ss);
    sendResponse(ss.str(), pCtx);
}

FX_NS_END
