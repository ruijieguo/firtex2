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
#include "firtex/index/LengthNormReader.h"
#include "firtex/utility/NumberFormatter.h"
#include "firtex/index/LengthNormWriter.h"

FX_NS_USE(store);
FX_NS_USE(utility);
FX_NS_USE(document);
using namespace std;

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, LengthNormReader);

LengthNormReader::LengthNormReader()
{
}

LengthNormReader::LengthNormReader(const FileSystemPtr& pFileSys)
    : m_pFileSys(pFileSys)
{
}

LengthNormReader::~LengthNormReader(void)
{
    m_pFileSys.reset();
}

void LengthNormReader::init(const std::string& sSuffix,
                            const DocumentSchema* pDocSchema)
{
    m_pDocSchema = pDocSchema;

    DocumentSchema::Iterator iter = pDocSchema->iterator();
    while (iter.hasNext())
    {
        const FieldSchema* pFieldSchema = iter.next();
        if (pFieldSchema->hasForwardIndex() && pFieldSchema->hasLengthNorm())
        {
            fieldid_t fieldId = pFieldSchema->getId();
            if (fieldId >= (fieldid_t)m_lengthNorms.size())
            {
                m_lengthNorms.resize(fieldId + 1);
            }
            m_lengthNorms[fieldId].assign(new LengthNorm(FdIndexType::INT32));
            m_lengthNorms[fieldId]->open(m_pFileSys, pFieldSchema,
                    NORM_FILEEXT, sSuffix);

            ForwardIndexIteratorPtr pIt = m_lengthNorms[fieldId]->iterator();
            LengthNormIteratorPtr pNormIt = pIt.cast<LengthNormIterator>();
            if (!pNormIt)
            {
                FIRTEX_THROW(CastException, "Create length norm iterator of field: [%d] FAILED",
                             fieldId);
            }
            addNormIterator(fieldId, pNormIt);
        }
    }
}

LengthNormIteratorPtr LengthNormReader::lengthNorm(const std::string& sField) const
{
    FIRTEX_ASSERT2(m_pDocSchema != NULL);
    fieldid_t fieldId = m_pDocSchema->getFieldId(sField);
    return lengthNorm(fieldId);
}

LengthNormIteratorPtr LengthNormReader::lengthNorm(fieldid_t fieldId) const
{
    if (fieldId < 0 || fieldId >= (fieldid_t)m_lengthNormIts.size())
    {
        return LengthNormIteratorPtr();
    }
    ForwardIndexIterator* pIt = m_lengthNormIts[fieldId]->clone();
    LengthNormIterator* pNormIt = dynamic_cast<LengthNormIterator*>(pIt);
    if (!pNormIt)
    {
        FIRTEX_THROW(CastException, "Invalid length norm iterator of field: [%d]", fieldId);
    }
    return LengthNormIteratorPtr(pNormIt);
}

const LengthNormReader::LengthNorm*
LengthNormReader::getLengthNorm(const std::string& sField) const
{
    fieldid_t fieldId = m_pDocSchema->getFieldId(sField);
    if (fieldId < 0 || fieldId >= (fieldid_t)m_lengthNorms.size())
    {
        return NULL;
    }
    return m_lengthNorms[fieldId].get();
}

void LengthNormReader::addNormIterator(fieldid_t fieldId, const LengthNormIteratorPtr& pNormIt)
{
    if (fieldId >= (fieldid_t)m_lengthNormIts.size())
    {
        m_lengthNormIts.resize(fieldId + 1);
    }
    m_lengthNormIts[fieldId].assign(pNormIt);
}

FX_NS_END

