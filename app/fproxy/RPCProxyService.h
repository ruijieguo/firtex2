#ifndef __FX_PRCPROXYSERVICE_H
#define __FX_PRCPROXYSERVICE_H

#include "ProxyServerConf.h"
#include "firtex/queryparser/QueryParser.h"
#include "firtex/utility/IconvWrapper.h"
#include "firtex/utility/URLDecoder.h"
#include "RPCSearchShards.h"
#include "ProxyServiceBase.h"
#include "../thrift/gen-cpp/ThriftSearchService.h"

FX_NS_DEF(app);

class RPCProxyService : public ThriftSearchServiceIf, public ProxyServiceBase
{
public:
    RPCProxyService();
    ~RPCProxyService();

public:
    virtual void syntaxSearch(std::string& sResult, const std::string& sUri);
    virtual void search(std::string& sResult, const QueryCommand& cmd);
    virtual bool ping();

protected:
    /// Create search shards implement
    virtual SearchShardsBase* newShards(int32_t nTimeout);
};

DEFINE_TYPED_PTR(RPCProxyService);

//////////////////////////////////////////
//

FX_NS_END

#endif //__FX_PRCPROXYSERVICE_H
