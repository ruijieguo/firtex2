#include "firtex/common/RefCountedObject.h"

FX_NS_DEF(common);

RefCountedObject::RefCountedObject()
    : m_refCounter(1)
{
}

RefCountedObject::~RefCountedObject()
{
}

void RefCountedObject::duplicate() const
{
    ++m_refCounter;
}

void RefCountedObject::release() const
{
    --m_refCounter;
    if (m_refCounter.load() == 0) 
    {
        delete this;
    }
}

FX_NS_END
