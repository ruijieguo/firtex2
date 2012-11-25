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
// Created : 2011-06-30 23:44:13

#ifndef __FX_TRECDOCUMENTPROCESSOR_H
#define __FX_TRECDOCUMENTPROCESSOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/extension/processor/DocumentProcessor.h"
#include "firtex/extension/processor/DocumentReader.h"
#include "firtex/extension/processor/DocumentTemplate.h"
#include "firtex/extension/processor/StandardDocumentProcessor.h"
#include "firtex/extension/processor/HTMLParser.h"

FX_NS_DEF(processor);

class TrecDocumentProcessor : public StandardDocumentProcessor
{
public:
    DECLARE_DOCUMENT_PROCESSOR_CREATOR(TrecDocumentProcessor, "trec", "*");

public:
    TrecDocumentProcessor();
    TrecDocumentProcessor(const TrecDocumentProcessor& src);
    ~TrecDocumentProcessor();

public:
    /// Initialize
    virtual void init(const FX_NS(document)::DocumentSchema* pDocSchema,
                      const DocumentTemplate* pTemplate);

    /// Clone a new object
    virtual DocumentProcessor* clone() const;

    /// Set content type
    void setContentType(const std::string& sContType);

private:
    virtual void doProcessFile(const std::string& sFilePath, DocumentSource& docSource);

private:
    class Tag
    {
    public:
        Tag(fieldid_t id, const std::string& tag)
            : fieldId(id)
        {
            tagLen = tag.length();
            beginTag = new char[tagLen + 3];
            beginTag[0] = '<';
            strcpy(beginTag + 1, tag.c_str());
            beginTag[tagLen + 1] = '>';
            beginTag[tagLen + 2] = 0;

            endTag = new char[tagLen + 4];
            strcpy(endTag + 2, tag.c_str());
            endTag[0] = '<';
            endTag[1] = '/';
            endTag[tagLen + 2] = '>';
            endTag[tagLen + 3] = 0;
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
        size_t tagLen;
    };

    class TagConsumer
    {
    public:
        struct MetaInfo 
        {
            fieldid_t idx;
            const char* metaName;
            const char* keyName;
        };

        const static fieldid_t META_URL = 0;
        const static fieldid_t META_DATE = 1;
        const static fieldid_t META_CONTENT_TYPE = 2;
        const static fieldid_t META_CONTENT_LENGTH = 3;
        const static fieldid_t META_LAST_MODIFIED = 4;
        const static fieldid_t META_TITLE = 5;

        const static size_t META_MAX = 6;

        static MetaInfo META_INFO[META_MAX];

        static std::string META_HEADER_TAG;

    public:
        TagConsumer()
            : m_pHTMLParser(NULL)
            , m_uCurTag(0)
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
#define FX_FIND_LINE_END(start, end, find)                              \
        find = FX_NS(utility)::strFindInRange(start, end, "\r\n");      \
        if (!find)                                                      \
        {                                                               \
            find = FX_NS(utility)::strFindInRange(start, end, "\n");    \
        }
    
        bool extractMeta(const char* szHeader, size_t length, 
                         FX_NS(document)::DocumentPtr& pDoc)
        {
            if (!szHeader || length == 0)
            {
                return false;
            }
            const char* pBeg = szHeader;
            const char* pEnd = szHeader + length;
            bool bHasField = false;
            fieldid_t fieldId = m_metadata[META_INFO[META_URL].idx].getId();
            if (fieldId != INVALID_FIELDID)
            {
                skipBlank(pBeg);
                const char* pCursor = NULL;
                FX_FIND_LINE_END(pBeg, pEnd, pCursor);
                if (pCursor)
                {
                    const char* pLineEnd = pCursor;
                    pCursor = FX_NS(utility)::strFindInRange(
                            pBeg, pLineEnd, " ");
                    if (!pCursor)
                    {
                        pCursor = pLineEnd;
                    }

                    if (pCursor - pBeg > 0)
                    {
                        pDoc->addField(fieldId, pBeg, pCursor - pBeg);
                        bHasField = true;
                    }
                }
                pBeg = pCursor;
                if (!pBeg)
                {
                    skipBlank(pBeg);
                }
            }

            for (size_t i = 1; i < META_MAX; ++i)
            {
                fieldid_t fieldId = m_metadata[META_INFO[i].idx].getId();
                if (fieldId != INVALID_FIELDID && *(META_INFO[i].keyName) && 
                    extractOneMeta(pBeg, pEnd, fieldId, META_INFO[i].keyName, pDoc))
                {
                    bHasField = true;
                }
            }
            return bHasField;

        }
        
