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
// Created	: 2005/11/20
//
#include "firtex/document/Field.h"
#include "firtex/utility/StringUtils.h"

FX_NS_USE(common);
FX_NS_USE(utility);
FX_NS_DEF(document);

Field::Field(const FieldSchema* pFieldSchema, const char* value)
    : m_pFieldSchema(pFieldSchema)
    , m_fBoost(1.0)
{
    FIRTEX_ASSERT2(m_pFieldSchema != NULL);
    if (!value)
    {
        throw IllegalArgumentException(_T("Value cannot be null."));
    }

    setValue(value, FX_NS(utility)::strLength(value));
}	

Field::Field(const FieldSchema* pFieldSchema, char* text, size_t len, bool bNoCopy)
    : m_pFieldSchema(pFieldSchema)
    , m_fBoost(1.0)
{
    FIRTEX_ASSERT2(m_pFieldSchema != NULL);
    if (text == NULL)
    {
        throw IllegalArgumentException(_T("Reader cannot be null."));
    }
    setValue(text, len, bNoCopy);
}

Field::Field(const FieldSchema* pFieldSchema, Field::TokenView* value)
    : m_pFieldSchema(pFieldSchema)
    , m_fBoost(1.0)
{
    FIRTEX_ASSERT2(m_pFieldSchema != NULL);
    if (value == NULL)
    {
        throw IllegalArgumentException(_T("Value cannot be null."));
    }

    setTokenView(value);
}

Field::Field(const FieldSchema* pFieldSchema,
             const Field::TokenViewPtr& value)
    : m_pFieldSchema(pFieldSchema)
    , m_fBoost(1.0)
{
    FIRTEX_ASSERT2(m_pFieldSchema != NULL);

    if (!value)
    {
        FIRTEX_THROW(IllegalArgumentException, _T("Value cannot be null."));
    }
        
    setTokenView(value);
}

Field::Field(const Field& clone)
{
    m_pFieldSchema = clone.m_pFieldSchema;

    m_value = clone.m_value;
    m_fBoost = clone.m_fBoost;
	
    m_pTokenView = clone.m_pTokenView;
}		

Field::Field(const FieldSchema* pFieldSchema)
    : m_pFieldSchema(pFieldSchema)
    , m_fBoost(1.0)
{
    FIRTEX_ASSERT2(m_pFieldSchema != NULL);
}

Field::~Field()
{
}

void Field::clear()
{
    if (m_pTokenView)
    {
        m_pTokenView->clear();
    }
}

FX_NS_END
