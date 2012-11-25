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
#ifndef __FX_TERM_H
#define __FX_TERM_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/Number.h"
#include "firtex/common/SharedPtr.h"
#include <string>
#include <sstream>

FX_NS_DEF(index);

template<typename R>
class TypedTerm;

class Term
{
public:
    const static int32_t UNKNOWN = 0;
    const static int32_t BOOL	 = 1;
    const static int32_t INT8	 = 2;
    const static int32_t UINT8	 = 3;
    const static int32_t INT16	 = 4;
    const static int32_t UINT16	 = 5;
    const static int32_t INT32	 = 6;
    const static int32_t UINT32	 = 7;
    const static int32_t INT64	 = 8;
    const static int32_t UINT64	 = 9;
 
    const static int32_t FLOAT	 = 10;
    const static int32_t DOUBLE	 = 11;

    const static int32_t TERMID	 = Term::DOUBLE + 1;
    const static int32_t TEXT	 = Term::TERMID + 1;

public:
    Term();
    Term(const std::string& field);
    Term(const std::string& field,const std::string& value);
    Term(const Term& term);
    virtual ~Term(void);

public:
    const std::string& getField() const {return m_sField;}
    void setField(const std::string& sField){m_sField = sField;}

    term_t getType() const {return m_type;};
    void setType(term_t t){m_type = t;}

    inline const std::string& stringValue() const;
    inline void	stringValue(const std::string& value);

    template<typename R>
    TypedTerm<R>* cast();

    template<typename R>
    const TypedTerm<R>* cast() const;

    virtual int32_t compare(const Term* pOther) const;
    virtual Term* clone() const;

    virtual void copy(const Term* pSrc);
    virtual void copyValue(const Term* pSrc);

    virtual std::string toString() const;

    virtual uint64_t hashCode() const;

protected:
    std::string m_sField; ///field name
    std::string m_sValue; ///field string value
    term_t m_type;        ///term type
};

DEFINE_TYPED_PTR(Term);

////////////////////////////////////////////////////////////
///
inline const std::string& Term::stringValue()const
{
    return m_sValue;
}

inline void Term::stringValue(const std::string& value)
{
    m_sValue = value;
}

FX_NS_END

#endif
