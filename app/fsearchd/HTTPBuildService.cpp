#include "HTTPBuildService.h"
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

FX_NS_DEF(app);

SETUP_STREAM_LOGGER(app, HTTPBuildService);

HTTPBuildService::HTTPBuildService(const SearchResource& searchRes)
    : m_searchRes(searchRes)
{
    IndexReaderPtr pIndexReader = m_searchRes.getIndexReader();
    FIRTEX_ASSERT2(pIndexReader);
    m_sEncoding = pIndexReader->getEncoding();
}

std::string HTTPBuildService::requestCanHandle() const
{
    return "/push";
}

void HTTPBuildService::handleRequest(EvHttpRequestContext* pCtx)
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

void HTTPBuildService::sendErrorMessage(const string& sErrorMsg,
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
