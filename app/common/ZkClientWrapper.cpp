#include "ZkClientWrapper.h"
#include "firtex/utility/StringTokenizer.h"

using namespace std;
FX_NS_USE(thread);
FX_NS_USE(utility);

FX_NS_DEF(common);

SETUP_LOGGER(common, ZkClientWrapper);

#define FX_CHECK_PATH(sPath)                                            \
    if(sPath.empty() || sPath[0] != '/')                                \
    {                                                                   \
        FX_LOG(ERROR, "invalid zookeeper path [%s]", sPath.c_str());    \
        return false;                                                   \
    }

ZkClientWrapper::ZkClientWrapper() 
    : m_pZkHandle(NULL)
    , m_nRecvTimeout(0)
{
}

ZkClientWrapper::~ZkClientWrapper() 
{
    close();
    unsubscribeAll();
}

bool ZkClientWrapper::connect(const std::string& sHost, int32_t nRecvTimeout)
{
    close();

    m_sHost = sHost;
    m_nRecvTimeout = nRecvTimeout;

    m_pZkHandle = zookeeper_init(m_sHost.c_str(), &ZkClientWrapper::watcherEvent,
                                 m_nRecvTimeout, 0, this, 0);
    if (NULL == m_pZkHandle) 
    {
        FX_LOG(ERROR, "zookeeper init failed. host: [%s]", m_sHost.c_str());
        return false;
    }
    return true;
}

void ZkClientWrapper::close() 
{
    if (NULL != m_pZkHandle)
    {
        FastMutex::Guard lock(m_closingMutex);
        zoo_set_context(m_pZkHandle, NULL);
        zoo_set_watcher(m_pZkHandle, NULL);
        int ret = zookeeper_close(m_pZkHandle);
        if (ZOK != ret) 
        {
            FX_LOG(ERROR, "zookeeper handle close failed.");
        }
        m_pZkHandle = NULL;
    }
}

void ZkClientWrapper::watcherEvent(zhandle_t*zk, int type, int state, 
                                   const char* path, void* context)
{
    if (ZOO_NOTWATCHING_EVENT == type) 
    {
        FX_LOG(ERROR, "ZOO_NOTWATCHING_EVENT event received: [%s]",
               path ? path : "null");
    }
    else if (context) 
    {
        ZkClientWrapper* pThis = (ZkClientWrapper*)context;
        pThis->handleEvent(type, state, path);
    }
}

void ZkClientWrapper::handleEvent(int type, int state, const char* path)
{
    if (true != m_closingMutex.tryLock()) 
    {
        FX_LOG(DEBUG, "Zookeeper is closing, ignore watcher event: [%s]",
               path ? path : "null");
        return;
    }
        
    std::vector<EventHandlerPtr> handlers;
    handlers.reserve(128);
    {
        ScopedRWLock rwlock(m_rwLock, false);
        ListenersMap::const_iterator it;
        if (ZOO_CHANGED_EVENT == type 
            || ZOO_CREATED_EVENT == type
            || ZOO_DELETED_EVENT == type)
        {
            it = m_dataListener.find(path);
            if (it != m_dataListener.end())
            {
                ListenerMapPtr pListenerMap = it->second;
                for (ListenerMap::const_iterator it2 = pListenerMap->begin();
                     it2 != pListenerMap->end(); ++it2)
                {
                    handlers.push_back(it2->second);
                }
            }
        }
        else if (ZOO_CHILD_EVENT == type)
        {
            it = m_childListener.find(path);
            if (it != m_dataListener.end())
            {
                ListenerMapPtr pListenerMap = it->second;
                for (ListenerMap::const_iterator it2 = pListenerMap->begin();
                     it2 != pListenerMap->end(); ++it2)
                {
                    handlers.push_back(it2->second);
                }
            }
        }
        else 
        {
            //ZOO_SESSION_EVENT
            FX_DEBUG("Received event: [%s](%d)", path ? path : "null", type);
            return;
        }
    }
        
    Context context(this, (state_t)getState(), (event_t)type);
    for (size_t i = 0; i < handlers.size(); ++i)
    {
        (*(handlers[i]))(string(path), context);
    }
    m_closingMutex.unlock();
}

ZkClientWrapper::watcherid_t ZkClientWrapper::subscribeChildChanges(
        const std::string& sPath, EventHandler* pChildListener,
        std::vector<std::string>& children)
{
    watcherid_t wid = INVALID_WATCHERID;
    {
        ScopedRWLock wlock(m_rwLock, true);
        wid = addHandler(sPath, m_childListener, pChildListener);
    }

    if (watchForChilds(sPath, children))
    {
        return wid;
    }
    else
    {
        removeHandler(sPath, m_childListener, wid);
        return INVALID_WATCHERID;
    }
}