        bool extractOneMeta(const char*& szBeg, const char* szEnd,
                            fieldid_t fieldId, const char* szKey, 
                            FX_NS(document)::DocumentPtr& pDoc)
        {
            const char* pCursor = FX_NS(utility)::strFindInRange(szBeg, szEnd, szKey);
            if (pCursor)
            {
                skipBlank(pCursor);
                const char* pCursorEnd = NULL;
                FX_FIND_LINE_END(pCursor, szEnd,pCursorEnd);
                if (!pCursorEnd)
                {
                    pCursorEnd = szEnd;
                }
                pCursor += FX_NS(utility)::strLength(szKey);
                skipBlank(pCursor);
                if (pCursorEnd - pCursor > 0)
                {
                    pDoc->addField(fieldId, pCursor, pCursorEnd - pCursor);
                    szBeg = pCursorEnd;
                    return true;
                }
            }
            return false;
        }
#undef FX_FIND_LINE_END

    public:
        void init(const FX_NS(document)::DocumentSchema* pDocSchema,
                  const DocumentTemplate* pTemplate,
                  HTMLParser* pHTMLParser)
        {
            m_pHTMLParser = pHTMLParser;

            if (!pTemplate || pTemplate->getNumTags() <= 0)
            {
                FIRTEX_THROW(InvalidConfigException, "Document template for "
                        "trec processor is not defined.");
            }

            const Metadata& metadata = pTemplate->getMetadata();
            if (metadata.size() > 0)
            {
                m_metadata.resize(META_MAX);
            }
            for (Metadata::const_iterator it = metadata.begin();
                 it != metadata.end(); ++it)
            {
                for (size_t i = 0; i < META_MAX; ++i)
                {
                    if (!FX_NS(utility)::strCompareNoCase(
                                    it->getMetaName().c_str(), META_INFO[i].metaName))
                    {
                        m_metadata[META_INFO[i].idx] = *it;
                        break;
                    } 
                }
            }

            m_tags.reserve(pTemplate->getNumTags() + 1);

            m_tags.push_back(new Tag(INVALID_FIELDID, pTemplate->getDocTag()));
            const std::string& sEmbedField = pTemplate->getEmbeddedField();
            if (!sEmbedField.empty())
            {
                const FX_NS(document)::FieldSchema* pFieldSchema =
                    pDocSchema->getSchema(sEmbedField);
                if (!pFieldSchema)
                {
                    FX_LOG(WARN, "Embedded field [%s] defined, "
                            "but not in document schema", sEmbedField.c_str());
                }
                else
                {
                    m_tags[0]->fieldId = pFieldSchema->getId();
                }
            }

            DocumentTemplate::Iterator iter = pTemplate->iterator();
            int32_t i = 1;
            while (iter.hasNext())
            {
                const DocumentTemplate::Tag& tag = iter.next();
                m_tags.push_back(new Tag(tag.getId(), tag.getTagName()));
                i++;
            }
            if (i == 1)
            {
                FIRTEX_THROW(InvalidConfigException, "Bad Trec collection "
                        "template: at least define one field.");
            }
        }

        void reset(size_t nDocLen)
        {
            m_uCurTag = 1;
            m_nDocLen = nDocLen;
            m_nCursorInDoc = 0;
        }

        const Tag* getDocTag() const 
        {
            return m_tags[0];
        }

        bool hasNext() const
        {
            if (m_tags[0]->fieldId != INVALID_FIELDID)
            {
                return m_uCurTag < m_tags.size() + 1;
            }
            return m_uCurTag < m_tags.size();
        }

        bool consumeNext(const char*& pBeg, const char*& pDocEnd,
                         FX_NS(document)::DocumentPtr& pDoc)
        {
            const char* pValBeg = NULL;
            const char* pValEnd = NULL;
            bool bHasField = false;

            const char* pSavedBeg = pBeg;
            while (m_uCurTag < m_tags.size())
            {
                const Tag* pCurTag = m_tags[m_uCurTag];
                skipBlank(pBeg);

                if (!FX_NS(utility)::strCompare(pBeg, pCurTag->beginTag, pCurTag->tagLen + 2))
                {
                    pValBeg = (pBeg += pCurTag->tagLen + 2); //Skip begin tag <xxx>
                    pValEnd = FX_NS(utility)::strFindInRange(pBeg, pDocEnd, 
                            pCurTag->endTag);
                    if (pValEnd)
                    {
                        //Skip end tag </xxx>
                        pBeg = pValEnd + pCurTag->tagLen + 3;
                        m_nCursorInDoc += pBeg - pSavedBeg;
                        ++m_uCurTag;
                        size_t len = pValEnd - pValBeg;
                        if (pCurTag->fieldId != INVALID_FIELDID && len > 0)
                        {
                            pDoc->addField(pCurTag->fieldId, pValBeg, len);
                            bHasField = true;
                        }

                        if (m_metadata.size() > 0 && !FX_NS(utility)::strCompare(
                                        pCurTag->beginTag, META_HEADER_TAG.c_str()))
                        {
                            if (extractMeta(pValBeg, pValEnd - pValBeg, pDoc))
                            {
                                bHasField = true;
                            }
                        }
                        return bHasField;
                    }
                    return bHasField;
                }
                
                ++m_uCurTag;
            }// end while

            if (m_tags[0]->fieldId != INVALID_FIELDID)
            {
                //Has embedded field
                pValBeg = pBeg;
                pValEnd = pDocEnd - m_tags[0]->tagLen + 3;

                if (m_pHTMLParser)
                {
                    bool bExtractTitle = false;
                    if (m_metadata.size() > 0 && m_metadata[META_INFO[5].idx].getId() 
                            != INVALID_FIELDID)
                    {
                        bExtractTitle = true;
                    }
                    m_pHTMLParser->setExtractTitle(bExtractTitle);

                    m_pHTMLParser->parse(pValBeg, pValEnd - pValBeg);
                    size_t nContentLen = 0;
                    pValBeg = m_pHTMLParser->getFullText(nContentLen);
                    if (nContentLen > 0)
                    {
                        pDoc->addField(m_tags[0]->fieldId, pValBeg, nContentLen);
                    }

                    if (bExtractTitle)
                    {
                        size_t nTitleLen = 0;
                        pValBeg = m_pHTMLParser->getTitle(nTitleLen);
                        if (nTitleLen > 0)
                        {
                            pDoc->addField(m_metadata[META_INFO[META_TITLE].idx].getId(), 
                                    pValBeg, nTitleLen);
                        }
                    }

                    pValEnd = pValBeg + nContentLen;
                    bHasField = true;
                }
                
                ++m_uCurTag;
                return bHasField;
            }
            return bHasField;
        }

