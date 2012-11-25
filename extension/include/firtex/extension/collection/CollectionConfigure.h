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
// Created : 2011-06-19 00:07:04

#ifndef __FX_COLLECTIONCONFIGURE_H
#define __FX_COLLECTIONCONFIGURE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/config/Configurable.h"
#include "firtex/document/DocumentSchema.h"
#include "firtex/extension/processor/DocumentTemplate.h"

FX_NS_DEF(collection);

class CollectionConfigure : public FX_NS(config)::Configurable
{
public:
    const static int32_t DEFAULT_DOCUMENT_QUEUE_SIZE = 100;
    const static int32_t DEFAULT_PROCESS_THREAD_COUNT = 8;
    
public:
    CollectionConfigure(void);
    CollectionConfigure(const CollectionConfigure& src);
    virtual ~CollectionConfigure(void);

public:
    void configure(FX_NS(config)::Configurator& conf);

public:
    struct sindex
    {
    };
    sindex Index;

    struct sCollection
    {
        sCollection() : inputDocQueueSize((uint32_t)DEFAULT_DOCUMENT_QUEUE_SIZE)
                      , processThreadCount((uint32_t)DEFAULT_PROCESS_THREAD_COUNT)
        {
        }

        std::string name;         ///collection name
        std::string identifier;   ///identifier of collection

        std::string source;       ///index source file path [required]
        std::string target;       ///destination index  [required]
        std::string buildMode;    ///index build mode, values: batch/append[required]

        struct sAutoCommit
        {
            sAutoCommit() : maxDocs(0), maxTime(0) {}

            /// Maximum number of documents to add since the last
            /// commit before automatically triggering a new commit.
            int32_t maxDocs;

            /// Maximum amount of time that is allowed to pass
            /// since a document was added before automatically
            /// triggering a new commit. 
            int32_t maxTime;        
        };

        sAutoCommit autoCommit;

        std::string encoding;     ///encoding of documents
        std::string docTypes;     ///document types, example: txt, html, pdf
        std::string parameter;    ///parameter of collection
        
        uint32_t inputDocQueueSize;  /// Input document queue size
        uint32_t processThreadCount; /// Input document queue size
    };
    sCollection Collection;

public:
    const FX_NS(processor)::DocumentTemplatePtr& getTemplate() const;
    FX_NS(processor)::DocumentTemplatePtr& getTemplate();
    void setDocSchema(const FX_NS(document)::DocumentSchemaPtr& pDocSchema);
    const FX_NS(document)::DocumentSchemaPtr& getDocSchema() const;

public:
    virtual CollectionConfigure* clone() const;

protected:
    FX_NS(processor)::DocumentTemplatePtr m_pTemplate;
    FX_NS(document)::DocumentSchemaPtr m_pDocSchema;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(CollectionConfigure);

////////////////////////////////////////////////////////
///
inline const FX_NS(processor)::DocumentTemplatePtr& CollectionConfigure::getTemplate() const 
{
    return m_pTemplate;
}

inline FX_NS(processor)::DocumentTemplatePtr& CollectionConfigure::getTemplate()
{
    return m_pTemplate;
}

inline void CollectionConfigure::setDocSchema(const FX_NS(document)::DocumentSchemaPtr& pDocSchema)
{
    m_pDocSchema = pDocSchema;
}

inline const FX_NS(document)::DocumentSchemaPtr& CollectionConfigure::getDocSchema() const 
{
    return m_pDocSchema;
}

FX_NS_END

#endif //__FX_COLLECTIONCONFIGURE_H
