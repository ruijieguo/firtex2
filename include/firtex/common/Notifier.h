#ifndef __FX_NOTIFIER_H
#define __FX_NOTIFIER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Notifiable.h"
#include "firtex/common/Logger.h"
#include "firtex/thread/FastMutex.h"
#include <list>
#include <map>

FX_NS_DEF(common)

class Notifier : public Notifiable
{
public:
    Notifier();
    virtual ~Notifier();

public:
    /**
     * Register notifiables which will receive notifications from this notifier
     *
     * @param sIdentifier identifier of the notification which related to the notifiable 
     * @param notifiable who care about the notification
     */
    void registerClient(const tstring& sIdentifier, Notifiable* notifiable);
    void unregisterAll();

    void update(const Notification& notif);

private:
    typedef std::list<Notifiable*> NotifyList;
    typedef std::map<tstring, NotifyList> NotifyTable;

    FX_NS(thread)::FastMutex m_tableLock;
    NotifyTable m_notifyTable;

private:
    DECLARE_STREAM_LOGGER();    
};

FX_NS_END

#endif
