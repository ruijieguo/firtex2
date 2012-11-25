//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2011-06-07 19:45:30

#ifndef __FX_DOCUMENTCONSUMER_H
#define __FX_DOCUMENTCONSUMER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(index);

class DocumentConsumer
{
public:
    virtual ~DocumentConsumer() {}

public:
    /// Initialize and start the consumer
    virtual void start() = 0;

    /**
     * Consume document 
     *
     * @param pDoc document to consume
     */
    virtual void consume(const FX_NS(document)::DocumentPtr& pDoc) = 0;

    /**
     * Commit all documents in queue to file system.
     */
    virtual void commit() = 0;

    /// Commit in-memory barrels and Stop all background threads
    virtual void stop() = 0;
};

DEFINE_TYPED_PTR(DocumentConsumer);

FX_NS_END

#endif //__FX_DOCUMENTCONSUMER_H
