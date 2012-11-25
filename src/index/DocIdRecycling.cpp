#include "firtex/index/DocIdRecycling.h"

FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, DocIdRecycling);

DocIdRecycling::DocIdRecycling()
    : m_pDocIdMapping(NULL)
    , m_docCount(0)
    , m_docCountAfterRecycle(0)
{
}

DocIdRecycling::~DocIdRecycling() 
{
    if (m_pDocIdMapping)
    {
        delete m_pDocIdMapping;
        m_pDocIdMapping = NULL;
    }
}

void DocIdRecycling::init(df_t docCount, const BitVector* pDocFilter)
{
    FIRTEX_ASSERT2(m_pDocIdMapping == NULL);

    m_docCount = docCount;
    if (pDocFilter && pDocFilter->any())
    {
        m_pDocIdMapping = new Vector(SLICE_SIZE,
                (docCount + SLICE_SIZE - 1) / SLICE_SIZE);
        size_t j = 0;
        for (size_t i = 0; i < (size_t)docCount; ++i)
        {
            if (!pDocFilter->test(i))
            {
                remap((docid_t)i, (docid_t)j++);
                FX_TRACE("Remap [%u]->[%u]", (uint32_t)i, (uint32_t)j);
            }
            else
            {
                remap((docid_t)i, INVALID_DOCID);
                FX_TRACE("Remap [%u]->[-1]", (uint32_t)i);
            }
        }
        m_docCountAfterRecycle = (df_t)j;
    }
    else
    {
        m_docCountAfterRecycle = docCount;
    }
}

FX_NS_END

