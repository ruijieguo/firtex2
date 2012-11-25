#include "firtex/search/FeatureProvider.h"

FX_NS_USE(index);
FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, FeatureProvider);

FeatureProvider::FeatureProvider() 
    : m_pSimilarity(NULL)
{
}

FeatureProvider::~FeatureProvider() 
{
}

void FeatureProvider::init(const IndexReaderPtr& pIndexReader,
                           const Similarity* pSimilarity)
{
    m_indexFeature.init(pIndexReader);
    m_pSimilarity = pSimilarity;
}

FX_NS_END

