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
// Created	: 2006/1/4
//

#include "firtex/index/LengthNormWriter.h"
#include "firtex/utility/NumberFormatter.h"
#include "firtex/store/OutputStream.h"
#include "firtex/index/LengthNormEncoder.h"

FX_NS_USE(store);
FX_NS_USE(utility);
FX_NS_USE(document);

using namespace std;

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, LengthNormWriter);

LengthNormWriter::LengthNormWriter(const FileSystemPtr& pFileSys)
    : m_pFileSys(pFileSys)
{
}

LengthNormWriter::~LengthNormWriter(void)
{
}

void LengthNormWriter::init(const DocumentSchema* pDocSchema)
{
    DocumentSchema::Iterator iter = pDocSchema->iterator();
    while (iter.hasNext())
    {
        const FieldSchema* pFieldSchema = iter.next();
        fieldid_t fieldId = pFieldSchema->getId();
        if (pFieldSchema->isIndexed() && pFieldSchema->hasLengthNorm())
        {
            if (fieldId >= (fieldid_t)m_lengthNorm.size())
            {
                m_lengthNorm.resize((size_t)fieldId + 1);
            }
            m_lengthNorm[fieldId].reset(new LengthNorm(FdIndexType::INT32));
            m_lengthNorm[fieldId]->init(pFieldSchema);
        }
    }
}

void LengthNormWriter::addField(docid_t docId, const AnalyzedField* pField)
{
    fieldid_t fieldId = pField->getId();
    FIRTEX_ASSERT2(m_lengthNorm[fieldId]);

    const AnalyzedField::TokenView* pTokenView = pField->getTokenView();

    int32_t length = (pTokenView == NULL) ? 1 :
                     (int32_t)pTokenView->getNumTokens();
    int32_t norm = LengthNormEncoder::encode(length, pField->getBoost());

    FIRTEX_ASSERT2(docId >= (docid_t)m_lengthNorm[fieldId]->size());
    m_lengthNorm[fieldId]->setValue(docId, norm);
}

void LengthNormWriter::commitBarrel(const std::string& sSuffix)
{
    for (fieldid_t fieldId = 0; fieldId < (fieldid_t)m_lengthNorm.size();
         fieldId++)
    {
        if (m_lengthNorm[fieldId])
        {
            m_lengthNorm[fieldId]->commit(m_pFileSys, NORM_FILEEXT, sSuffix);
        }
    }
    m_lengthNorm.clear();
}

LengthNormIteratorPtr LengthNormWriter::iterator(fieldid_t fieldId) const
{
    if (fieldId > 0 && fieldId < (fieldid_t)m_lengthNorm.size())
    {
        ForwardIndexIteratorPtr pIt = m_lengthNorm[fieldId]->iterator();
        LengthNormIteratorPtr pNormIt = std::dynamic_pointer_cast<LengthNormIterator>(pIt);
        if (!pNormIt)
        {
            FIRTEX_THROW(CastException, "Invalid length norm iterator for field: [%d]",
                         fieldId);
        }
    }
    return LengthNormIteratorPtr();
}

FX_NS_END

