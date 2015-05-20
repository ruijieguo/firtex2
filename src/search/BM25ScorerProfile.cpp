#include "firtex/search/BM25ScorerProfile.h"
#include "firtex/search/BM25Scorer.h"

FX_NS_DEF(search);

SETUP_LOGGER(search, BM25ScorerProfile);

BM25ScorerProfile::BM25ScorerProfile() 
{
}

BM25ScorerProfile::~BM25ScorerProfile() 
{
}

std::string BM25ScorerProfile::getProfileName() const
{
    return "bm25";
}

ScorerPtr BM25ScorerProfile::createScorer() const
{
    return ScorerPtr(new BM25Scorer);
}

FX_NS_END

