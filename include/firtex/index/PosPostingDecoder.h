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
// Created	: 2010-08-14 23:22:12

#ifndef __FX_POSPOSTINGDECODER_H
#define __FX_POSPOSTINGDECODER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/DocPostingDecoder.h"
#include "firtex/index/PosSkipListReader.h"
#include "firtex/store/InputStream.h"

FX_NS_DEF(index);

class PosPostingDecoder : public DocPostingDecoder
{
public:
    PosPostingDecoder();
    ~PosPostingDecoder();

public:
    /**
     * Initialize
     *
     * @param pDocStream input stream for doc-list
     * @param pPosStream input stream for pos-list
     * @param pInStreamPool input stream pool for recycling streams
     */
    void init(FX_NS(store)::InputStreamPtr& pDocStream,
              FX_NS(store)::InputStreamPtr& pPosStream,
              const FX_NS(store)::InputStreamPool* pInStreamPool);

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
    virtual void readTermMeta();

protected:
    FX_NS(store)::InputStreamPtr m_pPosStream;
    PosSkipListReaderPtr m_pPosSkipListReader;

    const PostingCoding* m_pPosDecoder;
    loc_t m_lastPos;
    offset_t m_nPosStartOff;
    uint64_t m_nPosLength;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(PosPostingDecoder);

/////////////////////////////////////////////////////////

FX_NS_END

#endif //__FX_POSPOSTINGDECODER_H
