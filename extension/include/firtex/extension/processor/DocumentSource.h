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
// Created : 2011-06-19 22:49:55

#ifndef __FX_DOCUMENTSOURCE_H
#define __FX_DOCUMENTSOURCE_H

#include <map>
#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/document/DocumentSchema.h"
#include "firtex/document/Document.h"
#include "firtex/extension/processor/RawDocument.h"

FX_NS_DEF(processor);

class DocumentSource
{
public:
    DocumentSource(const FX_NS(document)::DocumentSchema* pDocSchema)
        : m_pDocSchema(pDocSchema)
        , m_bToBeContinued(false)
    {
        m_lastDocIt = m_documents.end();
    }

    ~DocumentSource() {}

public:
    /// Set raw unparsed document.
    void setRawDocument(const RawDocumentPtr& pRawDoc);

    /// Return raw document
    const RawDocumentPtr& getRawDocument() const;

    /**
     * Add a processed document
     *
     * @param sProccessorName the processor name
     * @param pDoc the document to add
     */
    void addDocument(const std::string& sProccessorName,
                     const FX_NS(document)::DocumentPtr& pDoc);

    /// Return the document of specified processor
    FX_NS(document)::DocumentPtr getDocument(const std::string& sProccessorName) const;

    /**
     * Return the document of specified processor.
     * Create one if no document associated with this process.
     */
    FX_NS(document)::DocumentPtr acquireDocument(const std::string& sProccessorName);

    /// Steal the last processed document
    FX_NS(document)::DocumentPtr stealLastDocument();

    /// True if to be continued
    bool toBeContinued() const;

    /// True if to be continued
    void setToBeContinued(bool bCont = true);

    /**
     * Clear all documents
     */
    void reset();
    
private:
    typedef std::map<std::string, FX_NS(document)::DocumentPtr> DocumentMap;

    DocumentMap m_documents;
    RawDocumentPtr m_pRawDocument;
    DocumentMap::iterator m_lastDocIt;

    const FX_NS(document)::DocumentSchema* m_pDocSchema;

    bool m_bToBeContinued;

private:
    DECLARE_STREAM_LOGGER();
};

///////////////////////////////////////////////////
///

inline void DocumentSource::setRawDocument(const RawDocumentPtr& pRawDoc)
{
    m_pRawDocument = pRawDoc;
}

inline const RawDocumentPtr& DocumentSource::getRawDocument() const
{
    return m_pRawDocument;
}

inline void DocumentSource::addDocument(const std::string& sProccessorName,
                                        const FX_NS(document)::DocumentPtr& pDoc)
{
    DocumentMap::iterator it = m_documents.find(sProccessorName);
    if (it == m_documents.end())
    {
        m_lastDocIt = m_documents.insert(std::make_pair(sProccessorName, pDoc)).first;
    }
}

inline FX_NS(document)::DocumentPtr DocumentSource::getDocument(
        const std::string& sProccessorName) const
{
    DocumentMap::const_iterator it = m_documents.find(sProccessorName);
    if (it != m_documents.end())
    {
        return it->second;
    }
    return FX_NS(document)::DocumentPtr();
}

inline FX_NS(document)::DocumentPtr DocumentSource::acquireDocument(
        const std::string& sProccessorName)
{
    DocumentMap::iterator it = m_documents.find(sProccessorName);
    if (it != m_documents.end())
    {
        m_lastDocIt = it;
        return m_lastDocIt->second;
    }
    FX_NS(document)::DocumentPtr pDoc = new FX_NS(document)::Document(m_pDocSchema);
    addDocument(sProccessorName, pDoc);
    return pDoc;
}

inline bool DocumentSource::toBeContinued() const
{
    return m_bToBeContinued;
}

inline void DocumentSource::setToBeContinued(bool bCont)
{
    m_bToBeContinued = bCont;
}

inline void DocumentSource::reset()
{
    if (m_pRawDocument.isNotNull())
    {
        m_pRawDocument->clear();
    }
    m_lastDocIt = m_documents.end();

    for (DocumentMap::iterator it = m_documents.begin();
         it != m_documents.end(); ++it)
    {
        it->second->clear();
    }
}

inline FX_NS(document)::DocumentPtr DocumentSource::stealLastDocument()
{
    FX_NS(document)::DocumentPtr pDoc;
    if (m_lastDocIt != m_documents.end())
    {
        pDoc = m_lastDocIt->second;
        m_documents.erase(m_lastDocIt);
        m_lastDocIt = m_documents.end();
    }
    return pDoc;
}

FX_NS_END

#endif //__FX_DOCUMENTSOURCE_H
