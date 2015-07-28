#include "firtex/search/SimpleHitCollector.h"
#include "firtex/search/ScoredDoc.h"

FX_NS_USE(utility);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, SimpleHitCollector);

SimpleHitCollector::SimpleHitCollector(size_t nTopDocs,
                                       score_t minScore)
    : m_nTotalHits(0)
    , m_nTopDocs(nTopDocs)
    , m_minScore(minScore)
{
    m_pAllocator.reset(new SimpleSegregatedAllocator(sizeof(ScoredDoc),
                    (uint32_t)nTopDocs + 1));
    ScoredDocDestroyerPtr pDestroyer(new ScoredDocDestroyer(m_pAllocator));
    m_pHitQueue.reset(new SimpleHitQueue(nTopDocs, pDestroyer));
}

SimpleHitCollector::~SimpleHitCollector()
{
}

void SimpleHitCollector::collect(MatchedDocSet& matchedDocs)
{
    size_t nTopDocs = m_nTopDocs;
    size_t size = matchedDocs.size();
    for (size_t i = 0; i < size; i++)
    {
        const MatchedDoc& matchedDoc = matchedDocs[i];
        ++m_nTotalHits;
        if (m_pHitQueue->size() < nTopDocs || matchedDoc.getScore() >= m_minScore)
        {
            void* pAddr = m_pAllocator->allocate();
            ScoredDoc* sd = new (pAddr) ScoredDoc(matchedDocs[i].getDocId(), 
                    matchedDocs[i].getScore());
            if (isFiltered(*sd))
            {  
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

size_t SimpleHitCollector::totalHits() const
{
    return m_nTotalHits;
}

HitQueuePtr SimpleHitCollector::getHitQueue() const
{
    return m_pHitQueue;
}

FX_NS_END

