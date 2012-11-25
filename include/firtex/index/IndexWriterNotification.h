#ifndef __INDEXWRITERNOTIFICATION_H
#define __INDEXWRITERNOTIFICATION_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Notification.h"

FX_NS_DEF(index);

class IndexWriter;
class IndexWriterNotification : public FX_NS(common)::Notification
{
public:
    enum NotifyType
    {
        NONE = 0,
        BARREL_CREATED = 1,
        BARREL_BEGIN_COMMIT = 2,
        BARREL_END_COMMIT = 3,
    };

public:
    IndexWriterNotification() 
        : m_notifyType(NONE)
        , m_pIndexWriter(NULL) 
    {
    }

    IndexWriterNotification(NotifyType nt, IndexWriter* pIndexWriter)
        : m_notifyType(nt), m_pIndexWriter(pIndexWriter) 
    {
    }

    virtual ~IndexWriterNotification(){}

public:
    NotifyType getType() const {return m_notifyType;}
    IndexWriter* getIndexWriter() const { return m_pIndexWriter;}

    std::string getIdentifier() const {return "IndexWriterNotification";}

private:
    NotifyType m_notifyType;
    IndexWriter* m_pIndexWriter;
};

FX_NS_END

#endif 
