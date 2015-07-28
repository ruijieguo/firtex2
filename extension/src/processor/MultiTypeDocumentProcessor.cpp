#include "firtex/extension/processor/MultiTypeDocumentProcessor.h"
#include "firtex/utility/StringUtils.h"

FX_NS_USE(document);
FX_NS_USE(utility);

FX_NS_DEF(processor);

SETUP_STREAM_LOGGER(processor, MultiTypeDocumentProcessor);

MultiTypeDocumentProcessor::MultiTypeDocumentProcessor() 
{
}

MultiTypeDocumentProcessor::MultiTypeDocumentProcessor(const MultiTypeDocumentProcessor& src)
{
    for (ProcessorMap::const_iterator it = src.m_extToProcessor.begin();
         it != src.m_extToProcessor.end(); ++it)
    {
        DocumentProcessorPtr pTmp(it->second->clone());
        m_extToProcessor.insert(make_pair(it->first, pTmp));
    }
}

MultiTypeDocumentProcessor::~MultiTypeDocumentProcessor() 
{
}

void MultiTypeDocumentProcessor::init(const DocumentSchema* pDocSchema,
                                      const DocumentTemplate* pTemplate)
{
    for (ProcessorMap::iterator it = m_extToProcessor.begin();
         it != m_extToProcessor.end(); ++it)
    {
        it->second->init(pDocSchema, pTemplate);
    }
}

void MultiTypeDocumentProcessor::addProcessor(const std::string& sExtension, 
        const DocumentProcessorPtr& pProcessor)
{
    ProcessorMap::const_iterator it = m_extToProcessor.find(toLower(sExtension));
    if (it == m_extToProcessor.end())
    {
        m_extToProcessor.insert(make_pair(sExtension, pProcessor));
    }
}

void MultiTypeDocumentProcessor::process(DocumentSource& docSource)
{
    const RawDocumentPtr& pRawDoc = docSource.getRawDocument();
    if (pRawDoc->isPath())
    {
        const string& sFilePath = pRawDoc->getPath();
        string::size_type pos = sFilePath.rfind(".");
        if (pos != string::npos)
        {
            string sExt = sFilePath.substr(pos + 1, sFilePath.length() - pos - 1);
            ProcessorMap::iterator it = m_extToProcessor.find(sExt);
            if (it != m_extToProcessor.end())
            {
                return it->second->process(docSource);
            }
        }
        FX_LOG(WARN, "Can't find processor for file: [%s]", sFilePath.c_str());
    }
    else 
    {
        FX_LOG(WARN, "Skip document: input raw document is invalid.");
    }    
}

DocumentProcessor* MultiTypeDocumentProcessor::clone() const
{
    return new MultiTypeDocumentProcessor(*this);
}

FX_NS_END

