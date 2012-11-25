#include "firtex/utility/SimpleAllocator.h"

FX_NS_DEF(utility);

SimpleAllocator::SimpleAllocator()
{
}

SimpleAllocator::~SimpleAllocator() 
{
    clear();
}

FX_NS_END
