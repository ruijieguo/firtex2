//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2012-04-22 10:45:47

#ifndef __FX_SIMPLENUMERICINDEXMERGER_H
#define __FX_SIMPLENUMERICINDEXMERGER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/FieldMerger.h"
#include "firtex/index/TypedDictWriter.h"
#include "firtex/index/DocPostingMerger.h"

FX_NS_DEF(index);

template <typename T>
class SimpleNumericIndexMerger : public FieldMerger
{
public:
    typedef T KeyType;
    typedef TypedDictWriter<KeyType, offset_t> DictWriter;
    typedef FX_NS(common)::SharedPtr<DictWriter> DictWriterPtr;

public:
    SimpleNumericIndexMerger() : m_nTotalTerms(0) {}
    ~SimpleNumericIndexMerger() {}

protected:
    /**
     * Give dirived class a chance to know merging is begin
     * @param mergeFieldInfos field infos of multi index barrel
     */
    void beginMerge(const IndexMergeInfos& mergeFieldInfos);

    /**
     * Merge some terms inn different barrel
     * @param terms terms vectory to merge
     */
    void mergeTerms(const MergingTermVector& terms);

    /**
     * Give dirived class a chance to know merging is ended
     */
    void endMerge();

private:
    FX_NS(store)::OutputStreamPtr m_pDictOutStream;
    FX_NS(store)::OutputStreamPtr m_pDocOutStream;
    DictWriterPtr m_pDictWriter;

    int64_t m_nTotalTerms;

private:
    DECLARE_LOGGER();
};

//////////////////////////////////////////////////////
///
SETUP_LOGGER_TEMP(index, SimpleNumericIndexMerger);

template <typename T>
void SimpleNumericIndexMerger<T>::beginMerge(const IndexMergeInfos& mergeFieldInfos)
{
    std::string sSuffix = mergeFieldInfos.getSuffix();
    m_pDictOutStream = getFileSystem()->createFile(
            BarrelDirectory::getFilePath(getFieldSchema()->getName(),
                    DICT_FILEEXT, sSuffix));
    m_pDocOutStream = getFileSystem()->createFile(
            BarrelDirectory::getFilePath(getFieldSchema()->getName(),
                    DOC_POSTING_FILEEXT, sSuffix));
    m_pDictWriter.assign(new DictWriter(m_pDictOutStream));

    m_pDictWriter->reserve(10000); //TODO: optimize
}

template <typename T>
void SimpleNumericIndexMerger<T>::mergeTerms(const MergingTermVector& terms)
{
    const Term* pTerm = terms[0]->getTerm();
    const TypedTerm<KeyType>* pTypedTerm = pTerm->cast<KeyType>();
    DocPostingMerger postMerger;
    postMerger.init(m_pDocOutStream);

    offset_t off = postMerger.merge(terms);
    if (off != (offset_t)-1)
    {
        this->m_pDictWriter->pushBack(pTypedTerm->getValue(), off);
    }
}

template <typename T>
void SimpleNumericIndexMerger<T>::endMerge()
{
    this->m_pDictWriter->finalize();

    FX_DEBUG("End merge: total terms: %llu, distinct terms: %llu",
             this->m_nTotalTerms, this->m_pDictWriter->size());

    this->m_pDictOutStream->close();
    this->m_pDocOutStream->close();

    this->m_pDictOutStream.reset();
    this->m_pDocOutStream.reset();
}

FX_NS_END

#endif //__FX_SIMPLENUMERICINDEXMERGER_H
