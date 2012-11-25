//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn
// Created	: 2006/1/4
//
#ifndef __FX_MULTIPOSTINGDECODER_H
#define __FX_MULTIPOSTINGDECODER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/common/Logger.h"
#include "firtex/index/PostingDecoder.h"
#include "firtex/index/BarrelsInfo.h"
#include <list>

FX_NS_DEF(index);

class MultiPostingDecoder : public PostingDecoder
{
    struct Entry
    {
    public:
        Entry(const BarrelInfo* pBarrelInfo,
              const PostingDecoderPtr& pPosting)
            : m_pBarrelInfo(new BarrelInfo(*pBarrelInfo))
            , m_pPosting(pPosting)
        {
        }

        ~Entry() {}

    public:
        BarrelInfoPtr m_pBarrelInfo;
        PostingDecoderPtr m_pPosting;
    };

    DEFINE_TYPED_PTR(Entry);

    typedef std::list<EntryPtr> DecoderList;

public:
    MultiPostingDecoder(void);
    virtual ~MultiPostingDecoder(void);

public:
    /**
     * add a term docs&freqs
     * @param pBarrelInfo information of index barrel
     * @param pPosting term docs&freqs of a index barrel
     */
    void addDecoder(const BarrelInfo* pBarrelInfo,
                    const PostingDecoderPtr& pPosting);

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
                             uint32_t& nStartOff);

protected:
    DecoderList m_postingDecoders;
    Entry* m_pCurrent;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif
