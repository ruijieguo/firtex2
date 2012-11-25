#include "firtex/thread/Event.h"

FX_NS_DEF(thread);


Event::Event(bool bAutoReset): EventImpl(bAutoReset)
{
}

Event::~Event()
{
}

FX_NS_END
