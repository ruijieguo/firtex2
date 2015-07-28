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
// Created	: 2010-08-21 20:10:41

#ifndef __FX_PRIMITIVETYPEFORWARDINDEXMERGER_H
#define __FX_PRIMITIVETYPEFORWARDINDEXMERGER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/ForwardIndexMerger.h"
#include "firtex/index/BarrelDirectory.h"
#include "firtex/index/forwardindex/PrimitiveTypeForwardIndexWriter.h"

FX_NS_DEF(index);

template <typename T>
class PrimitiveTypeForwardIndexMerger : public ForwardIndexMerger
{
public:
    typedef T ValueType;

    typedef typename PrimitiveTypeForwardIndexWriter<T>::IteratorType IteratorType;

public:
    PrimitiveTypeForwardIndexMerger() {}
    ~PrimitiveTypeForwardIndexMerger() {}

public:
    /**
     * Merge multi forward index barrel of one field to one barrel
     * @param mergeInfos field infos of multi index barrel
     */
    void merge(const ForwardIndexMergeInfos& mergeInfos);

private:
    void doOnDiskMerge(const ForwardIndexMergeInfos& mergeInfos);
    void doInMemMerge(const ForwardIndexMergeInfos& mergeInfos);

private:
    DECLARE_STREAM_LOGGER();
};

/////////////////////////////////////////////
SETUP_STREAM_LOGGER_TEMP(index, PrimitiveTypeForwardIndexMerger);

template <typename T>
void PrimitiveTypeForwardIndexMerger<T>::merge(const ForwardIndexMergeInfos& mergeInfos)
{
    if (mergeInfos.size() <= 0)
    {
        return;
    }

    if (!mergeInfos[0].getIterator())
    {
        doOnDiskMerge(mergeInfos);
    }
    else
    {
        doInMemMerge(mergeInfos);
    }
}

template <typename T>
void PrimitiveTypeForwardIndexMerger<T>::doOnDiskMerge(const ForwardIndexMergeInfos& mergeInfos)
{
    std::string sFileName = BarrelDirectory::getFilePath(this->getFieldSchema()->getName(),
            FORWARD_INDEX_FILEEXT, mergeInfos.getSuffix());

    int64_t nTotalSize = 0;
    for (size_t i = 0; i < mergeInfos.size(); ++i)
    {
        const MergeInfo& mergeInfo = mergeInfos[i];
        const DocIdRecycling* pDocIdRecy = mergeInfo.getDocIdRecycling();
        nTotalSize += pDocIdRecy->getDocCountAfterRecle();
    }

    FX_NS(store)::OutputStreamPtr pOutStream =
        m_pFileSys->createFile(sFileName);

    int64_t nTotalMergedSize = 0;

    for (size_t i = 0; i < mergeInfos.size(); ++i)
    {
        const MergeInfo& mergeInfo = mergeInfos[i];
        if (mergeInfo.getBarrelInfo().getDocCount() <= 0)
        {
            continue;
        }

        std::string sName = BarrelDirectory::getFilePath(getFieldSchema()->getName(),
                FORWARD_INDEX_FILEEXT, mergeInfo.getBarrelInfo().getSuffix());
        FX_NS(store)::InputStreamPtr pInStream = m_pFileSys->openFile(sName);

        int64_t nSize = (int64_t)(pInStream->getSize()/sizeof(ValueType));

        const DocIdRecycling* pDocIdRecy = mergeInfo.getDocIdRecycling();
        if (!pDocIdRecy->hasDeletions())
        {
            FX_LOG(TRACE, "No deletion, offset: %lld, length: %lld",
                   pInStream->tell(), pInStream->getSize());
            pOutStream->write(*pInStream);
            nTotalMergedSize += nSize;
        }
        else
        {
            for (int64_t j = 0; j < nSize; ++j)
            {
                ValueType value;
                pInStream->read((void*)&value, sizeof(ValueType));
                if (pDocIdRecy->remap((docid_t)j) != INVALID_DOCID)
                {
                    pOutStream->write((const void*)&value, sizeof(ValueType));
                    ++nTotalMergedSize;
                }
            }
        }
    }
    if (nTotalMergedSize != nTotalSize)
    {
        FIRTEX_THROW(IndexCollapseException,
                     "Merged size is inconsistent with that before merging.");
    }
    pOutStream->close();
}

template <typename T>
void PrimitiveTypeForwardIndexMerger<T>::doInMemMerge(const ForwardIndexMergeInfos& mergeInfos)
{
    std::string sFileName = BarrelDirectory::getFilePath(this->getFieldSchema()->getName(),
            FORWARD_INDEX_FILEEXT, mergeInfos.getSuffix());

    int64_t nTotalSize = 0;
    for (size_t i = 0; i < mergeInfos.size(); ++i)
    {
        const MergeInfo& mergeInfo = mergeInfos[i];
        const DocIdRecycling* pDocIdRecy = mergeInfo.getDocIdRecycling();
        nTotalSize += pDocIdRecy->getDocCountAfterRecle();
    }

    FX_NS(store)::OutputStreamPtr pOutStream = m_pFileSys->createFile(sFileName);

    int64_t nTotalMergedSize = 0;

    for (size_t i = 0; i < mergeInfos.size(); ++i)
    {
        const ForwardIndexMergeInfo& mergeInfo = mergeInfos[i];
        if (mergeInfo.getBarrelInfo().getDocCount() <= 0)
        {
            continue;
        }

        ForwardIndexIteratorPtr pIterator = mergeInfo.getIterator();
        IteratorType* pTypedIt = dynamic_cast<IteratorType*>(pIterator.get());
        FIRTEX_ASSERT2(pTypedIt != NULL);

        size_t nSize = pTypedIt->size();

        const DocIdRecycling* pDocIdRecy = mergeInfo.getDocIdRecycling();
        if (!pDocIdRecy->hasDeletions())
        {
            for (docid_t docId = 0; docId < (docid_t)nSize; ++docId)
            {
                ValueType v;
                pTypedIt->seek(docId, v);
                pOutStream->write((const void*)&v, sizeof(ValueType));
            }
            nTotalMergedSize += nSize;
        }
        else
        {
            for (docid_t docId = 0; docId < (docid_t)nSize; ++docId)
            {
                ValueType value;
                pTypedIt->seek(docId, value);
                if (pDocIdRecy->remap(docId) != INVALID_DOCID)
                {
                    pOutStream->write((const void*)&value, sizeof(ValueType));
                    ++nTotalMergedSize;
                }
            }
        }
    }
    if (nTotalMergedSize != nTotalSize)
    {
        FIRTEX_THROW(IndexCollapseException,
                     "Merged size is inconsistent with that before merging.");
    }
    pOutStream->close();
}

FX_NS_END

#endif //__FX_PRIMITIVETYPEFORWARDINDEXMERGER_H
