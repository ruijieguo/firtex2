#include "firtex/extension/processor/HTMLDocumentProcessor.h"

FX_NS_USE(document);

FX_NS_DEF(processor);

SETUP_STREAM_LOGGER(processor, HTMLDocumentProcessor);

HTMLDocumentProcessor::HTMLDocumentProcessor() 
    : m_titleFieldId(INVALID_FIELDID)
{
}

HTMLDocumentProcessor::~HTMLDocumentProcessor() 
{
}

HTMLDocumentProcessor::HTMLDocumentProcessor(const HTMLDocumentProcessor& src)
    : PlainDocumentProcessor(src)
    , m_titleFieldId(src.m_titleFieldId)
{
}

void HTMLDocumentProcessor::init(const DocumentSchema* pDocSchema,
                                 const DocumentTemplate* /* pTemplate */)
{
    PlainDocumentProcessor::init(pDocSchema);
     
    const FieldSchema* pFieldSchema = pDocSchema->getSchema("TITLE");
    if (pFieldSchema)
    {
        m_titleFieldId = pFieldSchema->getId();
    }
}

void HTMLDocumentProcessor::process(DocumentSource& docSource)
{
    const RawDocumentPtr& pRawDoc = docSource.getRawDocument();
    DocumentPtr pDoc = docSource.acquireDocument("HTML");
    pDoc->setAction(Document::AT_ADD);

    m_parser.setExtractTitle(m_titleFieldId != INVALID_FIELDID);
    if (pRawDoc->isPath())
    {
        const string& sFilePath = pRawDoc->getPath();
        m_parser.parse(sFilePath);
        doProcessMeta(sFilePath, pDoc);
    }
    else 
    {
        const string& sContent = pRawDoc->getContent();
        m_parser.parse(sContent.c_str(), sContent.length());
    }
    
    if (m_titleFieldId != INVALID_FIELDID)
    {
        size_t nTitleLen = 0;
        const char* szTitle = m_parser.getTitle(nTitleLen);
        pDoc->addField(m_titleFieldId, szTitle, nTitleLen);
    }
    if (m_schemas[BODY] != INVALID_FIELDID)
    {
        size_t nBodyLen = 0;
        const char* szBody = m_parser.getFullText(nBodyLen);
        pDoc->addField(m_schemas[BODY], szBody, nBodyLen);
    }
}

DocumentProcessor* HTMLDocumentProcessor::clone() const
{
    return new HTMLDocumentProcessor(*this);
}

FX_NS_END

