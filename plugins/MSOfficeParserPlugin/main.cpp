#include "MSOfficeParserPlugin.h"

#include "firtex/extension/com/Com.h"
#include "firtex/extension/com/ComPtr.h"
#include "firtex/extension/com/DllServer.h"

FX_NS_USE(plugin);
FX_NS_USE(com);
/////////////////////////////////////////////////////////////////////////////
// Determines whether the DLL that implements this function is in use.
FX_STDAPI DllCanUnloadNow()
{
    return DllServer::DllCanUnloadNowImpl();
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

FX_STDAPI DllGetClassObject(FX_NS(com)::FX_REFCLSID rclsid, FX_NS(com)::FX_REFIID riid, void** ppv)
{
    static CMSOfficeParserFactory factory;

    // find the right UUID
    // if ( FX_NS(com)::FX_IsEqualCLSID( rclsid, FX_NS(plugin)::IParserPlugin::clsid ) )
    {
        return factory.QueryInterface( riid, ppv );
    }

    return FX_CLASS_E_CLASSNOTAVAILABLE;
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

FX_STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    return DllServer::DllRegisterServerImpl( FX_NS(plugin)::MSOfficeParserPluginNS::clsid, 
            L"Microsof Office Parser For FirteX",
            FX_NS(plugin)::MSOfficeParserPluginNS::VersionIndependentProgId, 
            FX_NS(plugin)::MSOfficeParserPluginNS::ProgId);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

FX_STDAPI DllUnregisterServer(void)
{
    return DllServer::DllUnregisterServerImpl( FX_NS(plugin)::MSOfficeParserPluginNS::clsid );
}

int main()
{
    return 0;
}
