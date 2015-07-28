#include "firtex/extension/processor/StandardDocumentProcessor.h"
#include "firtex/extension/processor/PlainDocumentReader.h"

FX_NS_USE(utility);
FX_NS_USE(document);

FX_NS_DEF(processor);

SETUP_STREAM_LOGGER(processor, StandardDocumentProcessor);

std::string StandardDocumentProcessor::DOCUMENT_LEADING_LINE_SEPARATOR = "\n";
std::string StandardDocumentProcessor::DOCUMENT_END_LINE_SEPARATOR = "\n";
std::string StandardDocumentProcessor::FIELD_LINE_SEPARATOR = "\n";

StandardDocumentProcessor::StandardDocumentProcessor() 
    : m_pBuffer(new char[DEFAULT_READ_BUFFER_SIZE])
    , m_nBufferSize(DEFAULT_READ_BUFFER_SIZE)
    , m_nBytesInBuffer(0)
    , m_nReadUpTo(0)
{
}

StandardDocumentProcessor::StandardDocumentProcessor(const StandardDocumentProcessor& src)
    : m_pBuffer(new char[src.m_nBufferSize])
    , m_nBufferSize(src.m_nBufferSize)
    , m_nBytesInBuffer(0)
    , m_nReadUpTo(0)
{
}

StandardDocumentProcessor::~StandardDocumentProcessor() 
{
    delete[] m_pBuffer;
    m_pBuffer = NULL;
}

void StandardDocumentProcessor::init(const DocumentSchema* pDocSchema,
                                     const DocumentTemplate* /* pTemplate */)
{
    m_tagConsumer.init(pDocSchema);
}

void StandardDocumentProcessor::process(DocumentSource& docSource)
{
    const RawDocumentPtr& pRawDoc = docSource.getRawDocument();
    if (pRawDoc->isPath())
    {
        doProcessFile(pRawDoc->getPath(), docSource);
    }
    else 
    {
        FX_LOG(WARN, "Skip document: input raw document is invalid.");
    }
}

void StandardDocumentProcessor::doProcessFile(const std::string& sPath,
        DocumentSource& docSource)
{
    FX_DEBUG("Process file: [%s]", sPath.c_str());
    if (!docSource.toBeContinued())
    {
        // a new raw document
        if (!initDocumentReader(sPath))
        {
            FX_LOG(ERROR, "Skip documents: init document reader for [%s] FAILED.", 
                   sPath.c_str());
            return;
        }
    }

    if (!m_pDocReader)
    {
        FX_LOG(WARN, "Skip document: document reader for [%s] "
               "FAILED did not initialize.", 
               sPath.c_str());
        return;
    }

    DocumentPtr pDoc = docSource.acquireDocument("standard");
    bool bHasFields = false;

    const char* pReadUpTo = m_pBuffer + m_nReadUpTo;
    while (!m_pDocReader->isEof() || m_nReadUpTo < m_nBytesInBuffer)
    {
        const char* pDocEnd = NULL;
        size_t nDocLen = processDocLine(pReadUpTo, pDocEnd, pDoc);
        if (nDocLen == 0)
        {
            if (!(pReadUpTo = ensureBuffer(pReadUpTo)))
            {
                break;
            }

            m_nReadUpTo = (pReadUpTo - m_pBuffer);
            continue;
        }
    
        m_tagConsumer.reset(nDocLen);
        
        for (;;)
        {
            const char* szValBeg = NULL;
            const char* szValEnd = NULL;
            fieldid_t fieldId = m_tagConsumer.next(pReadUpTo, pDocEnd,
                    szValBeg, szValEnd);
            if (fieldId != INVALID_FIELDID)
            {
                if ((szValEnd - szValBeg) > 0)
                {
                    pDoc->addField(fieldId, szValBeg, szValEnd - szValBeg);
                    bHasFields = true;
                }
            }
            else
            {
                break;
            }
        }

        skipBlank(pDocEnd);

        m_nReadUpTo = pDocEnd - m_pBuffer;
        if (m_nReadUpTo == m_nBytesInBuffer && m_pDocReader->isEof())
        {
            FX_DEBUG("Completed the processing of  file: [%s] ", sPath.c_str());
            docSource.setToBeContinued(false);
            m_pDocReader.reset();
        }
        else 
        {
            FX_DEBUG("The processing of  file: [%s] need to be continued", sPath.c_str());
            docSource.setToBeContinued(true);
        }

        if (!bHasFields)
        {
            //discard empty document
            (void)docSource.stealLastDocument();
        }

        return;
    } // end while 

    if (!bHasFields)
    {
        //discard empty document
        (void)docSource.stealLastDocument();
    }

    docSource.setToBeContinued(false);
}

