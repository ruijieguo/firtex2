#include "firtex/search/ScorerProfileFactory.h"

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, ScorerProfileFactory);

ScorerProfileFactory::ScorerProfileFactory() 
{
}

ScorerProfileFactory::~ScorerProfileFactory() 
{
}

const ScorerProfile* ScorerProfileFactory::getProfile(const tstring& sProfileName)
{
//    return ScorerProfile();
    return NULL;
    //TODO:
}

FX_NS_END

