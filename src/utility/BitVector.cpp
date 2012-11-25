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
#include "firtex/utility/BitVector.h"
#include "firtex/utility/Exception.h"
#include "firtex/store/InputStream.h"
#include "firtex/store/OutputStream.h"

using namespace std;
FX_NS_USE(store);

FX_NS_DEF(utility);

// Public Functions:
BitVector::BitVector( size_t n,uint32_t val)
{
    if(n<0 || n>= NPOS)
    {
        throw OutOfRangeException( _T("BitVector::BitVector( size_t n,uint32_t val)"));
    }

    // val == 0 is easy...
    if (val == 0)
    {
        init(n);
        return;
    }

    // Find highest significant bit
    uint32_t temp = val;
    size_t high_bit = 0;
    size_t i;
    for (i = 0; temp; ++i)
    {
        if (temp & 1)
            high_bit = i;
        temp >>= 1;
    }

    // Determine m_nBits and construct
    m_nBits = high_bit + 1;
    if (m_nBits < n)
        m_nBits = n;
    init(m_nBits);

    // Store bit pattern
    for (i = 0; i < m_nBits; ++i)
    {
        if (val & 1)
            set_(i);
        val >>= 1;
    }
}
BitVector::BitVector(FileSystemPtr& pFileSys,const tstring& name)
{
    read(pFileSys,name);
}
BitVector::BitVector(InputStreamPtr& pInput)
{
    read(pInput);
}
BitVector::BitVector(const string& s)
{
    // Validate that s has only 0's and 1's
    size_t i;
    for (i = 0; i < s.length(); ++i)
    {
        char c = s.at(i);
        if (c != '0' && c != '1')
            break;
    }
    if(i != s.length())
        throw BadParameterException(_T("BitVector::BitVector(const string& s)"));

    fromString(s);
}

BitVector::BitVector(const BitVector& b)
{
    init(b.m_nBits);
    memcpy(m_bits,b.m_bits,m_nBlks * sizeof m_bits[0]);
}

string BitVector::toString() const
{
    char *s = new char[m_nBits+1];
    for (size_t i = 0; i < m_nBits; ++i)
        s[i] = '0' +test_(i);

    s[m_nBits] = '\0';
    string s2(s);
    delete [] s;
    return s2;
}

BitVector& BitVector::operator=(const BitVector& b)
{
    if (this != &b)
    {
        delete [] m_bits;
        init(b.m_nBits);
        memcpy(m_bits,b.m_bits,m_nBlks * sizeof m_bits[0]);
        m_count = b.m_count;
    }
    return *this;
}

bool BitVector::operator==(const BitVector& b) const
{
    return (m_nBits == b.m_nBits) &&
        !memcmp(m_bits,b.m_bits,m_nBlks * sizeof m_bits[0]);
}

BitVector& BitVector::set()
{
    memset(m_bits,~0u,m_nBlks* sizeof m_bits[0]);
    m_count = m_nBits;
    cleanup();
    return *this;
}

BitVector& BitVector::set(size_t pos, bool val)
{
    if(pos < 0 || pos >= NPOS)
        throw OutOfRangeException( _T("BitVector& BitVector::set(size_t pos, bool val)"));

    if (pos >= m_nBits)
        length(pos + 1); // Append

    set_(pos,val);
    m_count = NPOS;
    return *this;
}

BitVector& BitVector::reset(size_t pos)
{
    if(pos < 0 || pos > m_nBits)
        throw OutOfRangeException( _T("BitVector& BitVector::reset(size_t pos)"));

    if (pos == m_nBits)
        length(m_nBits + 1); // Append

    reset_(pos);
    m_count = NPOS;
    return *this;
}

BitVector& BitVector::reset()
{
    memset(m_bits,0,m_nBlks * sizeof m_bits[0]);
    m_count = 0;
    return *this;
}

BitVector& BitVector::toggle()
{
    size_t nw = m_nBlks;
    while (nw--)
        m_bits[nw] = ~m_bits[nw];
    cleanup();
    m_count = NPOS;
    return *this;
}

