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
// Created : 2011-06-19 22:46:09

#ifndef __FX_STANDARDDOCUMENTPROCESSOR_H
#define __FX_STANDARDDOCUMENTPROCESSOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/extension/processor/DocumentProcessor.h"
#include "firtex/extension/processor/PlainDocumentReader.h"
#include "firtex/extension/processor/GZipDocumentReader.h"

FX_NS_DEF(processor);

class StandardDocumentProcessor : public DocumentProcessor
{
public:
    DECLARE_DOCUMENT_PROCESSOR_CREATOR(StandardDocumentProcessor, "standard", "standard");

public:
    /// Default value of read buffer size
    const static size_t DEFAULT_READ_BUFFER_SIZE = 1024 * 1024; //1 MB

    /// The minimal available bytes left in buffer
    const static size_t MIN_AVAILABLE_BYTES_IN_BUFFER = 32 * 1024; //32 KB

    /// Define document leading line separator
    static std::string DOCUMENT_LEADING_LINE_SEPARATOR;

    /// Define document end line separator
    static std::string DOCUMENT_END_LINE_SEPARATOR;

    /// Define field line separator
    static std::string FIELD_LINE_SEPARATOR;


public:
    StandardDocumentProcessor();
    StandardDocumentProcessor(const StandardDocumentProcessor& src);
    ~StandardDocumentProcessor();

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
    /**
     * Ensure at least nEnsureBytes available for read
     *
     * @param pReadUpTo current read position
     * @param nEnsureBytes number of bytes to ensure
     *
     * @return the new address of pReadUp after buffer relocation.
     *         Return null if failure.
     */
    const char* ensureBuffer(const char* pReadUpTo, size_t nEnsureBytes = 0);

    /// Process one document
    virtual void doProcessFile(const std::string& sPath, DocumentSource& docSource);

    /// Create document reader by file extension
    inline bool initDocumentReader(const std::string& sPath);


    inline static bool strToInt32(const char* str, int32_t& value);
    inline static void skipBlank(const char*& p);
    
private:
    /// Process document leading line
    size_t processDocLine(const char*& pReadUpTo,
                          const char*& pDocEnd,
                          FX_NS(document)::DocumentPtr& pDoc);

private:
    class Tag
    {
    public:
        Tag(fieldid_t id, const std::string& tag)
            : fieldId(id)
        {
            begTagLen = tag.length() + 1;
            beginTag = new char[begTagLen + 1];
            strcpy(beginTag, tag.c_str());
            beginTag[begTagLen - 1] = '=';
            beginTag[begTagLen] = 0;

            endTagLen = FIELD_LINE_SEPARATOR.length();
            endTag = new char[endTagLen + 1];
            strcpy(endTag, FIELD_LINE_SEPARATOR.c_str());
        }
        ~Tag()
        {
            delete[] beginTag;
            delete[] endTag;
        }
    public:
        fieldid_t fieldId;
        char* beginTag;
        char* endTag;
        size_t begTagLen;
        size_t endTagLen;
    };

    class TagConsumer
    {
    public:
        TagConsumer()
            : m_uCurTag(0)
            , m_nDocLen(0)
            , m_nCursorInDoc(0)
        {
        }

        ~TagConsumer()
        {
            for (size_t i = 0; i < m_tags.size(); ++i)
            {
                delete m_tags[i];
            }
            m_tags.clear();
        }

    public:
        void init(const FX_NS(document)::DocumentSchema* pDocSchema)
        {
            FX_NS(document)::DocumentSchema::Iterator it = pDocSchema->iterator();
            m_tags.reserve(it.size());
            while (it.hasNext())
            {
                const FX_NS(document)::FieldSchema* pFieldSchema = it.next();
                m_tags.push_back(new Tag(pFieldSchema->getId(), pFieldSchema->getName()));
            }
        }

        void reset(size_t nDocLen)
        {
            m_uCurTag = 0;
            m_nDocLen = nDocLen;
            m_nCursorInDoc = 0;
        }

        fieldid_t next(const char*& pBeg, const char*& pDocEnd, 
                       const char*& pValBeg, const char*& pValEnd)
        {
            const char* pSavedBeg = pBeg;
            while (m_uCurTag < m_tags.size())
            {
                const Tag* pCurTag = m_tags[m_uCurTag];
                skipBlank(pBeg);

                if (!FX_NS(utility)::strCompare(pBeg, pCurTag->beginTag, pCurTag->begTagLen))
                {
                    pValBeg = (pBeg += pCurTag->begTagLen);
                    pValEnd = FX_NS(utility)::strFindInRange(
                            pBeg, pDocEnd, pCurTag->endTag);
                    if (pValEnd)
                    {
                        //Skip FIELD_LINE_SEPARATOR
                        pBeg = pValEnd + pCurTag->endTagLen;
                        m_nCursorInDoc += pBeg - pSavedBeg;
                        ++m_uCurTag;
                        return pCurTag->fieldId;
                    }
                    else 
                    {
                        return INVALID_FIELDID;
                    }
                }
                
                ++m_uCurTag;
            }// end while
            return INVALID_FIELDID;
        }

    private:
        typedef std::vector<Tag*> TagVect;

        uint32_t m_uCurTag;
        TagVect m_tags;
        size_t m_nDocLen;
        size_t m_nCursorInDoc;
    };

private:
    TagConsumer m_tagConsumer;

protected:
    DocumentReaderPtr m_pDocReader;

    char* m_pBuffer;
    size_t m_nBufferSize;
    size_t m_nBytesInBuffer;
    size_t m_nReadUpTo;

private:
    DECLARE_STREAM_LOGGER();
};

//////////////////////////////////////////
inline bool StandardDocumentProcessor::strToInt32(
        const char* str, int32_t& value)
{
    char* end = NULL;
    value = (int32_t)strtol(str, &end, 10);
    return (value > 0);
}

inline void StandardDocumentProcessor::skipBlank(const char*& p)
{
    while(*p == '\n' || *p == '\r' || *p == ' ' || *p == '\t')  \
        p++;
}

inline bool StandardDocumentProcessor::initDocumentReader(const std::string& sPath)
{
    if (sPath.size() > 3 && !FX_NS(utility)::strCompareNoCase(sPath.c_str() + sPath.size() - 3, ".gz"))
    {
        m_pDocReader.reset(new GZipDocumentReader());
    }
    else 
    {
        m_pDocReader.reset(new PlainDocumentReader());
    }

    try
    {
        m_pDocReader->open(sPath);
        m_nBytesInBuffer = m_pDocReader->read(m_pBuffer, m_nBufferSize);
        m_nReadUpTo = 0;
    }
    catch(const FX_NS(utility)::FileIOException& e)
    {
        FX_LOG(ERROR, "Open plain file: [%s] FAILED, reason: [%s]",
               sPath.c_str(), e.what().c_str());
        m_pDocReader.reset();
        return false;
    }
    
    return true;
}

FX_NS_END

#endif //__FX_STANDARDDOCUMENTPROCESSOR_H
