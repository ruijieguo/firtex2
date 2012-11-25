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
// Created	: 2006/4/12
//
#include "firtex/index/StoredFieldsReaderImpl.h"
#include "firtex/document/DocumentSchema.h"
#include "firtex/index/BarrelsInfo.h"
#include "firtex/store/InputStream.h"
#include "firtex/utility/Number.h"
#include "firtex/utility/StringReader.h"
#include "firtex/index/BarrelDirectory.h"

FX_NS_USE(document);
FX_NS_USE(utility);
FX_NS_USE(store);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, StoredFieldsReaderImpl);

StoredFieldsReaderImpl::StoredFieldsReaderImpl(const DocumentSchema* pDocSchema)
    : m_pDocSchema(pDocSchema)
    , m_baseField(-1)
{
    m_sContent.reserve(DEFAULT_READ_BUFFER_SIZE);
}

StoredFieldsReaderImpl::StoredFieldsReaderImpl(const StoredFieldsReaderImpl& src)
    : m_pDocSchema(src.m_pDocSchema)
    , m_baseField(src.m_baseField)
{
    m_sContent.reserve(src.m_sContent.capacity());

    if (src.m_pDataReader.isNotNull())
    {
        m_pDataReader = src.m_pDataReader->clone();
    }
    if (src.m_pIdxReader.isNotNull())
    {
        m_pIdxReader = src.m_pIdxReader->clone();
    }    
}

StoredFieldsReaderImpl::~StoredFieldsReaderImpl()
{
    close();
}

bool StoredFieldsReaderImpl::getDocument(
        const FieldSelector& selector, ResultDoc& resultDoc)
{
    docid_t docId = resultDoc.getDocId();
    m_pIdxReader->seek(docId * sizeof(offset_t));
    offset_t off = m_pIdxReader->readInt64();
    m_pDataReader->seek(off);

    int32_t fieldCount = m_pDataReader->readVInt32();
    if (fieldCount < 0)
    {
        FX_LOG(ERROR, "Field count [%d] in stored fields is invalid", fieldCount);
        return false;
    }

    FieldSelector::Iterator it = selector.iterator();
    int32_t length = 0;
    int32_t readFieldCount = 0;
    fieldid_t fieldId = INVALID_FIELDID;
    while (it.hasNext())
    {
        const FieldSelector::FieldPtr& pField = it.next();
        fieldid_t selectFieldId = pField->getFieldId();
        if (selectFieldId == INVALID_FIELDID)
            continue;

        while (readFieldCount < fieldCount && fieldId < selectFieldId)
        {
            if (length > 0)
            {
                m_pDataReader->seek(m_pDataReader->tell() + length);
            }

            fieldId = (fieldid_t)m_pDataReader->readVInt32();
            length = m_pDataReader->readVInt32();
            ++readFieldCount;
        }

        if (fieldId == selectFieldId)
        {
            m_sContent.clear();
            m_sContent.resize(length);

            m_pDataReader->read((char*)m_sContent.c_str(), length);

            if (pField->hasFilter())
            {
                std::string sNewCont;
                if (pField->filter(m_sContent, sNewCont))
                {
                    resultDoc.addField(pField->getFieldName(), sNewCont);
                    length = 0;
                    continue;
                }
            }
        
            resultDoc.addField(pField->getFieldName(), m_sContent);
            length = 0;
        }
    }

    return true;
}

void StoredFieldsReaderImpl::open(const InputStreamPoolPtr& pStreamPool,
                                  const std::string& sSuffix)
{
    close();

    m_pStreamPool = pStreamPool;
    InputStreamPool::InputStreamPair inStreams = m_pStreamPool->getInputStreams(
            std::make_pair(BarrelDirectory::getFilePath(STORED_FIELDS_VALUE_FILENAME, sSuffix),
                           BarrelDirectory::getFilePath(STORED_FIELDS_IDX_FILENAME, sSuffix)));
    
    m_pDataReader = inStreams.first;
    m_pIdxReader = inStreams.second;

    m_baseField = -1;
}

void StoredFieldsReaderImpl::close()
{
    if (m_pDataReader.isNotNull())
    {
        m_pStreamPool->releaseInputStream(m_pDataReader);
    }
    if (m_pIdxReader.isNotNull())
    {
        m_pStreamPool->releaseInputStream(m_pIdxReader);
    }
}

StoredFieldsReader* StoredFieldsReaderImpl::clone() const
{
    return new StoredFieldsReaderImpl(*this);
}

FX_NS_END