const char* StandardDocumentProcessor::ensureBuffer(const char* pReadUpTo, size_t nEnsureBytes)
{
    FIRTEX_ASSERT2(pReadUpTo >= m_pBuffer && pReadUpTo <= (m_pBuffer + m_nBytesInBuffer));
    if (m_nReadUpTo == 0 || m_nBytesInBuffer < m_nBufferSize) 
    {
        if (nEnsureBytes == 0)
        {
            //Must refill buffer, but no bytes to fill
            return NULL;
        }
        return pReadUpTo;
    }

    int32_t nBytesToCpy = (int32_t)m_nBytesInBuffer - (int32_t)(pReadUpTo - m_pBuffer);
    
    if (nEnsureBytes > 0 && nEnsureBytes <= (size_t)nBytesToCpy 
        && (size_t)nBytesToCpy >= MIN_AVAILABLE_BYTES_IN_BUFFER)
    {
        return pReadUpTo;
    }
    
    FX_DEBUG("Ensurebuffer: relocate bytes: [%u].", (uint32_t)nBytesToCpy);
    if (nBytesToCpy > 0)
    {
        memcpy(m_pBuffer, pReadUpTo, nBytesToCpy);
    }

    try
    {
        size_t nBytesRead = m_pDocReader->read(m_pBuffer + nBytesToCpy, m_nBufferSize - nBytesToCpy);
        if (nBytesRead == 0)
        {
            return NULL;
        }

        FX_DEBUG("Ensurebuffer: read bytes: [%u].", (uint32_t)nBytesRead);

        m_nBytesInBuffer = nBytesToCpy + nBytesRead;
        m_nReadUpTo = 0;
    }
    catch(const FileIOException& e)
    {
        FX_LOG(ERROR, "Read data FAILED: [%s]", e.what().c_str());
        return NULL;
    }

    return m_pBuffer;
}

size_t StandardDocumentProcessor::processDocLine(const char*& pReadUpTo,
        const char*& pDocEnd, DocumentPtr& pDoc)
{
    //Ensure "DOC=ADD/DELETE/UPDATE\n" in the buffer
    pReadUpTo = ensureBuffer(pReadUpTo, 13);
    if (pReadUpTo == NULL)
    {
        return 0;
    }        

    char* pCursor = strFindInRange(pReadUpTo, m_pBuffer + m_nBytesInBuffer, "DOC=");
    if (!pCursor)
    {
        FX_LOG(WARN, "Skip document: find DOC tag FAILED");
        pReadUpTo = m_pBuffer + m_nBytesInBuffer;
        return 0;
    }
    pReadUpTo = pCursor;

    pReadUpTo += 4;//skip "DOC="
    if (!strCompare(pReadUpTo, "ADD", 3))
    {
        pDoc->setAction(Document::AT_ADD);
        pReadUpTo += 3; //skip "ADD"
    }
    else if (!strCompare(pReadUpTo, "DELETE", 6))
    {
        pDoc->setAction(Document::AT_DELETE);
        pReadUpTo += 6; //skip "DELETE"
    }
    else if (!strCompare(pReadUpTo, "UPDATE", 6))
    {
        /// UPDATE
        FX_LOG(WARN, "Skip document: update is not implemented");
        pReadUpTo += 6;
        return 0;
    }
    else 
    {
        /// Not an valid document leading line
        FX_LOG(WARN, "Skip document: found invalid DOC tag: file: [%s], offset[%llu]", 
               m_pDocReader->getFilePath().c_str(), (unsigned long long int)m_pDocReader->tell());
        pReadUpTo += 4; //Skip "DOC="
        return 0;
    }

    if (strCompare(pReadUpTo, DOCUMENT_LEADING_LINE_SEPARATOR.c_str(), 
                   DOCUMENT_LEADING_LINE_SEPARATOR.length()))
    {
        /// Not an valid document leading line
        FX_LOG(WARN, "Skip document: invalid leading line: missing \"[%s]\", "
               "file: [%s], offset: [%llu]", DOCUMENT_LEADING_LINE_SEPARATOR.c_str(),
               m_pDocReader->getFilePath().c_str(),

               (unsigned long long)m_pDocReader->tell() - (m_pBuffer + m_nBytesInBuffer - pReadUpTo));
        pReadUpTo += DOCUMENT_LEADING_LINE_SEPARATOR.length(); 
        return 0;
    }
    pReadUpTo += DOCUMENT_LEADING_LINE_SEPARATOR.length(); //skip DOCUMENT_LEADING_LINE_SEPARATOR


    while (*pReadUpTo == ' ' || *pReadUpTo == '\t' || *pReadUpTo == '\n')
        ++pReadUpTo;

    pDocEnd = strFindInRange(pReadUpTo, m_pBuffer + m_nBytesInBuffer,
                             DOCUMENT_END_LINE_SEPARATOR.c_str());
    if (!pDocEnd)
    {
        //Ensure the entire document is in the buffer
        if (!(pReadUpTo = ensureBuffer(pReadUpTo)))
        {
            FX_LOG(WARN, "Skip document: missing document end tag: \"[%s]\", " 
                   "file: [%s], offset: [%llu]", DOCUMENT_END_LINE_SEPARATOR.c_str(),
                   m_pDocReader->getFilePath().c_str(),
                   (unsigned long long)m_pDocReader->tell() - (m_pBuffer + m_nBytesInBuffer - pReadUpTo));
            pReadUpTo = m_pBuffer + m_nBytesInBuffer;
            return 0;
        }

        pDocEnd = strFindInRange(pReadUpTo, m_pBuffer + m_nBytesInBuffer,
                DOCUMENT_END_LINE_SEPARATOR.c_str());
        if (!pDocEnd)
        {
            FX_LOG(WARN, "Cut document: document is too long (> %d) " 
                   "file: [%s], offset: [%llu]", (int32_t)DEFAULT_READ_BUFFER_SIZE,
                   m_pDocReader->getFilePath().c_str(),
                   (unsigned long long)m_pDocReader->tell() - (m_pBuffer + m_nBytesInBuffer - pReadUpTo));
            pDocEnd = m_pBuffer + m_nBytesInBuffer;
            return pDocEnd - pReadUpTo;
        }
    }

    size_t nDocLen = pDocEnd - pReadUpTo;
    pDocEnd += 2; //skip "\n"
    return nDocLen;
}

DocumentProcessor* StandardDocumentProcessor::clone() const
{
    return new StandardDocumentProcessor(*this);
}

FX_NS_END

