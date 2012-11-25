#include "firtex/common/Notifier.h"

using namespace std;
FX_NS_USE(thread);

FX_NS_DEF(common);

SETUP_STREAM_LOGGER(common, Notifier);

Notifier::Notifier()
{
}

Notifier::~Notifier()
{
}

void Notifier::registerClient(const tstring& sIdentifier, Notifiable* pNotifiable)
{
    FastMutex::Guard lock(m_tableLock);
    NotifyTable::iterator it = m_notifyTable.find(sIdentifier);
    if (it == m_notifyTable.end())
    {
        NotifyList noList;
        noList.push_back(pNotifiable);
        m_notifyTable.insert(make_pair(sIdentifier, noList));
    }
    else 
    {
        NotifyList& noList = it->second;
        noList.push_back(pNotifiable);
    }
}

void Notifier::unregisterAll()
{
    FastMutex::Guard lock(m_tableLock);
    m_notifyTable.clear();
}

void Notifier::update(const Notification& notif)
{
    FastMutex::Guard lock(m_tableLock);

    NotifyTable::iterator it = m_notifyTable.find(notif.getIdentifier());
    if (it != m_notifyTable.end())
    {
        NotifyList& noList = it->second;
        for (NotifyList::iterator noIt = noList.begin();
             noIt != noList.end(); ++noIt)
        {
            (*noIt)->update(notif);
        }
    }
}

FX_NS_END
