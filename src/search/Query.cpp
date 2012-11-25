#include "firtex/search/Query.h"

FX_NS_USE(utility);
FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, Query);

Query::Query() 
    : m_fBoost(1.0)
{
}

Query::~Query() 
{
}

bool Query::isInstanceOf(const tstring& iden) const
{
    const tstring& id = getIdentifier();
    if (strCompare(id.c_str(), iden.c_str()) == 0)
    {
        return true;
    }
    return false;
}

FX_NS_END

