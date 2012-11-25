#ifndef __NOTIFICATION_H
#define __NOTIFICATION_H

#include "firtex/common/SharedPtr.h"
#include <typeinfo>
#include <string>

FX_NS_DEF(common);

/**
 * @class Notification
 * @brief Base class for all notification classes 
 */
class Notification
{
public:
    Notification(){}
    virtual ~Notification(){}

public:
    /**
     * Get the name of the notification.
     * The default implementation returns the class name.
     */
    virtual std::string getIdentifier() const {return typeid(*this).name();}
};

DEFINE_TYPED_PTR(Notification);

FX_NS_END

#endif 
