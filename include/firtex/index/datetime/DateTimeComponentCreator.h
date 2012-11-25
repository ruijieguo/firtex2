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
// Created	: 2010-10-17 16:08:12

#ifndef __FX_DATETIMECOMPONENTCREATOR_H
#define __FX_DATETIMECOMPONENTCREATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/FieldComponentCreator.h"
#include "firtex/utility/Singleton.h"

FX_NS_DEF(index);

class DateTimeComponentCreator : public FieldComponentCreator,
                                 public FX_NS(utility)::Singleton<DateTimeComponentCreator>
{
public:
    DateTimeComponentCreator();
    ~DateTimeComponentCreator();

public:
    /** 
     * interface for creating indexer component
     *
     * @param pFieldSchema field schema for indexer
     * @return field indexer component
     */
    FieldIndexer* createIndexer(
            const FX_NS(document)::FieldSchema* pFieldSchema) const;

    /** 
     * interface for creating merger component	 
     * @return field merger component
     */
    FieldMerger* createMerger() const;

    /** 
     * interface for creating term reader component	 
     * @return term reader component
     */
    TermReader* createTermReader() const;

protected:
    DECLARE_LAZY_SINGLETON(DateTimeComponentCreator);

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_DATETIMECOMPONENTCREATOR_H
