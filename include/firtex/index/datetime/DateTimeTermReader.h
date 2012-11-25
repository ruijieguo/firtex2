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
// Created	: 2010-08-14 16:02:39

#ifndef __FX_DATETIMETERMREADER_H
#define __FX_DATETIMETERMREADER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/InputStreamPool.h"
#include "firtex/index/common/IndexKeyHasher.h"
#include "firtex/index/common/TypedTermReader.h"

FX_NS_DEF(index);

class DateTimeTermReader : public TypedTermReader<int64_t, DateTimeHasher<int64_t> >
{
public:
    DateTimeTermReader();
    DateTimeTermReader(const DateTimeTermReader& src);
    ~DateTimeTermReader();

protected:
    /**
     * Create term iterator
     *
     * @param pLowerTerm lower bound term, inclusive
     * @param pUpperTerm upper bound term, inclusive
     * 
     * @return Term iterator instance
     */
    virtual TermIteratorPtr createTermIterator(
            const FX_NS(store)::InputStreamPoolPtr& pStreamPool,
            const Term* pLowerTerm, const Term* pUpperTerm) const;

    /**
     * Create term posting iterator
     * 
     * @param pStreamPool stream pool
     * @param nTermOffset term offset in doc-list file
     *
     * @return Term posting iterator instance
     */
    virtual TermPostingIteratorPtr createTermPostingIterator(
            const FX_NS(store)::InputStreamPoolPtr& pStreamPool,
            offset_t nTermOffset) const;

public:
    /**
     * Clone a new object
     */
    TermReader* clone() const;

private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(DateTimeTermReader);

FX_NS_END

#endif //__FX_DATETIMETERMREADER_H
