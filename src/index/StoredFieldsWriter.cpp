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
// Created	: 2005/12/9
//
#include "firtex/index/StoredFieldsWriter.h"
#include "firtex/index/StoredFieldsReader.h"
#include "firtex/index/BarrelsInfo.h"
#include "firtex/index/BarrelDirectory.h"
#include "firtex/utility/Exception.h"
#include "firtex/utility/NumberFormatter.h"

using namespace std;

FX_NS_USE(store);
FX_NS_USE(utility);
FX_NS_USE(thread);
FX_NS_USE(document);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, StoredFieldsWriter);

StoredFieldsWriter::StoredFieldsWriter(const FileSystemPtr& pFileSys,
                                       const std::string& sCacheDir)
    : m_pFileSys(pFileSys)
    , m_sCacheDir(sCacheDir)
    , m_bDirty(false)
{
}

StoredFieldsWriter::~StoredFieldsWriter()
{
    if (m_bDirty)
    {
        FX_LOG(WARN, "Stored fields data may be not committed. ");
    }
}

void StoredFieldsWriter::createStreams()
{
    FX_DEBUG("Create stored fields streams: [%s]", m_sCacheDir.c_str());
    m_pValueOutStream = m_pFileSys->createFile(
            BarrelDirectory::getFilePath(STORED_FIELDS_VALUE_FILENAME, m_sCacheDir));
    m_pIdxOutStream = m_pFileSys->createFile(
            BarrelDirectory::getFilePath(STORED_FIELDS_IDX_FILENAME, m_sCacheDir));
}

void StoredFieldsWriter::addDocument(const AnalyzedDocument* pDoc)
{
    fieldid_t fid  = 0;
    int32_t nCount = 0;

    if (!m_pIdxOutStream)
    {
        createStreams();
    }

    //Position of Stored Value
    m_pIdxOutStream->seek(pDoc->getDocId() * sizeof(offset_t));
    FX_TRACE("write stored idx: %lld", m_pValueOutStream->tell());
    m_pIdxOutStream->writeInt64(m_pValueOutStream->tell());

    const AnalyzedField* pField = NULL;
    AnalyzedDocument::Iterator iter = pDoc->iterator();
    while (iter.hasNext())
    {
        pField = iter.next(); 
        const FieldSchema* pFieldSchema = pField->getFieldSchema();
        if (pFieldSchema->isStored())
        {
            ++nCount;
        }
    }

    //Write number of stored field
    m_pValueOutStream->writeVInt32(nCount);

    AnalyzedDocument::Iterator docIter = pDoc->iterator();
    while (docIter.hasNext())
    {
        pField = docIter.next();
        fid = pField->getId();

        const FieldSchema* pFieldSchema = pField->getFieldSchema();
        if (pFieldSchema->isStored())
        {
            m_pValueOutStream->writeVInt32(fid);//write fieldNum

            const Field::String& text = pField->getField()->getValue();
            //Stored Value Length
            m_pValueOutStream->writeVInt32((int32_t)text.length());
            //write field Value
            m_pValueOutStream->write((const void*)text.c_str(), text.length());
        }
    }
    m_bDirty = true;
}

void StoredFieldsWriter::seal()
{
    FX_DEBUG("Seal stored fields: [%s]", m_sCacheDir.c_str());
             
    if (m_pValueOutStream)
    {
        m_pValueOutStream->close();
        m_pValueOutStream.reset();
    }
    if (m_pIdxOutStream)
    {
        m_pIdxOutStream->close();
        m_pIdxOutStream.reset();
    }

    m_bDirty = false;
}

void StoredFieldsWriter::commitBarrel(const string& sPrefix)
{
    FX_DEBUG("Commit stored fields data: [%s]", sPrefix.c_str());
    if (m_pValueOutStream || m_pIdxOutStream)
    {
        seal();
    }

    if (m_pFileSys)
    {
        string sCacheValFile = BarrelDirectory::getFilePath(STORED_FIELDS_VALUE_FILENAME, m_sCacheDir);
        string sCacheIdxFile = BarrelDirectory::getFilePath(STORED_FIELDS_IDX_FILENAME, m_sCacheDir);
        if (m_pFileSys->fileExists(sCacheIdxFile))
        {
            m_pFileSys->renameFile(sCacheValFile, BarrelDirectory::getFilePath(
                            STORED_FIELDS_VALUE_FILENAME, sPrefix));
            m_pFileSys->renameFile(sCacheIdxFile, BarrelDirectory::getFilePath(
                            STORED_FIELDS_IDX_FILENAME, sPrefix));
        }
    }
}

FX_NS_END

