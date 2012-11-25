#include "firtex/extension/processor/TrecDocumentProcessor.h"
#include "firtex/extension/processor/PlainDocumentReader.h"

FX_NS_USE(document);
FX_NS_USE(utility);
using namespace std;

FX_NS_DEF(processor);

SETUP_STREAM_LOGGER(processor, TrecDocumentProcessor);

string TrecDocumentProcessor::TagConsumer::META_HEADER_TAG = "<DOCHDR>";

TrecDocumentProcessor::TagConsumer::MetaInfo 
TrecDocumentProcessor::TagConsumer::META_INFO[] = 
{
    {0, "url", ""},
    {1, "date", "Date:"},
    {2, "content-type", "Content-type:"},
    {3, "content-length", "Content-length:"},
    {4, "last-modified", "Last-modified:"},
    {5, "title", ""}
};

TrecDocumentProcessor::TrecDocumentProcessor() 
{
}

TrecDocumentProcessor::TrecDocumentProcessor(const TrecDocumentProcessor& src) 
    : StandardDocumentProcessor(src)
{
}

TrecDocumentProcessor::~TrecDocumentProcessor() 
{
}

void TrecDocumentProcessor::init(const DocumentSchema* pDocSchema, 
                                 const DocumentTemplate* pTemplate)
{
    m_tagConsumer.init(pDocSchema, pTemplate, m_pHTMLParser.get());    
}

void TrecDocumentProcessor::doProcessFile(const string& sFilePath, 
        DocumentSource& docSource)
{
    FX_DEBUG("Process file: [%s]", sFilePath.c_str());
    if (!docSource.toBeContinued())
    {
        // a new raw document
        if (!initDocumentReader(sFilePath))
        {
            FX_LOG(ERROR, "Skip documents: init document reader for [%s] FAILED.", 
                   sFilePath.c_str());
            return;
        }
    }

    if (m_pDocReader.isNull())
    {
        FX_LOG(WARN, "Skip document: document reader for [%s] FAILED did not initialize.", 
               sFilePath.c_str());
        return;
    }

    DocumentPtr pDoc = docSource.acquireDocument("trec");
    pDoc->setAction(Document::AT_ADD);

    const char* pReadUpTo = m_pBuffer + m_nReadUpTo;
    while (!m_pDocReader->isEof() || m_nReadUpTo < m_nBytesInBuffer)
    {
        bool bNotEmty = false;
        const char* pDocEnd = NULL;
        size_t nDocLen = processDocTag(pReadUpTo, pDocEnd, m_tagConsumer.getDocTag());
        if (nDocLen == 0)
        {
            if (!(pReadUpTo = ensureBuffer(pReadUpTo)))
            {
                docSource.setToBeContinued(false);
                m_pDocReader.reset();
                break;
            }
            m_nReadUpTo = (pReadUpTo - m_pBuffer);
            continue;
        }

        m_tagConsumer.reset(nDocLen);
    
        while (m_tagConsumer.hasNext())
        {
            if (m_tagConsumer.consumeNext(pReadUpTo, pDocEnd, pDoc))
            {
                bNotEmty = true;
            }
        }

        skipBlank(pDocEnd);

        m_nReadUpTo = pDocEnd - m_pBuffer;

        if (!bNotEmty)
        {
            FX_LOG(INFO, "Get empty document");
            continue;
        }

        if (m_nReadUpTo >= m_nBytesInBuffer && m_pDocReader->isEof())
        {
            FX_DEBUG("Completed the processing of  file: [%s], "
                     "read to: [%u], in buffer size: [%u], isEof[%d]",
                     sFilePath.c_str(), m_nReadUpTo, m_nBytesInBuffer, m_pDocReader->isEof());
            docSource.setToBeContinued(false);
            m_pDocReader.reset();
        }
        else 
        {
            FX_DEBUG("The processing of  file: [%s] need to be continued, "
                     "read to: [%u], in buffer size: [%u], isEof[%d]",
                     sFilePath.c_str(), m_nReadUpTo, m_nBytesInBuffer, m_pDocReader->isEof());

            docSource.setToBeContinued(true);
        }
        return;
    } // end while 
}

DocumentProcessor* TrecDocumentProcessor::clone() const
{
    return new TrecDocumentProcessor(*this);
}

void TrecDocumentProcessor::setContentType(const string& sContType)
{
    if (!strCompareNoCase(sContType.c_str(), "html") 
        || !strCompareNoCase(sContType.c_str(), "htm"))
    {
        m_pHTMLParser = new HTMLParser;
    }
    else
    {
        FX_LOG(ERROR, "Invalid content type: [%s].", sContType.c_str());
    }
}

FX_NS_END

