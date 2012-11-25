#ifndef __FX_SEARCH_HANDLER_H
#define __FX_SEARCH_HANDLER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"

#ifdef WIN32
#include <ctype.h>
#include <winsock2.h>
#include <windows.h>
#include <iphlpapi.h>
#include <io.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include "firtex/utility/IconvWrapper.h"
#include "firtex/search/Statement.h"
#include "SearchResource.h"
#include "../thrift/gen-cpp/ThriftSearchService.h"

FX_NS_DEF(app);

class RPCSearchService : public ThriftSearchServiceIf
{
public:
    const static std::string URLENCODE_PRIFIX;

public:
    RPCSearchService(const SearchResource& searchRes)
        : m_searchRes(searchRes) 
    {
        FX_NS(index)::IndexReaderPtr pIndexReader = m_searchRes.getIndexReader();
        FIRTEX_ASSERT2(pIndexReader.isNotNull());
        m_sEncoding = pIndexReader->getEncoding();
    }

public:
    virtual void search(std::string& sResult, const QueryCommand& cmd);
    virtual void syntaxSearch(std::string& sResult, const std::string& sUri);
    virtual bool ping();

private:
    inline void doSearch(std::string& sResult, const std::string& sUri);
    void handleQuery(std::string& sResult, 
                     const FX_NS(search)::Statement& state) const;
    void formatErrorMessage(std::string& sMsgResult, 
                            const std::string& sErrorMsg) const;
private:
    const SearchResource& m_searchRes;
    std::string m_sEncoding;

private:
    DECLARE_LOGGER();
};

////////////////////////////////////////////////////////
///
inline void RPCSearchService::doSearch(std::string& sResult, const std::string& sUri)
{
    FX_NS(search)::Statement state;
    bool bValid = state.fromString(sUri);
    if (!bValid)
    {
        sResult = state.getErrorMessage();
    }    
    else
    {
        handleQuery(sResult, state);
    }
}

FX_NS_END


#endif //__FX_SEARCH_HANDLER_H
