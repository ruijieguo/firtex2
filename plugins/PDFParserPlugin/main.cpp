#include "PDFParserPlugin.h"

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

FX_STDAPI DllGetClassObject(FX_REFCLSID rclsid, FX_REFIID riid, void** ppv)
{
    static PDFParserFactory factory;

    // find the right UUID
    //if ( FX_IsEqualCLSID( rclsid, IParserPlugin::clsid ) )
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
    return DllServer::DllRegisterServerImpl( pdfparserplugin::clsid, 
            L"PDF Parser For FirteX",
            pdfparserplugin::VersionIndependentProgId, 
            pdfparserplugin::ProgId);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

FX_STDAPI DllUnregisterServer(void)
{
    return DllServer::DllUnregisterServerImpl( pdfparserplugin::clsid );
}

int main()
{
    return 0;
}
