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
// Created	: 2005/12/20
//
#include "firtex/index/Term.h"
#include "firtex/utility/Exception.h"
#include "firtex/utility/Hash.h"
#include "firtex/index/TypedTerm.h"

using namespace std;

FX_NS_USE(utility);

FX_NS_DEF(index);

Term::Term(const string& field) 
    : m_sField(field)
    , m_type(Term::TEXT)
{					
}

Term::Term(const string& field,const string& value)
    : m_sField(field)
    , m_sValue(value)
    , m_type(Term::TEXT)
{
}

Term::Term(const Term& term)
{
    m_sField = term.m_sField;
    m_sValue = term.m_sValue;
    m_type = term.m_type;
}

Term::Term()
    : m_type(Term::TEXT)
{
}

Term::~Term(void)
{
}

int32_t Term::compare(const Term* pOther) const
{
    int32_t ret = strCompare(m_sField.c_str(), pOther->m_sField.c_str());
    if (!ret)
    {
        return (int32_t)strCompare(m_sValue.c_str(), pOther->m_sValue.c_str());
    }
    return ret;
}

Term* Term::clone() const
{
    return new Term(*this);
}		

void Term::copy(const Term* pSrc)
{
    m_sField = pSrc->m_sField;
    m_sValue = pSrc->m_sValue;
}

void Term::copyValue(const Term* pSrc)
{
    m_sValue = pSrc->m_sValue;
}

string Term::toString() const 
{
    stringstream ss;
    ss << m_sField.c_str() << ": ";
    ss << m_sValue;
    return ss.str();
}

uint64_t Term::hashCode() const
{
    uint64_t hashVal1 = Hash::hseihHash(m_sField.c_str(), m_sField.length());
    uint64_t hashVal2 = Hash::hseihHash(m_sValue.c_str(), m_sValue.length());
    return ((hashVal1 << 32) | hashVal2);
}

template<>
TypedTerm<int8_t>* Term::cast()
{
    return dynamic_cast<TypedTerm<int8_t>*>(this);
}

template<>
const TypedTerm<int8_t>* Term::cast() const
{
    return dynamic_cast<const TypedTerm<int8_t>*>(this);
}

template<>
TypedTerm<uint8_t>* Term::cast()
{
    return dynamic_cast<TypedTerm<uint8_t>*>(this);
}

template<>
const TypedTerm<uint8_t>* Term::cast() const
{
    return dynamic_cast<const TypedTerm<uint8_t>*>(this);
}

template<>
TypedTerm<int16_t>* Term::cast()
{
    return dynamic_cast<TypedTerm<int16_t>*>(this);
}

template<>
const TypedTerm<int16_t>* Term::cast() const
{
    return dynamic_cast<const TypedTerm<int16_t>*>(this);
}

template<>
TypedTerm<uint16_t>* Term::cast()
{
    return dynamic_cast<TypedTerm<uint16_t>*>(this);
}

template<>
const TypedTerm<uint16_t>* Term::cast() const
{
    return dynamic_cast<const TypedTerm<uint16_t>*>(this);
}

template<>
TypedTerm<int32_t>* Term::cast()
{
    return dynamic_cast<TypedTerm<int32_t>*>(this);
}

template<>
const TypedTerm<int32_t>* Term::cast() const
{
    return dynamic_cast<const TypedTerm<int32_t>*>(this);
}

template<>
TypedTerm<uint32_t>* Term::cast()
{
    return dynamic_cast<TypedTerm<uint32_t>*>(this);
}

template<>
const TypedTerm<uint32_t>* Term::cast() const
{
    return dynamic_cast<const TypedTerm<uint32_t>*>(this);
}

template<>
TypedTerm<int64_t>* Term::cast()
{
    return dynamic_cast<TypedTerm<int64_t>*>(this);
}

template<>
const TypedTerm<int64_t>* Term::cast() const
{
    return dynamic_cast<const TypedTerm<int64_t>*>(this);
}

template<>
TypedTerm<uint64_t>* Term::cast()
{
    return dynamic_cast<TypedTerm<uint64_t>*>(this);
}

template<>
const TypedTerm<uint64_t>* Term::cast() const
{
    return dynamic_cast<const TypedTerm<uint64_t>*>(this);
}

template<>
const TypedTerm<uint128_t>* Term::cast() const
{
    return dynamic_cast<const TypedTerm<uint128_t>*>(this);
}

FX_NS_END

