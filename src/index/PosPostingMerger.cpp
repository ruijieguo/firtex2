#include "firtex/index/PosPostingMerger.h"

FX_NS_USE(utility);
FX_NS_USE(store);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, PosPostingMerger);

PosPostingMerger::PosPostingMerger() 
{
    m_pDocBuffer = new docid_t[RECORD_SIZE];
    m_pTfBuffer = new tf_t[RECORD_SIZE];

    m_pPosBuffer = new loc_t[RECORD_SIZE];
}

PosPostingMerger::~PosPostingMerger() 
{
    delete[] m_pDocBuffer;
    m_pDocBuffer = NULL;

    delete[] m_pTfBuffer;
    m_pTfBuffer = NULL;

    delete[] m_pPosBuffer;
    m_pPosBuffer = NULL;
}

void PosPostingMerger::beginMerge(const MergingTermVector& terms)
{
    FX_TRACE("Begin merge pos posting");
    TermMeta& termMeta = getTermMeta();
    termMeta.setDocFreq(0);
    termMeta.setCTF(0);
    m_streammer.beginMerge();
}

void PosPostingMerger::doMerge(const MergingTerm* pTerm)
{
    FX_DEBUG("Merge pos posting: doc count: %d, "
             "base docid: %d, new base docid: %d", 
             pTerm->getBarrelInfo().getDocCount(),
             pTerm->getBaseDocId(), pTerm->getNewBaseDocId());

    docid_t docBuffer[RECORD_SIZE];
    docid_t tfBuffer[RECORD_SIZE];
    loc_t posBuffer[RECORD_SIZE];

    PostingDecoderPtr pDecoder = pTerm->getPostingIterator()->getPostingDecoder();
    uint32_t nDecodedPos = 0;
    ctf_t nTotalDecodedPos = 0;
    
    const DocIdRecycling* pDocRecycle = pTerm->getDocIdRecycling();
    if (pDocRecycle->hasDeletions()) // has deletions
    {
        docid_t baseDocId = pTerm->getNewBaseDocId();
        uint32_t nDecoded = 0;
        docid_t lastDocId = 0;
        size_t nNumPosToAdd = 0;
        size_t nDecodedPos = 0;
        size_t nCursorInPosRecord = 0;
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
                docid_t newDocId = pDocRecycle->remap(docId);
                if (newDocId != INVALID_DOCID)
                {
                    docBuffer[i] = newDocId;
                    tf_t tf = tfBuffer[j];
                    tfBuffer[i++] = tf;
                    while (tf > 0)
                    {
                        if (nCursorInPosRecord == nDecodedPos)
                        {
                            uint32_t startOff = 0;
                            nDecodedPos = pDecoder->decodePosRecord(posBuffer,
                                    nTotalDecodedPos, startOff);
                            nTotalDecodedPos += nDecodedPos;
                            nCursorInPosRecord = 0;
                        }
                        nNumPosToAdd = ((size_t)tf > (nDecodedPos - nCursorInPosRecord)) ?
                                       nDecodedPos - nCursorInPosRecord : (size_t)tf;
                        addPositions(posBuffer + nCursorInPosRecord, (uint32_t)nNumPosToAdd);
                        nCursorInPosRecord += nNumPosToAdd;
                        tf -= (tf_t)nNumPosToAdd;
                    }
                }
                else // the document is deleted
                {
                    //skip poisitions
                    size_t skippedPos = (size_t)tfBuffer[j];
                    while (skippedPos > 0)
                    {
                        if (nCursorInPosRecord == nDecodedPos)
                        {
                            uint32_t startOff = 0;
                            nDecodedPos = pDecoder->decodePosRecord(posBuffer,
                                    nTotalDecodedPos, startOff);
                            nTotalDecodedPos += nDecodedPos;
                            nCursorInPosRecord = 0;
                        }
                        if (skippedPos >= (nDecodedPos - nCursorInPosRecord))
                        {
                            skippedPos -= (nDecodedPos - nCursorInPosRecord);
                            nCursorInPosRecord = nDecodedPos;
                        }
                        else
                        {
                            nCursorInPosRecord += skippedPos;
                            skippedPos = 0;
                        }
                    }// end while (skippedPos > 0)
                }
            }// end for

            if (i > 0)
            {
                commitDocuments(baseDocId, docBuffer, tfBuffer, (uint32_t)i);
            }
        }//end while
    }
    else
    {
        doMergeDocList(pTerm);
        uint32_t startOff = 0;
        while ((nDecodedPos = pDecoder->decodePosRecord(posBuffer,
                                nTotalDecodedPos, startOff)) > 0)
        {
            addPositions(posBuffer, nDecodedPos);
            nTotalDecodedPos += nDecodedPos;
        }
    }
}

void PosPostingMerger::doMergeDocList(const MergingTerm* pTerm)
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
            lastDocId = docBuffer[nDecoded - 1] + 1;
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

void PosPostingMerger::commitDocuments(docid_t baseDocId, docid_t* docBuffer,
                                       tf_t* tfBuffer, uint32_t nSize)
{
    FX_TRACE("commit documents: last committed doc: %d, base doc id: %d, "
                    "first doc: %d, length: %d",
             m_lastDocId, baseDocId, docBuffer[0], (int32_t)nSize);

    if (m_nUpTo == RECORD_SIZE)
    {
        flushBuffer(false);
    }

    docid_t docId = docBuffer[0] + baseDocId;
    m_pDocBuffer[m_nUpTo] = docId - m_lastDocId;
    m_lastDocId = docId;

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

    getTermMeta().getDocFreq() += nSize;
    getTermMeta().getCTF() += totalTf;
    m_curTermFreq = 0;
}

offset_t PosPostingMerger::endMerge()
{
    FX_TRACE("End merge pos posting");
    OutputStreamPtr pNullStream;
    return commit(pNullStream, pNullStream);
}

offset_t PosPostingMerger::commit(OutputStreamPtr& pDocStream, OutputStreamPtr& pPosStream)
{
    if (getTermMeta().getDocFreq() > 0)
    {
        return TypedPosPostingWriter<PosPostingMergerStreammer>::commit(
                m_streammer.m_pDocListWriter, m_streammer.m_pPosListWriter);
    }
    return (offset_t)-1;
}

FX_NS_END

