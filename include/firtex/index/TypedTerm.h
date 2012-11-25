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
#ifndef __FX_TYPEDTERM_H
#define __FX_TYPEDTERM_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Number.h"
#include "firtex/utility/Hash.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/Term.h"
#include <string>
#include <sstream>

FX_NS_DEF(index)

template<typename TermT>
class TypedTerm : public Term
{
public:
    TypedTerm() : m_value((TermT)0) {}
    TypedTerm(const tstring& field, const TermT t, term_t type);
    TypedTerm(const TypedTerm<TermT>& clone);
    virtual ~TypedTerm(){}	

public:
    TermT getValue() const {return m_value;}
    TermT getValue() {return m_value;}
    void setValue(const TermT t) {m_value = t;}
    void copy(const Term* pSrc);
    void copyValue(const Term* pSrc);

    virtual Term* clone() const {return new TypedTerm<TermT>(*this);}
    virtual int32_t compare(const Term* pOther)const;
    virtual uint64_t hashCode() const;
    virtual tstring toString() const;

private:
    TermT m_value;

private:
    DECLARE_STREAM_LOGGER();
};

////////////////////////////////////////////////////////////////////////
///
template<typename TermT>
TypedTerm<TermT>::TypedTerm(const tstring& field, const TermT t, term_t type)
    : Term(field)
    , m_value(t)		
{
    setType(type);
}

template<typename TermT>
TypedTerm<TermT>::TypedTerm(const TypedTerm<TermT>& clone)
    : Term(clone)
    , m_value(clone.m_value)
{
    setType(clone.getType());
}

template<typename TermT>
void TypedTerm<TermT>::copy(const Term* pSrc)
{
    m_sField = pSrc->getField();
    m_sValue = pSrc->stringValue();
    m_value = pSrc->cast<TermT>()->m_value;
}

template<typename TermT>
void TypedTerm<TermT>::copyValue(const Term* pSrc)
{
    m_sValue = pSrc->stringValue();
    m_value = pSrc->cast<TermT>()->m_value;
}

template<typename TermT>
int32_t TypedTerm<TermT>::compare(const Term* pOther) const
{
    uint64_t h1 = hashCode();
    uint64_t h2 = pOther->hashCode();
    if (h1 == h2)
    {
        return 0;
    }
    else if (h1 > h2)
    {
        return 1;
    }
    return -1;
}

template<typename TermT>
uint64_t TypedTerm<TermT>::hashCode() const
{
    uint64_t hashVal1 = FX_NS(utility)::Hash::hseihHash(
            m_sField.c_str(), m_sField.length());
    uint64_t hashVal2 = FX_NS(utility)::Hash::hseihHash(
            (const char*)&m_value, sizeof(TermT));
    return ((hashVal1 << 32) | hashVal2);
}

template <>
inline uint64_t TypedTerm<uint64_t>::hashCode() const
{
    uint64_t hashVal1 = FX_NS(utility)::Hash::hseihHash(
            m_sField.c_str(), m_sField.length());
    return hashVal1 + m_value;
}

template <>
inline uint64_t TypedTerm<uint32_t>::hashCode() const
{
    uint64_t hashVal1 = FX_NS(utility)::Hash::hseihHash(
            m_sField.c_str(), m_sField.length());
    return hashVal1 + m_value;
}

template <>
inline uint64_t TypedTerm<int32_t>::hashCode() const
{
    uint64_t hashVal1 = FX_NS(utility)::Hash::hseihHash(
            m_sField.c_str(), m_sField.length());
    return hashVal1 + (uint64_t)m_value;
}

template <>
inline uint64_t TypedTerm<int64_t>::hashCode() const
{
    uint64_t hashVal1 = FX_NS(utility)::Hash::hseihHash(
            m_sField.c_str(), m_sField.length());
    return hashVal1 + (uint64_t)m_value;
}

template<typename TermT>
tstring TypedTerm<TermT>::toString() const 
{
    std::stringstream ss;
    ss << m_sField.c_str() << ": " << m_value;
    return ss.str();
}

typedef TypedTerm<int32_t> Int32Term;
typedef FX_NS(common)::SharedPtr<Int32Term> Int32TermPtr;

typedef TypedTerm<int64_t> Int64Term;
typedef FX_NS(common)::SharedPtr<Int64Term> Int64TermPtr;

typedef TypedTerm<uint32_t> UInt32Term;
typedef FX_NS(common)::SharedPtr<UInt32Term> UInt32TermPtr;

typedef TypedTerm<uint64_t> UInt64Term;
typedef FX_NS(common)::SharedPtr<UInt64Term> UInt64TermPtr;

FX_NS_END

#endif
