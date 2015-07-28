#include "firtex/search/SortedHitCollector.h"

FX_NS_USE(utility);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, SortedHitCollector);

SortedHitCollector::SortedHitCollector(const SorterPtr& pSort, 
                                       size_t nTopDocs,
                                       score_t minScore)
    : m_pSort(pSort)
    , m_nTotalHits(0)
    , m_nTopDocs(nTopDocs)
    , m_minScore(minScore)
{
    m_pAllocator.reset(new SimpleSegregatedAllocator(
                    sizeof(ScoredDoc) + pSort->getLastExprValueIdx(),
                    (uint32_t)nTopDocs + 1));
    ScoredDocDestroyerPtr pDestroyer(new ScoredDocDestroyer(m_pAllocator));
    m_pHitQueue.reset(new SortedHitQueue(pSort, nTopDocs, pDestroyer));
}

SortedHitCollector::~SortedHitCollector()
{
}

void SortedHitCollector::collect(MatchedDocSet& matchedDocs)
{
    for (size_t i = 0; i < matchedDocs.size(); i++)
    {
        const MatchedDoc& matchedDoc = matchedDocs[i];
        FX_TRACE("Collect doc: [%d], score: [%.2f]",
                 matchedDoc.getDocId(), matchedDoc.getScore());
        ++m_nTotalHits;
        if (m_pHitQueue->size() < m_nTopDocs || matchedDoc.getScore() >= m_minScore)
        {
            void* pAddr = m_pAllocator->allocate();
            ScoredDoc* sd = new (pAddr) ScoredDoc(matchedDocs[i].getDocId(), 
                    matchedDocs[i].getScore(), m_pSort->getLastExprValueIdx());
            if (isFiltered(*sd))
            {
                m_pSort->evaluate(*sd);
                if (!m_pHitQueue->insert(sd))
                {
                    m_pAllocator->free(sd);
                }
                else 
                {
                    m_minScore = m_pHitQueue->top()->getScore(); 
                }
            }
            else
            {
                --m_nTotalHits;
                m_pAllocator->free(sd);
            }
        }
    }
}

size_t SortedHitCollector::totalHits() const
{
    return m_nTotalHits;
}

HitQueuePtr SortedHitCollector::getHitQueue() const
{
    return std::dynamic_pointer_cast<HitQueue>(m_pHitQueue);
}

FX_NS_END

