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
// Created	: 2006/6/3
//
#ifndef _BITVECTOR_H
#define _BITVECTOR_H


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/FileSystem.h"
#include "firtex/utility/Exception.h"
#include <stddef.h>
#include <limits.h>
#include <string>
#include <ostream>
#include <istream>

FX_NS_USE(store);

FX_NS_DEF(utility);

const size_t NPOS = (size_t) -1;      // "SIZE_T_MAX"

class BitVector
{
public:
    BitVector();
    BitVector(FX_NS(store)::FileSystemPtr& pFileSys, const tstring& name);
    BitVector(FX_NS(store)::InputStreamPtr& pInStream);
    BitVector(size_t n);
    BitVector(size_t n,uint32_t val);
    BitVector(const std::string& s);
    BitVector(const BitVector& b);
    ~BitVector();

    std::string toString() const;

    BitVector& operator=(const BitVector& b);

    bool operator==(const BitVector&) const;
    bool operator!=(const BitVector&) const;

    BitVector& set(size_t, bool = true);
    BitVector& set();
    BitVector& reset(size_t);
    BitVector& reset();
    BitVector& toggle(size_t);
    BitVector& toggle();
    bool test(size_t) const;
    bool any() const;
    bool none() const;
    bool hasSmallThan(size_t val)const;
    bool hasBiggerThan(size_t val)const;
    size_t firstNotSet()const;
    size_t firstSet()const;
    size_t firstNotSet(size_t start)const;
    size_t firstSet(size_t start)const;
    BitVector	operator~() const;

    /**
     * total set bits
     */
    size_t count() const;

    void read(FX_NS(store)::FileSystemPtr& pFileSys, const tstring& name);
    void write(FX_NS(store)::FileSystemPtr& pFileSys, const tstring& name);

    void read(FX_NS(store)::InputStreamPtr& pInStream);
    void write(FX_NS(store)::OutputStreamPtr& pOutStream);

    // Bitwise operators
    BitVector& operator&=(const BitVector& b);
    BitVector& operator|=(const BitVector& b);
    BitVector& operator^=(const BitVector& b);
    BitVector& operator>>=(size_t n);
    BitVector& operator<<=(size_t n);
    BitVector  operator>>(size_t n) const;
    BitVector  operator<<(size_t n) const;

    // String operations
    BitVector& operator+=(const BitVector&);
    BitVector& insert(size_t pos, const BitVector& b);
    BitVector& remove(size_t pos, size_t n);
    BitVector& replace(size_t pos, size_t n, const BitVector& b);
    size_t find(int val, size_t pos = 0) const;
    size_t rfind(int val, size_t pos = NPOS) const;
    BitVector substr(size_t pos, size_t n) const;

    /**
     * length of the vector
     */
    size_t length() const;

    /**
     * set capacity
     * @param n bit count
     * @param val init value
     */
    size_t length(size_t n, bool val = 0);

    /**
     * trim right empty bits
     */
    size_t trim();

private:
    typedef uint32_t Block;

    Block* m_bits;            ///block array
    size_t m_nBits;	      ///bit count
    size_t m_nBlks;	      ///number of blocks in block array
    mutable size_t m_count;   ///total setted bits
    Block	m_cleanmask;  ///clean mask

    enum {BLKSIZ = CHAR_BIT * sizeof(Block)};

    static Block word(size_t b)
        {return (Block)(b / BLKSIZ);}
    static Block offset(size_t b)
        {return (Block)(BLKSIZ - b%BLKSIZ - 1);}
    static Block mask1(size_t b)
        {return Block(1) << offset(b);}
    static Block mask0(size_t b)
        {return ~(Block(1) << offset(b));}
    static size_t nblks(size_t nb)
        {return (nb+BLKSIZ-1) / BLKSIZ;}

    void make_cleanmask();
    void cleanup();
    void set_(size_t n);
    void set_(size_t n, bool val);
    void reset_(size_t b);
    bool test_(size_t b) const;
    void fromString(const std::string& s);
    void init(size_t n);
    void equalize(BitVector& b);

    friend std::istream& operator>>(std::istream& is, BitVector& b);
};

DEFINE_TYPED_PTR(BitVector);

// Global Functions:
std::ostream& operator<<(std::ostream& os, const BitVector& b);
std::istream& operator>>(std::istream& is, BitVector& b);
BitVector operator& (const BitVector& x, const BitVector& y);
BitVector operator|(const BitVector& x, const BitVector& y);
BitVector operator^ (const BitVector& x, const BitVector& y);
BitVector operator+ (const BitVector& x, const BitVector& y);

//////////////////////////////////////////////////////////////////////////
// Inline publics:
inline BitVector::BitVector()
{
    init(0);
}		
inline BitVector::BitVector(size_t n)
{
    init(n);
}

inline BitVector::~BitVector()
{
    delete [] m_bits;
}

inline BitVector& BitVector::toggle(size_t pos)
{
    if(pos == size_t(-1) || pos >= m_nBits)
        throw OutOfRangeException( _T("bool BitVector::test(size_t pos) const") );

    m_bits[word(pos)] ^= mask1(pos);
    m_count = NPOS;
    return *this;
}

inline bool BitVector::test(size_t pos) const
{
    if(pos >= NPOS)
        throw OutOfRangeException( _T("bool BitVector::test(size_t pos) const" ) );
    if(pos >= m_nBits)
        return false;

    return test_(pos);
}

inline BitVector BitVector::operator~() const
{
    BitVector b(*this);
    b.toggle();
    return b;
}

inline bool BitVector::operator!=(const BitVector& b) const
{
    return !operator==(b);
}

inline bool BitVector::none() const
{
    return !any();
}

inline size_t BitVector::length() const
{
    return m_nBits;
}

inline BitVector
operator&(const BitVector& x, const BitVector& y)
{
    BitVector b(x);
    return b &= y;
}

inline BitVector
operator|(const BitVector& x, const BitVector& y)
{
    BitVector b(x);
    return b |= y;
}

inline BitVector
operator^(const BitVector& x, const BitVector& y)
{
    BitVector b(y);
    return b ^= x;
}

inline BitVector BitVector::operator<<(size_t n) const
{
    BitVector r(*this);
    return r <<= n;
}

inline BitVector BitVector::operator>>(size_t n) const
{
    BitVector r(*this);
    return r >>= n;
}

inline BitVector
operator+(const BitVector& b1, const BitVector& b2)
{
    BitVector b(b1);
    return b.operator+=(b2);
}

// Inline privates:
inline void BitVector::make_cleanmask()
{
    m_cleanmask = ~Block(0) << (m_nBlks * BLKSIZ - m_nBits);
}

inline void BitVector::cleanup()
{
// Make sure unused bits don't get set
    if (m_nBits % BLKSIZ)
        m_bits[m_nBlks - 1] &= m_cleanmask;
}
inline void BitVector::set_(size_t b)
{
    m_bits[word(b)] |= mask1(b);
}

inline void BitVector::reset_(size_t b)
{
    m_bits[word(b)] &= mask0(b);
}

inline bool BitVector::test_(size_t b) const
{
    return !!(m_bits[word(b)] & mask1(b));
}

FX_NS_END

#endif
