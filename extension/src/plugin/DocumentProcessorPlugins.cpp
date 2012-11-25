#include "firtex/extension/plugin/DocumentProcessorPlugins.h"
#include "firtex/extension/plugin/Plugins.h"
#include "firtex/extension/plugin/PluginFactory.h"
#include "firtex/extension/plugin/ComDocument.h"
#include "firtex/extension/plugin/ComDocumentSchema.h"
#include "firtex/extension/processor/DocumentProcessorFactory.h"
#include "firtex/extension/com/Com.h"
#include "firtex/extension/com/ComPtr.h"

using namespace std;
FX_NS_USE(com);
FX_NS_USE(document);
FX_NS_USE(processor);

FX_NS_DEF(plugin);

SETUP_LOGGER(plugin, DocumentProcessorPlugins);

DocumentProcessorPlugins::DocumentProcessorPlugins(void)
{
}

DocumentProcessorPlugins::~DocumentProcessorPlugins(void)
{
    clear();
}		

DocumentProcessorPlugin* DocumentProcessorPlugins::getProcessorByIdentifier(const string& identifier)
{
    DocumentProcessorPlugin* pProcessor = NULL;	
    FX_CLSID clsid;
    IDocumentProcessorPlugin* pInterface = NULL;
    if (Plugins::instance()->lookupCLSID(identifier, clsid))
    {
        if(Plugins::instance()->lookupEnable(clsid, true))
        {
            bool ret = PluginFactory::instance()->createInstance(clsid, NULL, 
                    IDocumentProcessorPlugin::iid, (void**)&pInterface);
            if(ret)
            {
                pProcessor = new DocumentProcessorPlugin(pInterface);
                pInterface->Release();
                return pProcessor;
            }
        }
        else 
        {
            FX_DEBUG("Pluin [%s] disnabled, ",  GUIDX::encode(&clsid).c_str());
        }
    }
    return NULL;
}

bool DocumentProcessorPlugins::loadPlugin(const PluginPtr& pPlugin)
{
    IDocumentProcessorPlugin* pInterface = NULL;
    bool ret = PluginFactory::instance()->createInstance(pPlugin,
            NULL, IDocumentProcessorPlugin::iid, (void**)&pInterface);
    if (ret)
    {
        DocumentProcessorFactory::instance()->registerProcessor(new DocumentProcessorPlugin::Creator(pInterface));

        return true;
    }
    return false;
}		

void DocumentProcessorPlugins::clear()
{
    PluginMap::iterator iter = m_pluginMap.begin();
    while(iter != m_pluginMap.end())
    {
        iter->second->Release();
        iter++;
    }
    m_pluginMap.clear();
}

DocumentProcessorPlugins::Iterator DocumentProcessorPlugins::iterator()
{
    return DocumentProcessorPlugins::Iterator(this);
}

FX_NS_END
