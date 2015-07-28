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
// Created	: 2011-02-24 22:32:04

#ifndef __FX_ANALYZEDFIELD_H
#define __FX_ANALYZEDFIELD_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/document/Field.h"
#include "firtex/analyzer/TokenView.h"

FX_NS_DEF(document);

class AnalyzedField
{
public:
    typedef FX_NS(analyzer)::TokenView TokenView;
    typedef FX_NS(analyzer)::TokenViewPtr TokenViewPtr;

public:
    AnalyzedField(const FieldSchema* ) : m_pField(NULL) {}
    AnalyzedField(const Field* pField, const TokenViewPtr& pTokenView) 
        : m_pField(pField)
        , m_pTokenView(pTokenView)
    {}

    AnalyzedField() : m_pField(NULL) {}
    ~AnalyzedField() {}

public:
    /**
     * Return field id
     */
    fieldid_t getId() const;

    /**
     * Return original field
     */
    const Field* getField() const;

    /**
     * Return analyzed field (a token view)
     */
    const TokenView* getTokenView() const;

    /**
     * Return field boost
     */
    float getBoost() const;

    /**
     * Set original field
     */
    void setField(const Field* pField);

    /**
     * Set analyzed token view
     */
    void setTokenView(const TokenViewPtr& pTokenView);

    /**
     * Clear for reuse
     */
    void clear();

    /**
     * Return schema of this field
     */
    const FieldSchema* getFieldSchema() const;

private:
    const Field* m_pField;
    TokenViewPtr m_pTokenView;
};

DEFINE_TYPED_PTR(AnalyzedField);

///////////////////////////////////////////////////
//
inline fieldid_t AnalyzedField::getId() const
{
    if (m_pField)
    {
        return m_pField->getId();
    }
    return INVALID_FIELDID;
}

inline const Field* AnalyzedField::getField() const
{
    return m_pField;
}

inline const AnalyzedField::TokenView* AnalyzedField::getTokenView() const
{
    return m_pTokenView.get();
}

inline float AnalyzedField::getBoost() const
{
    return m_pField ? m_pField->getBoost() : 1.0f;
}

inline void AnalyzedField::setField(const Field* pField)
{
    m_pField = pField;
}

inline void AnalyzedField::setTokenView(const TokenViewPtr& pTokenView)
{
    m_pTokenView = pTokenView;
}

inline void AnalyzedField::clear()
{
    m_pField = NULL;

    if (m_pTokenView)
    {
        m_pTokenView->clear();
    }
}

inline const FieldSchema* AnalyzedField::getFieldSchema() const
{
    if (m_pField)
    {
        return m_pField->getFieldSchema();
    }
    return NULL;
}

FX_NS_END

#endif //__FX_ANALYZEDFIELD_H