bool BitVector::any() const
{
    for (size_t i = 0; i < m_nBlks; ++i)
        if (m_bits[i])
            return true;
    return false;
}


bool BitVector::hasSmallThan(size_t val)const
{
    if(val >= m_nBits)
        return any();
    size_t nTestBlk = nblks(val);
    if(nTestBlk == 0)
        nTestBlk++;
    for (size_t i = 0; i < nTestBlk; ++i)
        if (m_bits[i])
            return true;
    return false;
}
bool BitVector::hasBiggerThan(size_t val)const
{
    if(val >= m_nBits)
        return false;
    size_t nTestBlk = nblks(val);
    if(nTestBlk > 0)
        nTestBlk--;
    for (size_t i = nTestBlk; i < m_nBlks; ++i)
        if (m_bits[i])
            return true;
    return false;
}

size_t BitVector::firstNotSet()const
{
    size_t i;	
    for (i = 0; i < m_nBits; ++i)
    {
        if (!test_(i))
            return i;
    }
    return i;
}

size_t BitVector::firstSet()const
{
    size_t i ;
    for (i = 0; i < m_nBits; ++i)
    {
        if (test_(i))
            return i;
    }
    return i;
}

size_t BitVector::firstNotSet(size_t start)const
{
    size_t i;	
    for (i = start; i < m_nBits; ++i)
    {
        if (!test_(i))
            return i;
    }
    return i;
}

size_t BitVector::firstSet(size_t start)const
{
    size_t i ;
    for (i = start; i < m_nBits; ++i)
    {
        if (test_(i))
            return i;
    }
    return i;
}

size_t BitVector::count() const
{
    if (m_count == NPOS)
    {
        size_t sum = 0;
        for (size_t i = 0; i < m_nBits; ++i)
        {
            if (test_(i))
            {
                ++sum;
            }
        }
        m_count = sum;
    }
    return m_count;
}

BitVector& BitVector::operator&=(const BitVector& b)
{
    BitVector rhs(b);

    equalize(rhs);
    for (size_t i = 0; i < m_nBlks;++i)
        m_bits[i] &= rhs.m_bits[i];
    m_count = NPOS;
    return *this;
}

BitVector& BitVector::operator|=(const BitVector& b)
{
    BitVector rhs(b);

    equalize(rhs);
    for (size_t i = 0; i < m_nBlks; ++i)
        m_bits[i] |= rhs.m_bits[i];
    cleanup();
    m_count = NPOS;
    return *this;
}

BitVector& BitVector::operator^=(const BitVector& b)
{
    BitVector rhs(b);

    equalize(rhs);
    for (size_t i = 0; i < m_nBlks; ++i)
        m_bits[i] ^= rhs.m_bits[i];
    cleanup();
    m_count = NPOS;
    return *this;
}

BitVector& BitVector::operator>>=(size_t n)
{
    if (n >= m_nBits)
        reset();
    else
    {
        size_t i;
        for (i = m_nBits - 1; i >= n; --i)
            (void) set_(i,test_(i-n));
        for (i = 0; i < n;++i)
            reset_(i);
    }
    m_count = NPOS;
    return *this;
}

BitVector& BitVector::operator<<=(size_t n)
{
    if (n >= m_nBits)
        reset();
    else
    {
        size_t i;
        for (i = 0; i < m_nBits - n; ++i)
            (void) set_(i,test_(i+n));
        for (i = m_nBits - n; i < m_nBits; ++i)
            reset_(i);
    }
    m_count = NPOS;
    return *this;
}

BitVector& BitVector::operator+=(const BitVector& b)
{
    if(m_nBits + b.m_nBits >= NPOS)
        throw OutOfRangeException( _T("BitVector& BitVector::operator+=(const BitVector& b)"));

    size_t oldlen = m_nBits;

    length(m_nBits + b.m_nBits);
    for (size_t i = 0; i < b.m_nBits; ++i)
        (void) set_(oldlen+i,b.test_(i));

    m_count = NPOS;
    return *this;
}

