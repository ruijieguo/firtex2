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
// Created	: 2010-08-21 11:50:44

#ifndef __FX_LENGTHNORMMERGER_H
#define __FX_LENGTHNORMMERGER_H

#include <list>
#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/FileSystem.h"
#include "firtex/index/MergeInfo.h"
#include "firtex/document/DocumentSchema.h"

FX_NS_DEF(index);

class LengthNormMerger
{
public:
    class StandaloneNormMerger
    {
    public:
        StandaloneNormMerger(const FX_NS(document)::FieldSchema& fieldSchema);

    public:
        /**
         * Merge multi barrel to one barrel
         * @param pFileSys file system instance
         * @param mergeInfos merge infos
         */
        void merge(FX_NS(store)::FileSystemPtr& pFileSys,
                   const MergeInfos& mergeInfos);

    private:
        std::string m_sNormField;
    };
    DEFINE_TYPED_PTR(StandaloneNormMerger);

public:
    LengthNormMerger();
    ~LengthNormMerger();

public:
    /**
     * Inialize
     */
    void init(const FX_NS(document)::DocumentSchema* pDocSchema);

    /**
     * Merge multi barrel to one barrel
     * @param pBarrel barrels to merge
     * @param pFileSys file system instance
     */
    void merge(FX_NS(store)::FileSystemPtr& pFileSys,
               const MergeInfos& mergeInfos);

private:
    typedef std::list<StandaloneNormMergerPtr> MergerList;
    
    MergerList m_merges;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(LengthNormMerger);

FX_NS_END

#endif //__FX_LENGTHNORMMERGER_H
