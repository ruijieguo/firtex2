#include "firtex/index/DocPostingMerger.h"

FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, DocPostingMergerStreammer);
SETUP_STREAM_LOGGER(index, DocPostingMerger);

//////////////////////////////////////
DocPostingMerger::DocPostingMerger() 
{
    m_pDocBuffer = new docid_t[RECORD_SIZE];
    m_pTfBuffer = new tf_t[RECORD_SIZE];
}

DocPostingMerger::~DocPostingMerger() 
{
    delete[] m_pDocBuffer;
    m_pDocBuffer = NULL;
    delete[] m_pTfBuffer;
    m_pTfBuffer = NULL;
}

void DocPostingMerger::beginMerge(const MergingTermVector& terms)
{
    TermMeta& termMeta = getTermMeta();
    termMeta.setDocFreq(0);
    termMeta.setCTF(0);
    m_streammer.beginMerge();
}

void DocPostingMerger::doMerge(const MergingTerm* pTerm)
{
    docid_t docBuffer[RECORD_SIZE];
    docid_t tfBuffer[RECORD_SIZE];

    PostingDecoderPtr pDecoder = pTerm->getPostingIterator()->getPostingDecoder();
    const DocIdRecycling* pDocIdRecycle = pTerm->getDocIdRecycling();
    if (pDocIdRecycle->hasDeletions())
    {
        docid_t baseDocId = pTerm->getNewBaseDocId();
        uint32_t nDecoded = 0;
        docid_t lastDocId = 0;
        while ((nDecoded = pDecoder->decodeDocRecord(docBuffer, lastDocId)) > 0)
        {
            if (pDecoder->decodeTfRecord(tfBuffer) != nDecoded)
            {
                FIRTEX_THROW(IndexCollapseException,
                        "Doc and Tf record is inconsistant.");
            }

            lastDocId = docBuffer[nDecoded - 1] + 1;

            size_t i, j;
            for (i = 0, j = 0; j < nDecoded; ++j)
            {
                docid_t docId = docBuffer[j];
                docid_t newDocId = pDocIdRecycle->remap(docId);
                if (newDocId != INVALID_DOCID)
                {
                    docBuffer[i] = newDocId;
                    tfBuffer[i] = tfBuffer[j];
                    ++i;
                }
            }
            if (i > 0)
            {
                commitDocuments(baseDocId, docBuffer, tfBuffer, (uint32_t)i);
            }
        }
    }
    else
    {
        docid_t baseDocId = pTerm->getNewBaseDocId();
        uint32_t nDecoded = 0;
        docid_t lastDocId = 0;
        FX_TRACE("Merge doc list: base doc id: %d", baseDocId);

        while ((nDecoded = pDecoder->decodeDocRecord(docBuffer, lastDocId)) > 0)
        {
            if (pDecoder->decodeTfRecord(tfBuffer) != nDecoded)
            {
                FIRTEX_THROW(IndexCollapseException,
                        "Doc and Tf record is inconsistant.");
            }
            commitDocuments(baseDocId, docBuffer, tfBuffer, nDecoded);
            lastDocId = docBuffer[nDecoded - 1] + 1;
        }
    }
}

offset_t DocPostingMerger::endMerge()
{
    FX_DEBUG("end merge doc posting");
    OutputStreamPtr nullStream;
    return commit(nullStream);
}

void DocPostingMerger::commitDocuments(docid_t baseDocId, docid_t* docBuffer,
                                       tf_t* tfBuffer, uint32_t nSize)
{
    FX_TRACE("commit documents: last committed doc: %d, base doc id: %d, "
                    "first doc: %d, length: %d",
                    m_lastDocId, baseDocId, docBuffer[0], (int32_t)nSize);

    if (m_nUpTo == RECORD_SIZE)
    {
        flushBuffer(false);
    }

    m_pDocBuffer[m_nUpTo] = docBuffer[0] + baseDocId - m_lastDocId;
    m_lastDocId = docBuffer[0] + baseDocId;

    ctf_t totalTf = tfBuffer[0];
    m_pTfBuffer[m_nUpTo] = (tf_t)totalTf;
    ++m_nUpTo;

    tf_t tf;
    for (uint32_t i = 1; i < nSize; ++i)
    {
        if (m_nUpTo == RECORD_SIZE)
        {
            getTermMeta().getCTF() += totalTf;
            flushBuffer(false);
            totalTf = 0;
        }
        m_pDocBuffer[m_nUpTo] = docBuffer[i] - docBuffer[i - 1];
        tf = tfBuffer[i];
        m_pTfBuffer[m_nUpTo] = tf;
        totalTf += tf;
        m_lastDocId = docBuffer[i] + baseDocId;
        ++m_nUpTo;
    }

    TermMeta& termMeta = getTermMeta();
    termMeta.getDocFreq() += nSize;
    termMeta.getCTF() += totalTf;
    
    m_curTermFreq = 0;
}

offset_t DocPostingMerger::commit(FX_NS(store)::OutputStreamPtr& pDocStream)
{
    return TypedDocPostingWriter<DocPostingMergerStreammer>::commit(
            m_streammer.m_pDocListWriter);
}

FX_NS_END

