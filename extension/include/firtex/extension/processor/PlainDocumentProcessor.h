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
// Created : 2011-06-25 11:52:45

#ifndef __FX_PLAINDOCUMENTPROCESSOR_H
#define __FX_PLAINDOCUMENTPROCESSOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/processor/DocumentProcessor.h"

FX_NS_DEF(processor);

class PlainDocumentProcessor : public DocumentProcessor
{
public:
    DECLARE_DOCUMENT_PROCESSOR_CREATOR(PlainDocumentProcessor, "plain", "txt");

public:
    enum FieldCat
    {
        BODY = 0,
        FILENAME,
        FILEPATH,
        FILETYPE,
        MODIFIED,
        FILESIZE,
        MAX_FIELD,
    };

public:
    PlainDocumentProcessor();
    PlainDocumentProcessor(const PlainDocumentProcessor& src);
    ~PlainDocumentProcessor();

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

protected:
    void doProcessFile(const std::string& sFilePath,
                       FX_NS(document)::DocumentPtr& pDoc) const;
    void doProcessMeta(const std::string& sFilePath,
                       FX_NS(document)::DocumentPtr& pDoc) const;

protected:
    fieldid_t m_schemas[MAX_FIELD];

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_PLAINDOCUMENTPROCESSOR_H
