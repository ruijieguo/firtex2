//
// Copyright(C) 2005--2010 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2010-08-27 22:57:40

#ifndef __FX_TERMITERATOR_H
#define __FX_TERMITERATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/Term.h"
#include "firtex/index/TermPostingIterator.h"

FX_NS_DEF(index);

class TermIterator
{
public:
    struct TermEntry
    {
        TermEntry() : term(NULL) {}

        TermEntry(const Term* t, const TermPostingIteratorPtr& it)
            : term(t), postingIterator(it)
        {
        }

        const Term* term;
        TermPostingIteratorPtr postingIterator;
    };

public:
    TermIterator() {}
    virtual ~TermIterator() {}

public:
    /**
     * Check if the iterator has next element or not
     */
    virtual bool hasNext() = 0;
    
    /**
     * Return the next element
     * Example code:
     * 
     *   while (hasNext())
     *   {
     *      TermEntry t = next();
     *   }
     */
    virtual TermEntry next() = 0;

    /**
     * Return number of terms
     */
    virtual int64_t size() const = 0;
};

DEFINE_TYPED_PTR(TermIterator);

FX_NS_END

#endif //__FX_TERMITERATOR_H
