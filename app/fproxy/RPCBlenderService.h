#ifndef __FX_RPCBLENDERSERVICE_H
#define __FX_RPCBLENDERSERVICE_H

#include "RPCSearchClusters.h"
#include "BlenderServerConf.h"
#include "BlenderServiceBase.h"
#include "../thrift/gen-cpp/ThriftSearchService.h"

FX_NS_DEF(app);

class RPCBlenderService : public ThriftSearchServiceIf, public BlenderServiceBase
{
public:
    RPCBlenderService();
    ~RPCBlenderService();

public:
    /**
     * Handle a search request. This method must be thread-safe
     * @param sResult search result
     * @param sUri search request
     */
    virtual void syntaxSearch(std::string& sResult, const std::string& sUri);

    /**
     * Handle a search request. This method must be thread-safe
     * @param sResult search result
     * @param cmd search request command
     */
    virtual void search(std::string& sResult, const QueryCommand& cmd);

    /// Implement ping service
    virtual bool ping();

protected:
    /// Create search clusters implement
    virtual SearchClustersBase* newClusters(int32_t nTimeout);
};

DEFINE_TYPED_PTR(RPCBlenderService);

//////////////////////////////////////////
//

FX_NS_END

#endif //__FX_RPCBLENDERSERVICE_H
