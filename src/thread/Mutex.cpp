#include "firtex/thread/Mutex.h"


FX_NS_DEF(thread);

Mutex::Mutex()
    : m_pImpl(new MutexImpl)
{
}

Mutex::~Mutex()
{
    delete m_pImpl;
    m_pImpl = NULL;
}

FX_NS_END
