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
// Created	: 2006/4/12
//
#include "firtex/index/MultiStoredFieldsReader.h"
#include "firtex/index/BarrelsInfo.h"
#include "firtex/store/InputStream.h"
#include "firtex/utility/Number.h"
#include "firtex/utility/StringReader.h"

FX_NS_USE(document);
FX_NS_USE(utility);
FX_NS_USE(store);

FX_NS_DEF(index)

SETUP_STREAM_LOGGER(index, MultiStoredFieldsReader);

MultiStoredFieldsReader::MultiStoredFieldsReader(const BarrelsInfoPtr& pBarrelsInfo)
    : m_pBarrelsInfo(pBarrelsInfo)
{
}

MultiStoredFieldsReader::MultiStoredFieldsReader(const MultiStoredFieldsReader& src)
    : m_pBarrelsInfo(src.m_pBarrelsInfo)
{
    for (size_t i = 0; i < src.m_readers.size(); ++i)
    {
        StoredFieldsReaderPtr pTmp(src.m_readers[i]->clone());
        m_readers.push_back(pTmp);
    }
}

MultiStoredFieldsReader::~MultiStoredFieldsReader(void)
{
    close();
}

bool MultiStoredFieldsReader::getDocument(const FieldSelector& selector, ResultDoc& resultDoc)
{
    docid_t docId = resultDoc.getDocId();
    StoredFieldsReaderPtr& pReader = getReaderByDocId(docId);
	resultDoc.setDocId(docId);
    if (pReader)
    {
        return pReader->getDocument(selector, resultDoc);
    }
    return false;
}

void MultiStoredFieldsReader::open(const InputStreamPoolPtr& pStreamPool,
                                   const std::string& barrel)
{
    FIRTEX_THROW(UnsupportedException, "Unsupport open().");
}

void MultiStoredFieldsReader::close()
{
    for (ReaderVector::iterator it = m_readers.begin(); 
         it != m_readers.end(); ++it)
    {
        (*it)->close();
    }
    m_readers.clear();
}

void MultiStoredFieldsReader::addRead(const StoredFieldsReaderPtr& reader)
{
    m_readers.push_back(reader);
}

StoredFieldsReader* MultiStoredFieldsReader::clone() const
{
    return new MultiStoredFieldsReader(*this);
}

FX_NS_END

