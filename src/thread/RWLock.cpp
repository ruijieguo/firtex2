#include "firtex/thread/RWLock.h"

FX_NS_DEF(thread);

RWLock::RWLock() : m_pImpl(new RWLockImpl)
{
}

	
RWLock::~RWLock()
{
    delete m_pImpl;
    m_pImpl = NULL;
}

FX_NS_END