bool ZkClientWrapper::unsubscribeChildChanges(const std::string& sPath,
        watcherid_t wid)
{
    ScopedRWLock wlock(m_rwLock, true);
    return removeHandler(sPath, m_childListener, wid);
}

bool ZkClientWrapper::watchForChilds(const std::string& sPath,
                                     std::vector<std::string>& children)
{
    return getChildren(sPath, true, children);
}  

bool ZkClientWrapper::getChildren(const std::string& sPath, bool bWatch,
                                  std::vector<std::string>& children) 
{
    FIRTEX_ASSERT2(m_pZkHandle);
    FX_CHECK_PATH(sPath);

    struct String_vector strings;
    int ret = zoo_get_children(m_pZkHandle, sPath.c_str(),
                               bWatch ? 1 : 0, &strings);
    if (ZOK == ret) 
    {
        for(int i = 0 ; i < strings.count ; ++i) 
        {
            children.push_back(std::string(strings.data[i]));
        }            
        deallocate_String_vector(&strings);
        return true;
    }
    else 
    {
        FX_LOG(ERROR, "zookeeper getchild FAILED: [%s](%d)",
               zerror(ret), ret);
        return false;
    }

    return false;
}

ZkClientWrapper::watcherid_t ZkClientWrapper::addHandler(
        const std::string& sPath, ListenersMap& map, 
        EventHandler* pHandler)
{
    watcherid_t wid = m_watchCounter;
    ++m_watchCounter;
    ListenersMap::iterator it = map.find(sPath);
    if (it != map.end())
    {
        it->second->insert(make_pair(wid, pHandler));
    }
    else
    {
        ListenerMapPtr pHandlers = new ListenerMap();
        pHandlers->insert(make_pair(wid, pHandler));
        map.insert(make_pair(sPath, pHandlers));
    }
    return wid;
}

bool ZkClientWrapper::removeHandler(const std::string& sPath, 
                                    ListenersMap& map, 
                                    watcherid_t wid)
{
    ListenersMap::iterator it = map.find(sPath);
    if (it == map.end())
    {
        return false;
    }
    
    ListenerMap::iterator it2 = it->second->find(wid);
    if (it2 == it->second->end())
    {
        return false;
    }
    it->second->erase(it2);

    return true;
}

ZkClientWrapper::watcherid_t ZkClientWrapper::subscribeDataChanges(
        const std::string& sPath, EventHandler* pDataListener)
{
    watcherid_t wid = INVALID_WATCHERID;
    {
        ScopedRWLock wlock(m_rwLock, true);
        wid = addHandler(sPath, m_dataListener, pDataListener);
    }
    if (watchForData(sPath))
    {
        return wid;
    }
    else
    {
        removeHandler(sPath, m_dataListener, wid);
        return INVALID_WATCHERID;
    }
}

bool ZkClientWrapper::unsubscribeDataChanges(const std::string& sPath, 
        watcherid_t wid)
{
    ScopedRWLock wlock(m_rwLock, true);
    return removeHandler(sPath, m_dataListener, wid);
}

void ZkClientWrapper::unsubscribeAll()
{
    ScopedRWLock wlock(m_rwLock, true);
    m_childListener.clear();
    m_dataListener.clear();
}

bool ZkClientWrapper::watchForData(const std::string& sPath)
{
    FIRTEX_ASSERT2(m_pZkHandle);
    FX_CHECK_PATH(sPath);

    struct Stat stat;
    int ret = zoo_exists(m_pZkHandle, sPath.c_str(), 1, &stat);
    if (ZOK == ret) 
    {
        return true;
    }
    else if (ZNONODE == ret) 
    {
        return true;
    }

    FX_LOG(ERROR, "zookeeper check exist failed. %s(%d)", zerror(ret), ret);        
    return false;
}

bool ZkClientWrapper::createPersistent(const std::string& sPath, 
                                       const std::string& sData)
{
    return createNode(sPath, sData, true);
}

bool ZkClientWrapper::createPersistentSequential(const std::string& sPath,
        const std::string& sData, std::string& sResultPath)
{
    FIRTEX_ASSERT2(m_pZkHandle);
    FX_CHECK_PATH(sPath);
    
    if (createNodeSequential(sPath, sData, true, sResultPath)) 
    {
        return true;
    }
    else 
    {
        return false;
    }    
}

