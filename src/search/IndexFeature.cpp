#include "firtex/search/IndexFeature.h"

FX_NS_USE(index);
FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, IndexFeature);

IndexFeature::IndexFeature()
    : m_nTotalTermCount(0)
    , m_nTotalDocCount(0)
{

}

IndexFeature::~IndexFeature() 
{
}

void IndexFeature::init(const IndexReaderPtr& pIndexReader)
{
    m_nTotalDocCount = (uint64_t)pIndexReader->getDocCount();
    FX_STREAM_LOG(TRACE) << "Total doc count: " 
                         << m_nTotalDocCount << FIRTEX_ENDL;

    IndexMeta indexMeta = pIndexReader->getIndexMeta();

    m_nTotalTermCount = 0;
    for (size_t i = 0; i < indexMeta.size(); ++i)
    {
        const FieldMeta& fieldMeta = indexMeta[i];
        m_fieldsAvgLength.insert(make_pair(fieldMeta.fieldName,
                        (uint32_t)(fieldMeta.totalTermCount / m_nTotalDocCount)));
        
        FX_STREAM_LOG(TRACE) << "Average field length: field: " 
                             << fieldMeta.fieldName << ", length: "
                             << fieldMeta.totalTermCount/m_nTotalDocCount
                             << FIRTEX_ENDL;

        m_nTotalTermCount += fieldMeta.totalTermCount;
    }
    FX_STREAM_LOG(TRACE) << "Total term count: " 
                         << m_nTotalTermCount << FIRTEX_ENDL;

    m_pIndexReader = pIndexReader;
}

FX_NS_END

