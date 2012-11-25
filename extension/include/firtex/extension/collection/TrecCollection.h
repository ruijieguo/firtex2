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
// Created : 2011-06-30 23:37:50

#ifndef __FX_TRECCOLLECTION_H
#define __FX_TRECCOLLECTION_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/collection/Collection.h"
#include "firtex/extension/processor/DocumentProcessor.h"
#include "firtex/extension/collection/StandardCollection.h"

FX_NS_DEF(collection);

class TrecCollection : public StandardCollection
{
public:
    DECLARE_COLLECTION_CREATOR(TrecCollection, "trec");

public:
    TrecCollection();
    ~TrecCollection();

public:
    /**
     * Create document processor for processing raw document. 
     * Implement by derived class.
     * 
     * @return document processor object, caller the ownership.
     */
    virtual FX_NS(processor)::DocumentProcessor* createDocumentProcessor();

private:
    std::string m_sDocType;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_TRECCOLLECTION_H
