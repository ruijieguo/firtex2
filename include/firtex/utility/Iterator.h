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
// Created	: 2006/7/16
//

#ifndef __ITERATOR_H
#define __ITERATOR_H

#include "firtex/common/StdHeader.h"

FX_NS_DEF(utility)

template<class ElemT>
class Iterator
{
public:

    Iterator(void)
    {
    }

    virtual ~Iterator(void)
    {
    }
    typedef ElemT element_type;
public:
    /**
     * reset the iterator
     */
    virtual void reset() = 0;

    /**
     * whether reach to end or not
     * @return true for not, false for yes
     */
    virtual bool hasNext() const = 0;

    /**
     * retrieval current element
     * @return current element
     */
    virtual ElemT next() = 0;

    /**
     * number of elements
     * @return number of elements
     */
    virtual size_t size()const= 0;
};

FX_NS_END

#endif
