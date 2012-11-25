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
// Created : 2011-06-25 12:39:24

#ifndef __FX_HTMLDOCUMENTPROCESSOR_H
#define __FX_HTMLDOCUMENTPROCESSOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/processor/PlainDocumentProcessor.h"
#include "firtex/extension/processor/HTMLParser.h"

FX_NS_DEF(processor);

class HTMLDocumentProcessor : public PlainDocumentProcessor
{
public:
    DECLARE_DOCUMENT_PROCESSOR_CREATOR(HTMLDocumentProcessor, "html", "html;htm");

public:
    enum FieldCat
    {
        TITLE = 0,
        BODY,
        FILENAME,
        FILEPATH,
        FILETYPE,
        MODIFIED,
        FILESIZE,
        MAX_FIELD,
    };

public:
    HTMLDocumentProcessor();
    HTMLDocumentProcessor(const HTMLDocumentProcessor& src);
    ~HTMLDocumentProcessor();

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

private:
    fieldid_t m_titleFieldId;
    HTMLParser m_parser;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_HTMLDOCUMENTPROCESSOR_H
