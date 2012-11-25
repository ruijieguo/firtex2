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
#ifndef _FIELD_H
#define _FIELD_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/analyzer/TokenView.h"
#include "firtex/utility/String.h"
#include "firtex/common/Any.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/document/FieldSchema.h"

FX_NS_DEF(document);

//////////////////////////////////////////////////////////////////////////
//Field
class Field
{
public:
    typedef FX_NS(utility)::StringBase<char> String;
    typedef FX_NS(analyzer)::TokenView TokenView;
    typedef FX_NS(analyzer)::TokenViewPtr TokenViewPtr;
    
public:
    /**
     * @param pFieldSchema field type
     * @param value text value
     */ 
    Field(const FieldSchema* pFieldSchema, const char* value);

    /** 
     * @param pFieldSchema field type
     * @param value text value
     * @param length length of text value
     * @param bNoCopy copy text value or not
     */ 
    Field(const FieldSchema* pFieldSchema, char* text, size_t length, bool bNoCopy);

    /**
     * @param pFieldSchema field type
     * @param value TokenView value
     */ 
    Field(const FieldSchema* pFieldSchema, Field::TokenView* value);

    /**
     * @param pFieldSchema field type
     * @param value TokenView value
     */ 
    Field(const FieldSchema* pFieldSchema, const Field::TokenViewPtr& value);

    /**
     * copy constructor
     */
    Field(const Field& clone);

    /**
     * constructor an empty field
     * @param pFieldSchema field type
     */
    Field(const FieldSchema* pFieldSchema);

    virtual ~Field();

public:
    fieldid_t getId() const {return m_pFieldSchema->getId();}

    float getBoost() const {return m_fBoost;}
    void setBoost(float fBoost) {m_fBoost = fBoost;}

    inline const String& getValue() const;
    inline String& getValue();

    inline Field::TokenViewPtr getTokenView() const;

    void setValue(const std::string& text);
    void setValue(const char* text);
    void setValue(const char* text, size_t length);
    void setValue(char* text, size_t len, bool bNoCopy);

    void setTokenView(Field::TokenView* tokens);
    void setTokenView(const Field::TokenViewPtr& pTokens);

    void clear();

    /** 
     * clone a new  field object
     * @return a new cloned field object
     */
    Field* clone() const {return new Field(*this);}

    const FieldSchema* getFieldSchema() const {return m_pFieldSchema;}
    
private:
    const FieldSchema* m_pFieldSchema;

    String m_value;
    float m_fBoost;

    Field::TokenViewPtr m_pTokenView; ///analyzed tokens
};

DEFINE_TYPED_PTR(Field);

//////////////////////////////////////////////////////////////////////////
//Inline functions
inline const Field::String& Field::getValue() const
{
    return m_value;
}

inline Field::String& Field::getValue()
{
    return m_value;
}

inline Field::TokenViewPtr Field::getTokenView() const
{
    return m_pTokenView;
}

inline void Field::setValue(const std::string& text)
{
    m_value.assign(text.c_str(), text.length());
}

inline void Field::setValue(const char* text)
{
    m_value.assign(text, FX_NS(utility)::strLength(text));
}

inline void Field::setValue(const char* text, size_t length)
{
    m_value.assign(text, length);
}

inline void Field::setValue(char* text, size_t len, bool bNoCopy)
{
    m_value.assign(text, len, bNoCopy);
}

inline 	void Field::setTokenView(Field::TokenView* pTokens)
{
    m_pTokenView.assign(pTokens);
}

inline void Field::setTokenView(const Field::TokenViewPtr& pTokens)
{
    m_pTokenView = pTokens;
}

FX_NS_END

#endif