BitVector& BitVector::insert(size_t pos, const BitVector& b)
{
    if(pos < 0 || pos > m_nBits)
        throw OutOfRangeException( _T("BitVector& BitVector::insert(size_t pos, const BitVector& b)"));

    size_t oldlen = m_nBits;
    size_t newlen = m_nBits + b.m_nBits;

    // Grow to accommodate insertion
    length(newlen);

    // Move tail to right
    size_t i ;
    for (i = 0; i < oldlen - pos;++i)
        set_(newlen-i-1,test_(oldlen-i-1));

    // Replicate b in between

    for (i = 0; i < b.m_nBits;++i)
        set_(pos+i,b.test_(i));

    m_count = NPOS;
    return *this;
}

BitVector& BitVector::remove(size_t pos, size_t n)
{			
    if(pos < 0 || pos >= m_nBits)
        throw OutOfRangeException( _T("BitVector& BitVector::remove(size_t pos, size_t n)"));

    if (n > m_nBits - pos)
        n = m_nBits - pos;

    // Slide tail down to cover gap
    for (size_t i = 0; i < m_nBits - pos - n; ++i)
        set_(pos+i,test_(pos+n+i));

    // Shrink
    length(m_nBits - n);
    m_count = NPOS;
    return *this;
}

BitVector& BitVector::replace(size_t pos, size_t n,const BitVector& b)
{		

    if(pos < 0 || pos > m_nBits)
        throw OutOfRangeException( _T("BitVector& BitVector::replace(size_t pos, size_t n,const BitVector& b)"));

    if (n > m_nBits - pos)
        n = m_nBits - pos;

    size_t oldlen= m_nBits;
    size_t newlen = oldlen - n + b.m_nBits;
    ptrdiff_t diff = newlen - oldlen;

    // Adjust length and move tail as needed
    if (diff > 0)
    {
        // Grow
        length(newlen);
        for (size_t i = oldlen - 1; i >= pos + n; --i)
            (void) set_(i+diff,test_(i));
    }
    else if (diff < 0)
    {
        // Shrink
        for (size_t i = pos + n; i < oldlen; ++i)
            (void) set_(i+diff,test_(i));
        length(newlen);
    }

    // Copy b into place
    for (size_t i = 0; i < b.m_nBits;++i)
        (void) set_(pos+i,b.test_(i));

    m_count = NPOS;

    return *this;
}

size_t BitVector::find(int val, size_t pos) const
{			
    if(pos < 0 || pos >= m_nBits)
        throw OutOfRangeException( _T("size_t BitVector::find(int val, size_t pos) const"));

    for (size_t i = pos; i < m_nBits; ++i)
    {
        int t = test_(i);
        if ((val && t) || (!val && !t))
            return i;
    }
    return NPOS;
}

size_t BitVector::rfind(int val, size_t pos) const
{			
    if(pos < 0 || pos >= m_nBits)
    {
        FIRTEX_THROW(OutOfRangeException, _T("pos=[%u], size=[%u]"),
                        (uint32_t)pos, (uint32_t)m_nBits);
    }

    for (size_t i = pos; i >= 0; --i)
    {
        int t = test_(i);
        if ((val && t) || (!val && !t))
            return i;
    }
    return NPOS;
}

BitVector BitVector::substr(size_t pos, size_t n) const
{			
    if(pos < 0 || pos > m_nBits)
        throw OutOfRangeException( _T("BitVector BitVector::substr(size_t pos, size_t n) const"));

    if (n > m_nBits - pos)
        n = m_nBits - pos;

    BitVector b(n,0);
    for (size_t i = 0; i < n; ++i)
        b.set_(i,test_(pos + i));
    return b;
}

size_t BitVector::length(size_t n, bool val)
{
    size_t oldlen = m_nBits;
    size_t nw1 = m_nBlks;
    size_t nw2 = nblks(n);

    // Alter the size of a BitVector
    if (nw1 != nw2)
    {
        Block *newbits = new Block[nw2];
        size_t i ;
        for (i = 0; i < nw1 && i < nw2; ++i)
            newbits[i] = m_bits[i];

        delete [] m_bits;
        m_bits = newbits;

        for (i = nw1; i < nw2;++i)
            m_bits[i] = val ? ~Block(0) : Block(0);
        m_nBlks = nw2;
    }
    m_nBits = n;
    make_cleanmask();
    cleanup();
    m_count = NPOS;
    return oldlen;
}

