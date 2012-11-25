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
// Created : 2011-06-25 14:18:08

#ifndef __FX_MULTITYPEDOCUMENTPROCESSOR_H
#define __FX_MULTITYPEDOCUMENTPROCESSOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/processor/DocumentProcessor.h"
#include <map>

FX_NS_DEF(processor);

class MultiTypeDocumentProcessor : public DocumentProcessor
{
public:
    DECLARE_DOCUMENT_PROCESSOR_CREATOR(MultiTypeDocumentProcessor, "multi_type", "*");

public:
    MultiTypeDocumentProcessor();
    MultiTypeDocumentProcessor(const MultiTypeDocumentProcessor& src);
    ~MultiTypeDocumentProcessor();

public:
    /// Initialize
    virtual void init(const FX_NS(document)::DocumentSchema* pDocSchema,
                      const DocumentTemplate* pTemplate = NULL);

    /**
     * Process document
     *
     * @param docSource source document to process
     */
    virtual void process(DocumentSource& docSource);

    /// Clone a new object
    virtual DocumentProcessor* clone() const;

    /**
     * Associate processor to file extension
     *
     * @param sExtension file extension
     * @param pProcessor processor for the file extension
     */
    void addProcessor(const std::string& sExtension, 
                      const DocumentProcessorPtr& pProcessor);

private:
    typedef std::map<std::string, DocumentProcessorPtr> ProcessorMap;
    
    ProcessorMap m_extToProcessor;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_MULTITYPEDOCUMENTPROCESSOR_H
