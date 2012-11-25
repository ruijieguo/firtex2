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
// Created	: 2010-09-04 11:53:37

#ifndef __FX_POSTINGDECODER_H
#define __FX_POSTINGDECODER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/TermMeta.h"

FX_NS_DEF(index);

class PostingDecoder
{
public:
    PostingDecoder() {}
    virtual ~PostingDecoder() {}

public:
    /**
     * Return term meta
     */
    const TermMeta& getTermMeta() const {return m_termMeta;}

    /**
     * Decode doc record whose value equal to or greater than docId
     * @param docBuffer doc buffer to store decoded values,
     *        its size should not be less than RECORD_SIZE
     * @param docId target doc id value
     * @return number of values have been decoded
     */
    virtual uint32_t decodeDocRecord(docid_t* docBuffer, docid_t docId) = 0;

    /**
     * Decode tf record of current record
     * @param tfBuffer tf buffer to stored decoded values,
     *                 its size should not be less than RECORD_SIZE
     * @return number of values have been decoded
     */
    virtual uint32_t decodeTfRecord(tf_t* tfBuffer) = 0;

    /**
     * Skip #nSkipPosCount positions and decode pos record
     * @param posBuffer pos buffer to stored decoded values
     *      its size should not be less than RECORD_SIZE
     * @param nSkipPosCount number of positions to skip
     * @param nStartOff return offset of pos in posBuffer
     * @return number of values have been decoded
     */
    virtual uint32_t decodePosRecord(loc_t* posBuffer, ctf_t nSkipPosCount,
            uint32_t& nStartOff) { return 0;}

    /**
     * Return cumulated sum of tf
     */
    virtual ctf_t getCumulatedSumTf() const { return 0;}

protected:
    TermMeta m_termMeta;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(PostingDecoder);

FX_NS_END

#endif //__FX_POSTINGDECODER_H
