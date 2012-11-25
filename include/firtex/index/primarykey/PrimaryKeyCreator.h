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
// Email	: ruijieguo@gmail.com
// Created	: 2006/11/13
//

#ifndef __PRIMARYKEYCREATOR_H
#define __PRIMARYKEYCREATOR_H

#include "firtex/index/FieldComponentCreator.h"
#include "firtex/utility/Singleton.h"

FX_NS_DEF(index);

class PrimaryKeyCreator : public FieldComponentCreator ,
                           public FX_NS(utility)::Singleton<PrimaryKeyCreator>
{		
protected:
    PrimaryKeyCreator(void);
    virtual ~PrimaryKeyCreator(void);

public:
    /** 
     * interface for creating indexer component
     *
     * @param pFieldSchema field info for indexer
     * @return field indexer component
     */
    virtual FieldIndexer* createIndexer(
            const FX_NS(document)::FieldSchema* pFieldSchema) const;

    /** 
     * interface for creating merger component	 
     * @return field merger component
     */
    virtual FieldMerger* createMerger() const;

    /** 
     * interface for creating term reader component	 
     * @return term reader component
     */
    virtual TermReader* createTermReader() const;

protected:
    DECLARE_LAZY_SINGLETON(PrimaryKeyCreator);

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif
