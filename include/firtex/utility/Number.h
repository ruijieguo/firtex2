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
// Email	: ruijieguo@software.ict.ac.cn,ruijieguo@gmail.com
// Created	: 2006/5/20
//
#ifndef __NUMBER_H
#define __NUMBER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/utility/Exception.h"
#include "firtex/common/Any.h"
#include <string>
#include <sstream>

FX_NS_DEF(utility);

class Number
{
public:			
	Number();
	Number(int8_t i8v);
	Number(uint8_t ui8v);
	Number(int16_t i16v);
	Number(uint16_t ui16v);
	Number(int32_t i32v);
	Number(uint32_t ui32v);
	Number(int64_t i64v);
	Number(uint64_t ui64v);
	Number(float f);
	Number(double d);
	Number(const Number& clone);
	~Number(void);
public:
    tstring toString();
    tstring toString(int nWidth);
    tstring toString(int nWidth, int precision);

public:
	operator int8_t() const ;                          // Extracts a int8_t from a Number::INT8
	operator uint8_t() const ;                         // Extracts a uint8_t from a Number::UINT8
	operator int16_t() const ;                         // Extracts a int16_t from a Number::INT16
	operator uint16_t() const ;                        // Extracts a uint16_t from a Number::UINT8
	operator int32_t() const ;                         // Extracts a int32_t from a Number::INT32
	operator uint32_t() const ;                        // Extracts a uint32_t from a Number::UINT32
	operator int64_t() const ;                         // Extracts a int64_t from a Number::INT64
	operator uint64_t() const ;                        // Extracts a uint64_t from a Number::UINT64
	operator float() const ;                           // Extracts a float from a VT_R4
	operator double() const ;                          // Extracts a double from a VT_R8		

	Number& operator=(int8_t i8Src) ;					// Assign a VT_I1
	Number& operator=(uint8_t ui8Src) ;				// Assign a VT_UI1
	Number& operator=(int16_t i8Src) ;					// Assign a VT_I2
	Number& operator=(uint16_t ui16Src) ;				// Assign a VT_UI2
	Number& operator=(int32_t i32Src) ;				// Assign a VT_I4
	Number& operator=(uint32_t ui32Src) ;				// Assign a VT_UI4
	Number& operator=(int64_t i64Src) ;				// Assign a VT_I8
	Number& operator=(uint64_t ui64Src) ;				// Assign a VT_UI8
	Number& operator=(float fltSrc) ;					// Assign a VT_R4
	Number& operator=(double dblSrc) ;					// Assign a VT_R8, or a VT_DATE			

	bool operator == (const Number& pSrc) const throw();
	bool operator != (const Number& pSrc) const throw();

private:
    FX_NS(common)::Any m_anyValue;
};
//////////////////////////////////////////////////////////////////////////
//inline functions	
inline Number::Number(int8_t i8v)
{
    m_anyValue = i8v;
}
inline Number::Number(uint8_t ui8v)
{
    m_anyValue = ui8v;
}
inline Number::Number(int16_t i16v)
{
    m_anyValue = i16v;
}
inline Number::Number(uint16_t ui16v)
{
    m_anyValue = ui16v;
}
inline Number::Number(int32_t i32v)
{
    m_anyValue = i32v;
}
inline Number::Number(uint32_t ui32v)
{
    m_anyValue = ui32v;
}
inline Number::Number(int64_t i64v)
{
    m_anyValue = i64v;
}
inline Number::Number(uint64_t ui64v)
{
    m_anyValue = ui64v;
}
inline Number::Number(float f)
{
    m_anyValue = f;
}
inline Number::Number(double d)
{
    m_anyValue = d;
}

inline Number::Number(const Number& clone)
{
	m_anyValue = clone.m_anyValue;
}

inline Number::~Number(void){}

inline Number::operator int8_t() const 
{
	return FX_NS(common)::AnyCastNumber<int8_t>(m_anyValue);
}

inline Number::operator uint8_t() const 
{
    return FX_NS(common)::AnyCastNumber<uint8_t>(m_anyValue);
}

inline Number::operator int16_t() const
{
    return FX_NS(common)::AnyCastNumber<int16_t>(m_anyValue);
}

inline Number::operator uint16_t() const
{
    return FX_NS(common)::AnyCastNumber<uint16_t>(m_anyValue);
}

inline Number::operator int32_t() const
{
    return FX_NS(common)::AnyCastNumber<int32_t>(m_anyValue);
}

inline Number::operator uint32_t() const
{
    return FX_NS(common)::AnyCastNumber<uint32_t>(m_anyValue);
}

inline Number::operator int64_t() const
{
    return FX_NS(common)::AnyCastNumber<int64_t>(m_anyValue);
}

inline Number::operator uint64_t() const
{
    return FX_NS(common)::AnyCastNumber<uint64_t>(m_anyValue);
}

inline Number::operator float() const
{
    return FX_NS(common)::AnyCastNumber<float>(m_anyValue);
}

inline Number::operator double() const
{
    return FX_NS(common)::AnyCastNumber<double>(m_anyValue);
}

//////////////////////////////////////////////////////////////////////////
inline Number& Number::operator=(int8_t i8Src)
{
    m_anyValue = i8Src;
	return *this;
}

inline Number& Number::operator=(uint8_t ui8Src)
{
    m_anyValue = ui8Src;
	return *this;
}

inline Number& Number::operator=(int16_t i8Src)
{
    m_anyValue = i8Src;
	return *this;
}

inline Number& Number::operator=(uint16_t ui16Src)
{
    m_anyValue = ui16Src;
	return *this;
}

inline Number& Number::operator=(int32_t i32Src)
{
    m_anyValue = i32Src;
	return *this;
}

inline Number& Number::operator=(uint32_t ui32Src)
{
    m_anyValue = ui32Src;
	return *this;
}

inline Number& Number::operator=(int64_t i64Src)
{
    m_anyValue = i64Src;
	return *this;
}

inline Number& Number::operator=(uint64_t ui64Src)
{
    m_anyValue = ui64Src;
	return *this;
}

inline Number& Number::operator=(float fltSrc)
{
    m_anyValue = fltSrc;
	return *this;
}

inline Number& Number::operator=(double dblSrc)
{
    m_anyValue = dblSrc;
	return *this;
}

//////////////////////////////////////////////////////////////////////////
#define FX_IF_EQUAL(tp)                                             \
    if (type == typeid(tp))                                         \
    {                                                               \
        return (FX_NS(common)::AnyCastNumber<tp>(m_anyValue) ==     \
                FX_NS(common)::AnyCastNumber<tp>(pSrc.m_anyValue)); \
    }

inline bool Number::operator == (const Number& pSrc) const throw()
{
    const std::type_info& type = pSrc.m_anyValue.getType();
    if (type != m_anyValue.getType())
    {
        return false;
    }
    FX_IF_EQUAL(int8_t)
    else 
    FX_IF_EQUAL(uint8_t)
    else 
    FX_IF_EQUAL(int16_t)
    else 
    FX_IF_EQUAL(uint16_t)
    else 
    FX_IF_EQUAL(int32_t)
    else 
    FX_IF_EQUAL(uint32_t)
    else 
    FX_IF_EQUAL(int64_t)
    else 
    FX_IF_EQUAL(uint64_t)
    else 
    FX_IF_EQUAL(float)             
    else 
    FX_IF_EQUAL(double)
    return false;        
}


FX_NS_END

#endif