bool ZkClientWrapper::createNodeSequential(const std::string& sPath,
        const std::string& sData, bool bPermanent, std::string &sResultPath)
{
    const static size_t SEQ_ID_LENGTH = 238;
    std::vector<char> resultBuff(sPath.length() + SEQ_ID_LENGTH);
    int ret = zoo_create(m_pZkHandle, sPath.c_str(), sData.c_str(), sData.length(), 
                         &ZOO_OPEN_ACL_UNSAFE, bPermanent ? ZOO_SEQUENCE 
                         : (ZOO_SEQUENCE|ZOO_EPHEMERAL), 
                         &*resultBuff.begin(), resultBuff.size());
    if (ZOK == ret) 
    {
        sResultPath = &*resultBuff.begin();
        return true;
    }
    else if (ZNONODE == ret) 
    {
        if (!createParentPath(sPath)) 
        {
            return false;
        }

        int ret = zoo_create(m_pZkHandle, sPath.c_str(), sData.c_str(), sData.length(), 
                             &ZOO_OPEN_ACL_UNSAFE, bPermanent ? ZOO_SEQUENCE 
                             : (ZOO_SEQUENCE|ZOO_EPHEMERAL), 
                             &*resultBuff.begin(), resultBuff.size());
        if (ZOK == ret) 
        {
            sResultPath = &*resultBuff.begin();
            return true;
        }
        else 
        {
            FX_LOG(ERROR,"zookeeper create node [%s] FAILED: [%s](%d).",
                   sPath.c_str(), zerror(ret), ret);
            return false;
        }
    }
    else 
    {
        FX_LOG(ERROR,"zookeeper create node [%s] FAILED: [%s](%d).",
               sPath.c_str(), zerror(ret), ret);
        return false;
    }
}

bool ZkClientWrapper::createNode(const std::string& sPath,
                                 const std::string &sData,
                                 bool bPermanent) 
{
    int ret = zoo_create(m_pZkHandle, sPath.c_str(), sData.c_str(), sData.length(), 
                         &ZOO_OPEN_ACL_UNSAFE, bPermanent ? 0 : ZOO_EPHEMERAL, NULL, 0);
    if (ZOK == ret) 
    {
        return true;
    }
    else if (ZNONODE == ret) 
    {
        if (!createParentPath(sPath)) 
        {
            return false;
        }

        int ret = zoo_create(m_pZkHandle, sPath.c_str(), sData.c_str(), sData.length(), 
                             &ZOO_OPEN_ACL_UNSAFE, bPermanent ? 0 : ZOO_EPHEMERAL, NULL, 0);
        if (ZOK == ret) 
        {
            return true;
        }
        else 
        {
            FX_LOG(ERROR,"zookeeper create Node %s failed: [%s](%d).",
                   sPath.c_str(), zerror(ret), ret);
            return false;
        }
    }
    else
    {
        FX_LOG(ERROR,"zookeeper create Node %s failed: [%s](%d).",
               sPath.c_str(), zerror(ret), ret);
        return false;
    }
}

bool ZkClientWrapper::createParentPath(const std::string& sPath) 
{
    StringTokenizer st(sPath, "/");
    std::string current;
    for (size_t i = 0; i < st.getNumTokens() - 1; ++i)
    {
        current += std::string("/") + st[i];
        bool succ = false;
        StringTokenizer st2(m_sHost, ",", StringTokenizer::TOKEN_TRIM
                            | StringTokenizer::TOKEN_IGNORE_EMPTY);
        size_t nTryCount = st2.getNumTokens();
        for (size_t tryCount = 0; tryCount < nTryCount; ++tryCount) 
        {
            int ret = zoo_create(m_pZkHandle, current.c_str(), "" , 0,
                    &ZOO_OPEN_ACL_UNSAFE, 0, NULL, 0);
            if (ret == ZOK || ret == ZNODEEXISTS) 
            {
                succ = true;
                break;
            }

            if (ret == ZCONNECTIONLOSS || ret == ZOPERATIONTIMEOUT) 
            {
                FX_LOG(DEBUG, "zookeeper create node [%s] retry for [%s](%d).", 
                        current.c_str(), zerror(ret), ret);
                continue;
            }

            FX_LOG(ERROR,"zookeeper create node %s failed: [%s](%d).",
                   current.c_str(), zerror(ret), ret);
            return false;
        }
        if (!succ) 
        {
            return false;
        }
    }

    return true;
}

size_t ZkClientWrapper::countChildren(const std::string& sPath)
{
    vector<string> children;
    if (getChildren(sPath, false, children))
    {
        return children.size();
    }
    return 0;
}