size_t BitVector::trim()
{
    size_t i;
    for (i = m_nBits - 1; i >= 0; --i)
        if (test_(i))
            break;
    size_t newlen = i + 1;
    length(newlen);
    return newlen;
}

ostream& operator<<(ostream& os, const BitVector& b)
{
    os << b.toString();
    return os;
}

istream& operator>>(istream& is, BitVector& b)
{
    const size_t N = 256;
    char *buf = new char[N];
    char c;

    // Consume bit characters
    is >> ws;
    size_t i;
    for (i = 0; i < N && is.get(c); ++i)
    {
        if (c == '0' || c == '1')
            buf[i] = c;
        else
        {
            is.putback(c);
            break;
        }
    }
    buf[i] = '\0';

    if (i == 0)
        is.clear(ios::failbit);
    else
    {
        delete [] b.m_bits;
        b.fromString(string(buf));
    }

    delete buf;
    return is;
}

// Private Functions:
void BitVector::set_(size_t n, bool val)
{
    if (val)
        set_(n);
    else
        reset_(n);
    m_count = NPOS;			
}

void BitVector::fromString(const string& s)
{
    // Assumes string contains only 0's and 1's
    init(s.length());
    for (size_t i = 0; i < s.length(); ++i)
        if (s.at(i) == '1')
            set_(i);
}

void BitVector::init(size_t n)
{
    m_nBits = n;			
    if (n > 0)
    {
        m_nBlks = nblks(n);
        m_bits = new Block[m_nBlks];
        memset(m_bits,0,m_nBlks * sizeof m_bits[0]);
        make_cleanmask();		
    }
    else
    {
        m_bits = 0;
        m_nBlks = 0;
        m_cleanmask = 0;
    }				
    m_count = NPOS;
}

void BitVector::equalize(BitVector& b)
{
    // Make the smaller the size of the larger
    if (b.m_nBits < m_nBits)
        b.length(m_nBits);
    else if (m_nBits < b.m_nBits)
        length(b.m_nBits);
}
void BitVector::read(FileSystemPtr& pFileSys, const tstring& name)
{
    InputStreamPtr pInput = pFileSys->openFile(name);
    try
    {				
        m_nBits = (size_t)pInput->readInt32();	//total bit count	
        init(m_nBits);
        m_count = (size_t)pInput->readInt32();	//set bit count
        pInput->read((char*)m_bits,m_nBlks*sizeof(Block));
    }
    catch (std::bad_alloc& ) 
    {
        FIRTEX_THROW(OutOfMemoryException, ("Allocate memory FAILED"));
    }
    catch (...) 
    {
        FIRTEX_THROW(FirteXException, _T("Unknown exception."));
    }		
}

void BitVector::write(FileSystemPtr& pFileSys, const tstring& name)
{
    OutputStreamPtr pOutput = pFileSys->createFile(name);

    pOutput->writeInt32((int32_t)m_nBits);
    count();
    pOutput->writeInt32((int32_t)m_count);
    pOutput->write((const char*)m_bits,m_nBlks*sizeof(Block));
}

void BitVector::read(InputStreamPtr& pInput)
{
    try
    {				
        m_nBits = (size_t)pInput->readInt32();	//total bit count
        init(m_nBits);
        m_count = (size_t)pInput->readInt32();	//set bit count
        if(m_count > 0)
        {
            pInput->read((void*)m_bits, m_nBlks * sizeof(Block));
        }
    }
    catch (std::bad_alloc& ) 
    {				
        throw OutOfMemoryException(_T(""));
    }
    catch (...) 
    {				
        throw FirteXException(_T("Unknown exception."));
    }		
}

void BitVector::write(OutputStreamPtr& pOutput)
{
    trim();
    pOutput->writeInt32((int32_t)m_nBits);
    count();
    pOutput->writeInt32((int32_t)m_count);
    pOutput->write((const void*)m_bits, m_nBlks * sizeof(Block));
}

FX_NS_END

