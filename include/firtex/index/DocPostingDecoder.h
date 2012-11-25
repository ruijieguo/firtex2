//
// Copyright(C) 2005--2010 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2010-08-14 11:45:39

#ifndef __FX_DOCPOSTINGDECODER_H
#define __FX_DOCPOSTINGDECODER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/InputStream.h"
#include "firtex/store/InputStreamPool.h"
#include "firtex/index/TermMeta.h"
#include "firtex/index/PostingDecoder.h"
#include "firtex/index/PostingCoding.h"
#include "firtex/index/DocSkipListReader.h"

FX_NS_DEF(index);

class DocPostingDecoder : public PostingDecoder
{
public:
    DocPostingDecoder();
    ~DocPostingDecoder();

public:
    /**
     * Initialize
     *
     * @param pDocStream input stream for doc-list
     * @param pInStreamPool input stream pool for recycling streams
     */
    void init(const FX_NS(store)::InputStreamPtr& pDocStream,
              const FX_NS(store)::InputStreamPtr& pPosStream,
              const FX_NS(store)::InputStreamPool* pInStreamPool);

    /**
     * Decode doc record whose value equal to or greater than docId
     * @param docBuffer doc buffer to store decoded values,
     *        its size should not be less than RECORD_SIZE
     * @param docId target doc id value
     * @return number of values have been decoded
     */
    uint32_t decodeDocRecord(docid_t* docBuffer, docid_t docId);

    /**
     * Decode tf record of current record
     * @param tfBuffer tf buffer to stored decoded values,
     *                 its size should not be less than RECORD_SIZE
     * @return number of values have been decoded
     */
    uint32_t decodeTfRecord(tf_t* tfBuffer);

    /**
     * Return cumulated sum of tf
     */
    ctf_t getCumulatedSumTf() const;

protected:
    virtual void readTermMeta();

protected:
    const FX_NS(store)::InputStreamPool* m_pInStreamPool;
    FX_NS(store)::InputStreamPtr m_pDocStream;
    DocSkipListReaderPtr m_pDocSkipListReader;
    docid_t m_lastDocId;
    df_t m_nNumDocsDecoded;
    offset_t m_nStartOff;
    uint64_t m_nDocLength;
    offset_t m_nLastSeekOff;

    const PostingCoding* m_pDocDecoder;
    const PostingCoding* m_pTfDecoder;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(DocPostingDecoder);

/////////////////////////////////////////////////////////
//
inline ctf_t DocPostingDecoder::getCumulatedSumTf() const
{
    return m_pDocSkipListReader.isNotNull() ? m_pDocSkipListReader->getCurPoint().cumSumTf : 0;
}

FX_NS_END

#endif //__FX_DOCPOSTINGDECODER_H
