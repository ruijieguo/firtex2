#include "firtex/search/ScorerProfile.h"
#include "firtex/search/BM25Scorer.h"

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, ScorerProfile);

ScorerProfile::ScorerProfile() 
{
}

ScorerProfile::~ScorerProfile() 
{
}

ScorerPtr ScorerProfile::createScorer() const
{
    return ScorerPtr(new BM25Scorer);
}

const Similarity* ScorerProfile::getSimilarity() const
{
    //TODO:
    return NULL;
}

FX_NS_END

