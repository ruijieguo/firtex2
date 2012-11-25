#ifndef __PLUGIN_FACTORY_H
#define __PLUGIN_FACTORY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/thread/SynchronizedObject.h"
#include "firtex/utility/Singleton.h"
#include "firtex/extension/com/Com.h"
#include "firtex/extension/plugin/SharedLibrary.h"
#include "firtex/extension/plugin/Plugin.h" 

#include <map>

FX_NS_DEF(plugin);

class PluginFactory : public FX_NS(thread)::SynchronizedObject ,
                      public FX_NS(utility)::Singleton<PluginFactory>
{
public:
    bool createInstance(FX_NS(com)::FX_REFCLSID rclsid, 
                        FX_NS(com)::IUnknown* pUnkOuter, 
                        FX_NS(com)::FX_REFIID iid,
                        void** ppv);

    bool createInstance(const PluginPtr& pPlugin, 
                        FX_NS(com)::IUnknown* pUnkOuter, 
                        FX_NS(com)::FX_REFIID iid,
                        void** ppv);

protected:
    bool getClassObject(FX_NS(com)::FX_REFCLSID rclsid, 
                        FX_NS(com)::FX_REFIID iid, void** ppv);

    bool getClassObject(const PluginPtr& pPlugin, FX_NS(com)::FX_REFIID iid, void** ppv);

private:
    typedef std::map<tstring, SharedLibraryPtr> LibraryMap;

    LibraryMap m_loadedLibs;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif
