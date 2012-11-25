#include "firtex/thread/FastMutex.h"


FX_NS_DEF(thread);

FastMutex::FastMutex()
    : m_pImpl(new FastMutexImpl)
{
}

FastMutex::~FastMutex()
{
    delete m_pImpl;
    m_pImpl = NULL;
}

FX_NS_END
