#ifndef __DLLSERVER_H
#define __DLLSERVER_H

#include "firtex/common/StdHeader.h"
#include "firtex/extension/com/Com.h"

FX_NS_DEF(com);

class DllServer
{
public:
    static FX_HRESULT DllCanUnloadNowImpl();
    static FX_HRESULT DllLockServerImpl( int32_t fLock );
    static FX_HRESULT DllRegisterServerImpl(FX_REFCLSID clsid, wchar_t const* name,
                                            wchar_t const* VersionIndependentProgId,
                                            wchar_t const* ProgId);
    static FX_HRESULT DllUnregisterServerImpl(FX_REFCLSID clsid);

private:
    static int volatile ms_lockCount;
};

FX_NS_END

#endif
