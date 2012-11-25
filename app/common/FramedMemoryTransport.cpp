#include "FramedMemoryTransport.h"

FX_NS_DEF(app);

SETUP_LOGGER(common, FramedMemoryTransport);

FramedMemoryTransport::FramedMemoryTransport() 
{
    wroteBytes(sizeof(int32_t));
}

FramedMemoryTransport::~FramedMemoryTransport() 
{
}

void FramedMemoryTransport::flush()
{
    int32_t len = writeEnd() - sizeof(uint32_t);
    len = (int32_t)htonl((uint32_t)(len));
    *(uint32_t*)rBase_ = (uint32_t)len;
    FX_TRACE("Flush data, length: [%d]", len);
}

FX_NS_END

