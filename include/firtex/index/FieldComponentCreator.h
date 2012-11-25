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
// Email	: ruijieguo@software.ict.ac.cn ruijieguo@gmail.com
// Created	: 2006/6/10
//
#ifndef __FX_FIELDCOMPONENTCREATOR_H
#define __FX_FIELDCOMPONENTCREATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Singleton.h"
#include "FieldMerger.h"
#include "TermReader.h"
#include "firtex/document/Field.h"
#include "firtex/utility/String.h"
#include "firtex/document/FieldSchema.h"
#include <map>

FX_NS_DEF(index);

class FieldComponentCreator
{
public:
    FieldComponentCreator(void)
        : m_identifier(_T("indexer"))
    {}
public:
    virtual ~FieldComponentCreator(void){}

public:
    /**
     * get identifier  
     */
    virtual const tstring& getIdentifier() const { return m_identifier;}

public:
    /** 
     * interface for creating indexer component
     *
     * @param pFieldSchema field schema for indexer
     * @return field indexer component
     */
    virtual FieldIndexer* createIndexer(
            const FX_NS(document)::FieldSchema* pFieldSchema) const = 0;

    /** 
     * interface for creating merger component	 
     * @return field merger component
     */
    virtual FieldMerger* createMerger() const = 0;

    /** 
     * interface for creating term reader component	 
     * @return term reader component
     */
    virtual TermReader* createTermReader() const = 0;

protected:
    tstring m_identifier;
};

FX_NS_END

#endif
