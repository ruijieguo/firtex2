//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2011-01-29 11:08:27

#ifndef __FX_PRIMARYKEYPOSTINGDECODER_H
#define __FX_PRIMARYKEYPOSTINGDECODER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/PostingDecoder.h"

FX_NS_DEF(index);

class PrimaryKeyPostingDecoder : public PostingDecoder
{
public:
    PrimaryKeyPostingDecoder(docid_t docId)
        : m_docId(docId)
    {
        m_termMeta.setDocFreq(1);
        m_termMeta.setCTF(1);
    }

    ~PrimaryKeyPostingDecoder() {}

public:
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
     * Skip #nSkipPosCount positions and decode pos record
     * @param posBuffer pos buffer to stored decoded values
     *      its size should not be less than RECORD_SIZE
     * @param nSkipPosCount number of positions to skip
     * @param nStartOff return offset of pos in posBuffer
     * @return number of values have been decoded
     */
    uint32_t decodePosRecord(loc_t* posBuffer, ctf_t nSkipPosCount,
                             uint32_t& nStartOff) { return 0;}

    /**
     * Return cumulated sum of tf
     */
    ctf_t getCumulatedSumTf() const { return m_termMeta.getCTF();}

private:
    docid_t m_docId;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_PRIMARYKEYPOSTINGDECODER_H
