#include "HTTPSearchService.h"
#include "SearchExecutor.h"
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
}


std::string HTTPSearchService::requestCanHandle() const
{
    return "GET ^/([\\w,]{1,})(/[\\w,]{1,}){0,1}/_search$";
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

void HTTPSearchService::doSearch(const string& sUri,
                                 EvHttpRequestContext* pCtx) const
{
    const EvHttpRequestContext::Resources& res = pCtx->getResources();
    if (res.size() == 0)
    {
        sendErrorMessage("Invalid request", pCtx);
        return;
    }

    Statement state;
    bool bValid = state.fromString(pCtx->getQuery());
    if (!bValid)
    {
        sendErrorMessage(state.getErrorMessage(), pCtx);
        return;
    }    
    
    string sCluster = res[0];
    string sType;
    if (res.size() >= 3)
    {
        sType = res[1];
    }

    IndexReaderPtr pIndexReader = m_searchRes.getIndexReader(sCluster, sType);
    if (pIndexReader.isNull())
    {
        sendErrorMessage("No such cluster [" + sCluster + "] or type ["
                         +sType + "]", pCtx);
        return;
    }

    IndexSearcher searcher(pIndexReader);
    XMLResultFormatter formatter;
    stringstream ss;

    SearchExecutor executor(searcher, formatter);
    try
    {
        executor.search(ss, sUri);
        sendResponse(ss.str(), pCtx);
    }
    catch(const FirteXException& e)
    {
        FX_LOG(ERROR, "Handle request FAILED: [%s], reason: [%s]",
               pCtx->getQuery().c_str(), e.what().c_str());
        sendErrorMessage("Handle request FAILED: " + e.what(), pCtx);
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
