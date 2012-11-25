#ifndef __FX_SEARCHCLUSTERBASE_H
#define __FX_SEARCHCLUSTERBASE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "SearchServiceClient.h"
#include "BlenderServerConf.h"
#include "RequestAsyncDeliver.h"

FX_NS_DEF(app);

class SearchClusterBase : public SearchServiceClient
{
public:
    using SearchServiceClient::ErrorCode;

    typedef RequestAsyncDeliver<SearchClusterBase> AsyncDeliver;

public:
    SearchClusterBase(AsyncDeliver* pDeliver);
    virtual ~SearchClusterBase();

public:
    /// Initialize the search cluster Shard
    bool init(const BlenderServerConf::sCluster& clusterConf);

    /**
     * Send request to searcher (non-blocking)
     *
     * @param sRequest request sending to searcher
     */
    void asyncSearch(const std::string& sRequest);

    /// Return cluster name
    const std::string& getName() const;

    /// Return request
    const std::string& getRequest() const;

    /// Return response
    const std::string& getResponse() const;

    /// Return status code
    ErrorCode getStatus() const;

    /// Send request in blocking mode
    virtual void run() = 0;

protected:
    /// Cluster name
    std::string m_sClusterName;

    /// Current request
    std::string m_sRequest;

    /// Response of current request
    std::string m_sResponse;

    /// Status of current request
    ErrorCode m_status;

    /// Async request deliver
    AsyncDeliver* m_pDeliver;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(SearchClusterBase);

/////////////////////////////////////////////////////
inline const std::string& SearchClusterBase::getName() const
{
    return m_sClusterName;
}

inline const std::string& SearchClusterBase::getRequest() const
{
    return m_sRequest;
}

inline const std::string& SearchClusterBase::getResponse() const
{
    return m_sResponse;
}

inline SearchClusterBase::ErrorCode SearchClusterBase::getStatus() const
{
    return m_status;
}

FX_NS_END

#endif
