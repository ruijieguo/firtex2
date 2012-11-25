#include "firtex/extension/processor/PlainDocumentProcessor.h"
#include "firtex/utility/BinaryFile.h"
#include "firtex/utility/NumberFormatter.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/File.h"

FX_NS_USE(document);
FX_NS_USE(utility);
using namespace std;

FX_NS_DEF(processor);

SETUP_STREAM_LOGGER(processor, PlainDocumentProcessor);

PlainDocumentProcessor::PlainDocumentProcessor() 
{
    for (size_t i = 0; i < MAX_FIELD; ++i)
    {
        m_schemas[i] = INVALID_FIELDID;
    }
}

PlainDocumentProcessor::~PlainDocumentProcessor() 
{
}

PlainDocumentProcessor::PlainDocumentProcessor(const PlainDocumentProcessor& src)
{
    for (size_t i = 0; i < MAX_FIELD; ++i)
    {
        m_schemas[i] = src.m_schemas[i];
    }
}

void PlainDocumentProcessor::init(const DocumentSchema* pDocSchema,
                                  const DocumentTemplate* /* pTemplate */)
{
    const FieldSchema* pFieldSchema = pDocSchema->getSchema("BODY");
    if (!pFieldSchema)
    {
        pFieldSchema = pDocSchema->getSchema("CONTENT");
        if (!pFieldSchema)
        {
            FX_LOG(WARN, "BODY field is not defined.");
        }
    }
    if (pFieldSchema)
    {
        m_schemas[BODY] = pFieldSchema->getId();
    }

    pFieldSchema = pDocSchema->getSchema("FILEPATH");
    if (!pFieldSchema)
    {
        pFieldSchema = pDocSchema->getSchema("PATH");
        if (!pFieldSchema)
        {
            pFieldSchema = pDocSchema->getSchema("URL");
        }
    }
    if (pFieldSchema)
    {
        m_schemas[FILEPATH] = pFieldSchema->getId();
    }
    else 
    {
        FIRTEX_THROW_AND_LOG(InitializationException, "FILEPATH field is not defined." );
    }

    pFieldSchema = pDocSchema->getSchema("FILENAME");
    if (pFieldSchema)
    {
        m_schemas[FILENAME] = pFieldSchema->getId();
    }

    pFieldSchema = pDocSchema->getSchema("FILETYPE");
    if (pFieldSchema)
    {
        m_schemas[FILETYPE] = pFieldSchema->getId();
    }

    pFieldSchema = pDocSchema->getSchema("MODIFIED");
    if (pFieldSchema)
    {
        m_schemas[MODIFIED] = pFieldSchema->getId();
    }

    pFieldSchema = pDocSchema->getSchema("FILESIZE");
    if (pFieldSchema)
    {
        m_schemas[FILESIZE] = pFieldSchema->getId();
    }
}

void PlainDocumentProcessor::process(DocumentSource& docSource)
{
    DocumentPtr pDoc = docSource.acquireDocument("plain");
    pDoc->setAction(Document::AT_ADD);

    const RawDocumentPtr& pRawDoc = docSource.getRawDocument();
    if (pRawDoc->isPath())
    {
        doProcessFile(pRawDoc->getPath(), pDoc);
    }
    else 
    {
        if (m_schemas[BODY] != INVALID_FIELDID)
        {
            pDoc->addField(m_schemas[BODY], pRawDoc->getContent());
        }
    }

    docSource.setToBeContinued(false);
}

void PlainDocumentProcessor::doProcessFile(const string& sFilePath,
        DocumentPtr& pDoc) const
{
    FX_LOG(INFO, "Processing file: [%s]", sFilePath.c_str());

    doProcessMeta(sFilePath, pDoc);

    /// Read content
    if (m_schemas[BODY] != INVALID_FIELDID)
    {
        try
        {
            BinaryFile binFile;
            binFile.open(sFilePath, BinaryFile::READ);
            size_t nByte = (size_t)binFile.getLength();
            Field* pField = pDoc->addField(m_schemas[BODY]);
            FIRTEX_ASSERT2(pField != NULL);
            Field::String& value = pField->getValue();
            value.resize(nByte);
            binFile.read(value.data(), nByte);
            binFile.close();
        }
        catch(const FirteXException& e)
        {
            FX_LOG(ERROR, "Read file: [%s] FAILED, reason: [%s].",
                   sFilePath.c_str(), e.what().c_str());
        }
    }
}

void PlainDocumentProcessor::doProcessMeta(const string& sFilePath,
        DocumentPtr& pDoc) const
{
    Path filePath(sFilePath);

    if (m_schemas[FILENAME] != INVALID_FIELDID)
    {
        pDoc->addField(m_schemas[FILENAME], filePath.getFileName());
    }
    if (m_schemas[FILEPATH] != INVALID_FIELDID)
    {
        pDoc->addField(m_schemas[FILEPATH], sFilePath);
    }
    if (m_schemas[FILETYPE] != INVALID_FIELDID)
    {
        pDoc->addField(m_schemas[FILETYPE], filePath.getExtension());
    }

    if ((m_schemas[MODIFIED] != INVALID_FIELDID) || (m_schemas[FILESIZE] != INVALID_FIELDID))
    {
        File file(sFilePath);
       
        if (m_schemas[MODIFIED] != INVALID_FIELDID)
        {
            int64_t value = file.lastModified().time();
            string str;
            NumberFormatter::append(str, value);
            pDoc->addField(m_schemas[MODIFIED], str);
        }
            
        if (m_schemas[FILESIZE] != INVALID_FIELDID)
        {
            int64_t value = (int64_t)file.getSize();
            string str;
            NumberFormatter::append(str, value);
            pDoc->addField(m_schemas[FILESIZE], str);
        }
    }
}

DocumentProcessor* PlainDocumentProcessor::clone() const
{
    return new PlainDocumentProcessor(*this);
}

FX_NS_END

