#ifndef __FX_NOTIFIABLE_H
#define __FX_NOTIFIABLE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Notification.h"
#include <typeinfo>

FX_NS_DEF(common)

class Notifiable
{
public:
    virtual ~Notifiable() {}

public:
    virtual void update(const Notification& notif) = 0;
    
};

FX_NS_END

#endif
