#include "RPCSearchService.h"
#include "../common/SnippetGenerator.h"
#include "firtex/search/XMLResultFormatter.h"
#include "firtex/index/IndexReader.h"
#include "firtex/index/Index.h"
#include "firtex/search/IndexSearcher.h"
#include "../thrift/ThriftSearchServiceHelper.h"
#include "firtex/queryparser/QueryParser.h"
#include "firtex/utility/MiscUtil.h"

using namespace std;
FX_NS_USE(utility);
FX_NS_USE(index);
FX_NS_USE(search);
FX_NS_USE(analyzer);
FX_NS_USE(queryparser);

FX_NS_DEF(app);

SETUP_LOGGER(app, RPCSearchService);

const std::string RPCSearchService::URLENCODE_PRIFIX = "urlencode=";

void RPCSearchService::search(string& sResult, const QueryCommand& cmd)
{
    string sUri;
    sUri.reserve(256);
    ThriftSearchServiceHelper::queryCommandToStatmentString(sUri, cmd);

    syntaxSearch(sResult, sUri);
}

void RPCSearchService::syntaxSearch(string& sResult, const string& sUri)
{
    if (!m_sEncoding.empty())
    {
        std::string sDecodedStr;
        sDecodedStr.reserve(sUri.length() * 2);
        int32_t ret = MiscUtil::convertRequest(sDecodedStr, sUri,
                m_sEncoding, URLENCODE_PRIFIX);
        if (ret == 1)
        {
            return doSearch(sResult, sDecodedStr);
        }
        else if (ret == 0)
        {
            return doSearch(sResult, sUri);
        }
        else
        {
            formatErrorMessage(sResult, "Invalid request.");
            return;
        }
    }
    return doSearch(sResult, sUri);
}

bool RPCSearchService::ping()
{
    return true;
}


void RPCSearchService::handleQuery(std::string& sResult, const Statement& state) const
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
                    const FieldSelectClause::SnippetParam& param = 
                        pFieldClause->getField(i);
                    FieldFilterPtr pFieldFilter;
                    if (param.snippet)
                    {
                        SnippetGenerator* pSnippetGen = new SnippetGenerator();
                        pFieldFilter.reset(pSnippetGen);
                        
                        if (!pSnippetGen->init(pQuery, parser.getAnalyzerMapper(), 
                                        param.field, param.preTag, param.postTag,
                                        param.separator))
                        {
                            FX_LOG(ERROR, "Init snippet generator for field: [%s] FAILED", param.field.c_str());
                            formatErrorMessage(sResult, "Init snippet generator for field: "
                                    + param.field + " FAILED");
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

        FX_QUERY_TRACE(INFO, result.getTracer(), "search phase time [%d] ms",
                       (int32_t)result.getTimeCost());

        std::stringstream ss;
        XMLResultFormatter formatter;
        formatter.format(result, ss);
        sResult = ss.str();
    }
    catch(const FirteXException& e)
    {
        stringstream ss;
        state.toString(ss);
        FX_LOG(ERROR, "Handle request FAILED: [%s], reason: [%s]",
               ss.str().c_str(), e.what().c_str());
        formatErrorMessage(sResult, "Handle request failed");
    }
}

void RPCSearchService::formatErrorMessage(string& sMsgResult, 
        const string& sErrorMsg) const
{
    stringstream ss;
    QueryResult result;
    result.setError(sErrorMsg);

    XMLResultFormatter formatter;
    formatter.format(result, ss);
    sMsgResult = ss.str();
}


FX_NS_END
