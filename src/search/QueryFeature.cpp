#include "firtex/search/QueryFeature.h"

FX_NS_DEF(search);

QueryFeature::QueryFeature() 
    : m_featureArray(NULL)
    , m_nCapacity(0) 
    , m_nSize(0)
{
}

QueryFeature::QueryFeature(size_t nNumTerms) 
    : m_featureArray(NULL)
    , m_nCapacity(0) 
    , m_nSize(0)
{
    reserve(nNumTerms);
}

QueryFeature::~QueryFeature() 
{
    if (m_featureArray)
    {
        delete[] m_featureArray;
    }
}

void QueryFeature::reserve(size_t nCapacity)
{
    if (nCapacity > m_nCapacity)
    {
        Vector tmpArray = new TermQueryFeature[nCapacity];
        for (size_t i = 0; i < m_nCapacity; ++i)
        {
            tmpArray[i] = m_featureArray[i];
        }
        if (m_featureArray)
        {
            delete[] m_featureArray;
        }
        m_featureArray = tmpArray;
        m_nCapacity = nCapacity;
    }
}


FX_NS_END

