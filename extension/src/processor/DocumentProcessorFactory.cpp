#include "firtex/extension/processor/DocumentProcessorFactory.h"
#include "firtex/extension/processor/PlainDocumentProcessor.h"
#include "firtex/extension/processor/HTMLDocumentProcessor.h"
#include "firtex/extension/processor/StandardDocumentProcessor.h"
#include "firtex/utility/StringTokenizer.h"

FX_NS_USE(utility);

FX_NS_DEF(processor);

SETUP_STREAM_LOGGER(processor, DocumentProcessorFactory);

DocumentProcessorFactory::DocumentProcessorFactory() 
{
    registerProcessor(new PlainDocumentProcessor::Creator);
    registerProcessor(new HTMLDocumentProcessor::Creator);
    registerProcessor(new StandardDocumentProcessor::Creator);
}

DocumentProcessorFactory::~DocumentProcessorFactory() 
{
}

void DocumentProcessorFactory::registerProcessor(
        const CreatorPtr& pCreator)
{
    string sIdent = pCreator->getIdentifier();
    CreatorMap::iterator it = m_ident2Processor.find(sIdent);
    if (it != m_ident2Processor.end())
    {
        m_ident2Processor.erase(it);
        FX_LOG(WARN, "Processor : [%s] already exists.", sIdent.c_str());
    }
    m_ident2Processor.insert(make_pair(sIdent, pCreator));

    string sExt = pCreator->getDocType();
    if (!sExt.empty())
    {
        StringTokenizer st(sExt, ";", StringTokenizer::TOKEN_TRIM | 
                           StringTokenizer::TOKEN_IGNORE_EMPTY);
        for (size_t i = 0; i < st.getNumTokens(); ++i)
        {
            it = m_ext2Processor.find(st[i]);
            if (it != m_ext2Processor.end())
            {
                FX_LOG(WARN, "Processor for file type : [%s] already exists.", st[i].c_str());
                m_ext2Processor.erase(it);
            }
            m_ext2Processor.insert(make_pair(st[i], pCreator));
        }
    }
}

DocumentProcessor* DocumentProcessorFactory::createProcessor(const std::string& sIdent) const
{
    CreatorMap::const_iterator it = m_ident2Processor.find(sIdent);
    if (it != m_ident2Processor.end())
    {
        CreatorPtr pCreator = it->second;
        return pCreator->create();
    }
    return NULL;
}

DocumentProcessor* DocumentProcessorFactory::createProcessorByFileExtension(
        const std::string& sExtension) const
{
    CreatorMap::const_iterator it = m_ext2Processor.find(sExtension);
    if (it != m_ext2Processor.end())
    {
        CreatorPtr pCreator = it->second;
        return pCreator->create();
    }
    return NULL;
}

FX_NS_END

