#ifndef __FX_SEARCHSERVICECLIENT_H
#define __FX_SEARCHSERVICECLIENT_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "BlenderServerConf.h"

FX_NS_DEF(app);

class SearchServiceClient
{
public:
    enum ErrorCode
    {
        EC_OK,
        EC_TIMEOUT,
        EC_ERROR
    };

public:
    SearchServiceClient();
    virtual ~SearchServiceClient();

public:
    /// Initialize the search cluster Shard
    bool init(const std::string& sHostAndPort);

public:
    /**
     * Initialize client, implemented by derived class
     *
     * @param sAddr host address
     * @param nPort host port
     */
    virtual void initClient(const std::string& sAddr, int32_t nPort) = 0;

public:
    static void formatError(std::string& sResult,
                            const std::string& sErrorMsg);

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(SearchServiceClient);

FX_NS_END

#endif