bool ZkClientWrapper::exists(const std::string& sPath,
                             bool bWatch, bool& bExist)
{
    bExist = false;

    FIRTEX_ASSERT2(m_pZkHandle);
    FX_CHECK_PATH(sPath);

    if (sPath.empty() || sPath[0] != '/') 
    {
        FX_LOG(ERROR, "invalid zookeeper path [%s]", sPath.c_str());
        return false;
    }

    struct Stat stat;
    int ret = zoo_exists(m_pZkHandle, sPath.c_str(), bWatch ? 1 : 0, &stat);
    if (ZOK == ret) 
    {
        bExist = true;
        return true;
    }
    else if (ZNONODE == ret) 
    {
        bExist = false;
        return true;
    }

    FX_LOG(ERROR, "zookeeper check exist failed. [%s](%d)",
           zerror(ret), ret);
    return false;
}

bool ZkClientWrapper::hasListeners(const std::string& sPath)
{
    ScopedRWLock rwlock(m_rwLock, false);

    return hasListeners(sPath, m_childListener) 
        || hasListeners(sPath, m_dataListener);
}

bool ZkClientWrapper::hasListeners(const std::string& sPath,
                                   const ListenersMap& map)
{
    ListenersMap::const_iterator it = map.find(sPath);
    return (it != map.end() && it->second.isNotNull() && it->second->size() > 0);
}

bool ZkClientWrapper::remove(const std::string& sPath) 
{
    FIRTEX_ASSERT2(m_pZkHandle);
    FX_CHECK_PATH(sPath);

    int ret = zoo_delete(m_pZkHandle, sPath.c_str(), -1);
    if ((ZOK == ret) || (ZNONODE == ret)) 
    {
        return true;
    }
    else if (ZNOTEMPTY == ret) 
    {
        struct String_vector strings;
        ret = zoo_get_children(m_pZkHandle, sPath.c_str(), 0, &strings);
        if (ZOK == ret) 
        {
            for(int i = 0 ; i < strings.count ; ++i) 
            {
                remove(sPath + "/" + strings.data[i]);
            }            
            deallocate_String_vector(&strings);
        }
        ret = zoo_delete(m_pZkHandle, sPath.c_str(), -1);
        if ((ZOK == ret) || (ZNONODE == ret)) 
        {
            return true;
        }
    }

    FX_LOG(ERROR, "zookeeper delete failed. [%s](%d)", zerror(ret), ret);        
    return false;
}

bool ZkClientWrapper::readData(const std::string& sPath,
                               std::string& sData, bool bWatch)
{
    FIRTEX_ASSERT2(m_pZkHandle);
    FX_CHECK_PATH(sPath);

    char buffer[1024];
    struct Stat stat;

    int buffer_len = sizeof(buffer);
    int ret = zoo_get(m_pZkHandle, sPath.c_str(), bWatch ? 1 : 0, 
                      buffer, &buffer_len, &stat);
    if (ZOK != ret) 
    {
        if (ZNONODE != ret) 
        {
            FX_LOG(ERROR, "zookeeper get failed. %s(%d)", zerror(ret), ret);        
        }
        return false;
    }    
        
    if ((unsigned)stat.dataLength > sizeof(buffer)) 
    {
        char *newBuffer = new char[stat.dataLength];
        buffer_len = stat.dataLength;
        int ret = zoo_get(m_pZkHandle, sPath.c_str(), bWatch ? 1 : 0,
                          newBuffer, &buffer_len, &stat);
        if (ZOK != ret) 
        {
            delete [] newBuffer;
            if (ZNONODE != ret)
            {
                FX_LOG(ERROR, "zookeeper get data FAILED: [%s](%d)", 
                       zerror(ret), ret);
            }
            return false;
        }
        sData = std::string(newBuffer, (size_t)buffer_len);
        delete[] newBuffer;
    }
    else 
    {
        sData = std::string(buffer, (size_t)buffer_len);
    }
    return true;
}

bool ZkClientWrapper::writeData(const std::string& sPath, 
                                const std::string& sData)
{
    FIRTEX_ASSERT2(m_pZkHandle);
    FX_CHECK_PATH(sPath);
    
    int ret = zoo_set(m_pZkHandle, sPath.c_str(), 
                      sData.c_str(), sData.length(), -1);
    if (ZOK == ret) 
    {
        return true;
    }
    FX_LOG(ERROR, "zookeeper get data FAILED: [%s](%d)", 
           zerror(ret), ret);

    return false;
}

#undef FX_CHECK_PATH

FX_NS_END

