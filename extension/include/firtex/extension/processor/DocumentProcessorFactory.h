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
// Created : 2011-06-25 16:21:29

#ifndef __FX_DOCUMENTPROCESSORFACTORY_H
#define __FX_DOCUMENTPROCESSORFACTORY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/Singleton.h"
#include "firtex/extension/processor/DocumentProcessor.h"

FX_NS_DEF(processor);

class DocumentProcessorFactory : public FX_NS(utility)::Singleton<DocumentProcessorFactory>
{
public:
    typedef DocumentProcessor::CreatorPtr CreatorPtr;
public:
    DocumentProcessorFactory();
    ~DocumentProcessorFactory();

public:
    /**
     * Register processor
     * 
     * @param pCreator creator of processor
     */
    void registerProcessor(const CreatorPtr& pCreator);

    /**
     * Create processor by identifier
     */
    DocumentProcessor* createProcessor(const std::string& sIdent) const;

    /// Create processor by file extension
    DocumentProcessor* createProcessorByFileExtension(const std::string& sExtension) const;

private:
    typedef std::map<std::string, DocumentProcessor::CreatorPtr> CreatorMap;

    /// Identifier to processor map
    CreatorMap m_ident2Processor;

    /// file extension to processor map
    CreatorMap m_ext2Processor;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_DOCUMENTPROCESSORFACTORY_H
