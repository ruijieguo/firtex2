#include "firtex/search/FeatureProvider.h"

FX_NS_USE(index);
FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, FeatureProvider);

FeatureProvider::FeatureProvider() 
{
}

FeatureProvider::~FeatureProvider() 
{
}

void FeatureProvider::init(const IndexReaderPtr& pIndexReader)
{
    m_indexFeature.init(pIndexReader);
}

FX_NS_END

