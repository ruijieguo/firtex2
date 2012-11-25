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
// Email	: ruijieguo@software.ict.ac.cn
// Created	: 2007/10/03
//

#ifndef _MULTIINDEXREADER_H
#define _MULTIINDEXREADER_H

#include "firtex/index/IndexReader.h"
#include "firtex/common/SharedPtr.h"
#include <vector>

FX_NS_DEF(index)

class Index;
class MultiIndex;

class MultiIndexReader :  public IndexReader
{
public:
    typedef std::vector<Index*> IndexVector;

public:
    MultiIndexReader(MultiIndex* pIndex);
    virtual ~MultiIndexReader(void);

protected:
    /**
     * Open index in READ mode 
     */
    void open();

protected:
    /**
     * create barrel reader
     */			
    virtual void createBarrelReader();

private:
    MultiIndex* m_pIndex;
    BarrelsInfoPtr m_pBarrelsInfo;

private:
    DECLARE_STREAM_LOGGER();
};

typedef FX_NS(common)::SharedPtr<MultiIndexReader> MultiIndexReaderPtr;

FX_NS_END

#endif
