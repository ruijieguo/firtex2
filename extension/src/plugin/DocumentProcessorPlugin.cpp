#include "firtex/extension/plugin/DocumentProcessorPlugin.h"
#include "firtex/extension/plugin/ComDocumentSchema.h"
#include "firtex/extension/plugin/ComDocumentTemplate.h"
#include "firtex/extension/plugin/ComDocumentSource.h"

FX_NS_USE(processor);
FX_NS_USE(document);

FX_NS_DEF(plugin);

SETUP_LOGGER(plugin, DocumentProcessorPlugin);

DocumentProcessorPlugin::DocumentProcessorPlugin(IDocumentProcessorPlugin* pInterface)
    : m_pInterface(pInterface)
{
}

DocumentProcessorPlugin::~DocumentProcessorPlugin() 
{
    m_pInterface->Release();
    m_pInterface = NULL;
}

void DocumentProcessorPlugin::init(const DocumentSchema* pDocSchema,
                                   const DocumentTemplate* pTemplate)
{
    ComDocumentSchema docSchemaWrapper(const_cast<DocumentSchema*>(pDocSchema));
    ComDocumentTemplate docTempWrapper(const_cast<DocumentTemplate*>(pTemplate));
    
    if (m_pInterface->init(docSchemaWrapper.getInterface(), 
                           docTempWrapper.getInterface()) != FX_S_OK)
    {
        FIRTEX_THROW(RuntimeException, "Init document processor plugin FAILED");
    }
}

void DocumentProcessorPlugin::process(DocumentSource& docSource)
{
    ComDocumentSource docSrcWrapper(&docSource);
    if (m_pInterface->process(docSrcWrapper.getInterface()) != FX_S_OK)
    {
        FX_LOG(WARN, "Document processor process document FAILED"); 
    }
}

DocumentProcessor* DocumentProcessorPlugin::clone() const
{
    IDocumentProcessorPlugin* pPlugin = NULL;
    FX_NS(com)::FX_HRESULT ret = m_pInterface->clone(&pPlugin);
    if (ret == FX_S_OK)
    {
        return new DocumentProcessorPlugin(pPlugin);
    }
    return NULL;
}

FX_NS_END

