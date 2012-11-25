#include "firtex/extension/plugin/PluginFactory.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/common/Logger.h"
#include "firtex/extension/plugin/Plugins.h"
#include "firtex/utility/Path.h"

#ifdef FX_WINDOWS
#else
#include <dlfcn.h>
#endif

using namespace std;

FX_NS_USE(com);
FX_NS_USE(utility);

FX_NS_DEF(plugin);

SETUP_STREAM_LOGGER(parser, PluginFactory);

bool PluginFactory::createInstance(FX_REFCLSID rclsid, IUnknown* pUnkOuter,
                                   FX_REFIID iid, void** ppv)
{
    if (ppv == NULL)
    {
        return false;
    }
    *ppv = NULL;
    IClassFactory* lpclf = NULL;
    bool ret = getClassObject(rclsid, IClassFactory::iid, (void**)&lpclf);
    if (!ret)
    {
        return false;
    }
    FX_HRESULT hr = lpclf->CreateInstance(pUnkOuter, iid, ppv);
    lpclf->Release();
    return (!FX_FAILED(hr));
}

bool PluginFactory::createInstance(const PluginPtr& pPlugin, IUnknown* pUnkOuter, 
                    FX_REFIID iid, void** ppv)
{
    if (ppv == NULL)
    {
        return false;
    }
    *ppv = NULL;
	IClassFactory* lpclf = NULL;
    bool ret = getClassObject(pPlugin, IClassFactory::iid, (void**)&lpclf);
    if (!ret)
    {
        return false;
    }
    FX_HRESULT hr = lpclf->CreateInstance(pUnkOuter, iid, ppv);
    lpclf->Release();
    return (!FX_FAILED(hr));
}

bool PluginFactory::getClassObject(FX_REFCLSID rclsid, FX_REFIID iid, void** ppv)
{
    PluginPtr pPlugin = Plugins::instance()->findByCLSID(rclsid);
    if (!pPlugin)
    {
        return false;
    }

    return getClassObject(pPlugin, iid, ppv);
}

bool PluginFactory::getClassObject(const PluginPtr& pPlugin, FX_REFIID iid, void** ppv)
{
    SCOPED_LOCK();

    Path p(GLOBAL_CONF().General.pluginPath);
    p.makeDirectory();
    p.setFileName(pPlugin->getFileName());
    p.setExtension(SharedLibrary::suffix());

    SharedLibraryPtr sharedLib;

    LibraryMap::const_iterator it = m_loadedLibs.find(pPlugin->getFileName());
    if (it != m_loadedLibs.end())
    {
        sharedLib = it->second;
    }
    else 
    {
        sharedLib.assign(new SharedLibrary);
        try 
        {
            sharedLib->load(p.toString());
        }
        catch(const FirteXException& e)
        {
            FX_STREAM_LOG(ERROR) << _T("Load library FAILED: ") 
                                    << p.toString() << _T(", Error code: ") 
                                    << e.what() << FIRTEX_ENDL;
            return false;
        }
    }

    FX_HRESULT (FX_STDAPICALLTYPE *GetClassObject)(FX_REFCLSID rclsid, FX_REFIID riid, void** ppv) = 0;
    *(void**)&GetClassObject = sharedLib->getSymbol("DllGetClassObject");
    if ( !GetClassObject ) 
    {
        FX_STREAM_LOG(ERROR) << _T("Could not find the DllGetClassObject entry point in ")
                                << p.toString() << FIRTEX_ENDL;
        return false;
    }
    
    m_loadedLibs.insert(make_pair(pPlugin->getFileName(), sharedLib));

	IUnknown* ppUnk = NULL;
    FX_HRESULT hr = GetClassObject(pPlugin->getCLSID(), IUnknown::iid, (void**)&ppUnk);
    if (FX_FAILED(hr))
    {
        return false;
    }
    hr = ppUnk->QueryInterface(iid, ppv);
    ppUnk->Release();
    return (!FX_FAILED(hr));
}

FX_NS_END;