    private:
        typedef std::vector<Tag*> TagVect;
        typedef DocumentTemplate::Metadata Metadata;

        HTMLParser* m_pHTMLParser;
        
        uint32_t m_uCurTag;
        TagVect m_tags;
        size_t m_nDocLen;
        size_t m_nCursorInDoc;

        Metadata m_metadata;
    };

private:
    inline size_t processDocTag(const char*& pReadUpTo, const char*& pDocEnd,
                                const Tag* pDocTag);

private:
    TagConsumer m_tagConsumer;
    HTMLParserPtr m_pHTMLParser;

private:
    DECLARE_STREAM_LOGGER();
};

/////////////////////////////////////////////////
///
inline size_t TrecDocumentProcessor::processDocTag(const char*& pReadUpTo, 
        const char*& pDocEnd, const Tag* pDocTag)
{
    skipBlank(pReadUpTo);
    FIRTEX_ASSERT2(pReadUpTo <= (m_pBuffer + m_nBytesInBuffer));
            
    /// Ensure document tag <xxx> is in buffer
    pReadUpTo = ensureBuffer(pReadUpTo, pDocTag->tagLen + 2);
    if (pReadUpTo == NULL)
    {
        return 0;
    }

    const char* pReadEnd = m_pBuffer + m_nBytesInBuffer;

    char* pCursor = FX_NS(utility)::strFindInRange(pReadUpTo, pReadEnd, 
            pDocTag->beginTag);
    if (!pCursor)
    {
        FX_LOG(WARN, "Skip document: find DOC tag FAILED.");
        pReadUpTo = pReadEnd;
        return 0;
    }
    pReadUpTo = pCursor;

    pReadUpTo += (pDocTag->tagLen + 2); //skip document tag <xxx>
    FIRTEX_ASSERT2(pReadUpTo <= pReadEnd);

    pDocEnd = FX_NS(utility)::strFindInRange(pReadUpTo, pReadEnd,
            pDocTag->endTag);
    if (!pDocEnd)
    {
        //Ensure the entire document is in the buffer
        if (!(pReadUpTo = ensureBuffer(pReadUpTo)))
        {
            FX_LOG(WARN, "Skip document: missing document end tag: \"[%s]\", " 
                   "file: [%s], offset: [%llu]", pDocTag->endTag,
                   m_pDocReader->getFilePath().c_str(),
                   (unsigned long long)m_pDocReader->tell() - (pReadEnd - pReadUpTo));
            pReadUpTo = pReadEnd = m_pBuffer + m_nBytesInBuffer;
            return 0;
        }
        pReadEnd = m_pBuffer + m_nBytesInBuffer;

        pDocEnd = FX_NS(utility)::strFindInRange(pReadUpTo, pReadEnd, pDocTag->endTag);
        if (!pDocEnd)
        {
            FX_LOG(WARN, "Cut document: document is too long (> %d) or missing document end tag." 
                            "file: [%s], offset: [%llu]", (int32_t)DEFAULT_READ_BUFFER_SIZE,
                   m_pDocReader->getFilePath().c_str(),
                   (unsigned long long)m_pDocReader->tell() - (pReadEnd - pReadUpTo));
            pDocEnd = pReadEnd;
            return (pDocEnd - pReadUpTo);            
        }
    }

    size_t nDocLen = pDocEnd - pReadUpTo;
    pDocEnd += (pDocTag->tagLen + 3);//skip document end tag </xxx>
    return nDocLen;
}

FX_NS_END

#endif //__FX_TRECDOCUMENTPROCESSOR_H
