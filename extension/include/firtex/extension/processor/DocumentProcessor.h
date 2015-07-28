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
// Created : 2011-06-19 00:04:31

#ifndef __FX_DOCUMENTPROCESSOR_H
#define __FX_DOCUMENTPROCESSOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/processor/DocumentSource.h"
#include "firtex/extension/processor/DocumentTemplate.h"
#include "firtex/utility/Singleton.h"
#include <string>

#define DECLARE_DOCUMENT_PROCESSOR_CREATOR(c, id, ext)                  \
    class Creator : public DocumentProcessor::Creator,                  \
                    public FX_NS(utility)::Singleton<c::Creator>        \
    {                                                                   \
    public:                                                             \
        Creator()                                                       \
        {                                                               \
            m_sIdentifier += "." id;                                    \
            m_sDocType = ext;                                           \
        }                                                               \
    public:                                                             \
        virtual ~Creator(){}                                            \
        DocumentProcessor* create() const {return new c;};              \
        DECLARE_LAZY_SINGLETON(c::Creator);                             \
    }

FX_NS_DEF(processor);

class DocumentProcessor
{
public:
    class Creator
    {
    protected:
        Creator() : m_sIdentifier("processor") {}

    public:
        virtual ~Creator(){}

    public:
        /**
         * create a processor object
         * @return create a parser
         */
        virtual DocumentProcessor* create() const = 0;

        /** 
         * get the identifier of processor
         */
        virtual std::string getIdentifier() const {return m_sIdentifier;}

        /**
         * get document type this parser can process
         * @return document type name
         */
        virtual std::string getDocType() const {return m_sDocType;}

    protected:
        std::string m_sIdentifier;
        std::string m_sDocType;
    };

    DEFINE_TYPED_PTR(Creator);

public:
    virtual ~DocumentProcessor() {}

public:
    /// Initialize
    virtual void init(const FX_NS(document)::DocumentSchema* pDocSchema,
                      const DocumentTemplate* pTemplate) = 0;

    /**
     * Process document
     *
     * @param docSource source document to process
     */
    virtual void process(DocumentSource& docSource) = 0;

    /// Clone a new object
    virtual DocumentProcessor* clone() const = 0;
};

DEFINE_TYPED_PTR(DocumentProcessor);

FX_NS_END

#endif //__FX_DOCUMENTPROCESSOR_H
