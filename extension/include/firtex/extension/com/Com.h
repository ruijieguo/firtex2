#ifndef __FX_COM_H_
#define __FX_COM_H_

#include "firtex/common/StdHeader.h"
#include "firtex/common/IntTypes.h"

#include <memory.h>

#ifdef __cplusplus
#define EXTERN_C    extern "C"
#else
#define EXTERN_C    extern
#endif

#if defined(WIN32) 

#define FX_STDMETHODCALLTYPE    __stdcall
#define FX_STDMETHODVCALLTYPE   __cdecl
#define FX_STDAPICALLTYPE       __stdcall
#define FX_STDAPIVCALLTYPE      __cdecl
#define FX_WINOLEAPI            FX_STDAPI //EXTERN_C DECLSPEC_IMPORT FX_HRESULT FX_STDAPICALLTYPE
#define FX_WINOLEAPI_(type)     FX_STDAPI_(type) //EXTERN_C DECLSPEC_IMPORT type FX_STDAPICALLTYPE
#define FX_WINOLEAUTAPI         FX_STDAPI //EXTERN_C DECLSPEC_IMPORT FX_HRESULT FX_STDAPICALLTYPE
#define FX_WINOLEAUTAPI_(type)  FX_STDAPI_(type) //EXTERN_C DECLSPEC_IMPORT type FX_STDAPICALLTYPE
#define                         __RPC_FAR

#else //for linux

#define FX_STDMETHODCALLTYPE
#define FX_STDMETHODVCALLTYPE 
#define FX_STDAPICALLTYPE          
#define FX_STDAPIVCALLTYPE         
#define FX_WINOLEAPI           FX_STDAPI //EXTERN_C DECLSPEC_IMPORT FX_HRESULT FX_STDAPICALLTYPE
#define FX_WINOLEAPI_(type)    FX_STDAPI_(type) //EXTERN_C DECLSPEC_IMPORT type FX_STDAPICALLTYPE
#define FX_WINOLEAUTAPI	       FX_STDAPI
#define FX_WINOLEAUTAPI_(type) FX_STDAPI_(type)
#define                        __RPC_FAR

#endif

#ifdef __cplusplus

#define FX_STDMETHOD(method)            virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE method
#define FX_STDMETHOD_(type,method)      virtual type FX_STDMETHODCALLTYPE method
#define FX_STDMETHODIMP                 FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE
#define FX_STDMETHODIMP_(type)          type FX_STDMETHODCALLTYPE
#define FX_STDAPI                       EXTERN_C FX_NS(com)::FX_HRESULT FX_STDAPICALLTYPE
#define FX_STDAPI_(type)                EXTERN_C type FX_STDAPICALLTYPE
#define FX_STDAPIV                      EXTERN_C FX_NS(com)::FX_HRESULT FX_STDAPIVCALLTYPE
#define FX_STDAPIV_(type)               EXTERN_C type FX_STDAPIVCALLTYPE

#else
#define FX_STDMETHOD(method)       virtual FX_HRESULT FX_STDMETHODCALLTYPE method
#define FX_STDMETHOD_(type,method) virtual type FX_STDMETHODCALLTYPE method

#define FX_STDMETHODIMP                 FX_HRESULT FX_STDMETHODCALLTYPE
#define FX_STDMETHODIMP_(type)          type FX_STDMETHODCALLTYPE
#define FX_STDAPI                       EXTERN_C FX_NS(com)::FX_HRESULT FX_STDAPICALLTYPE
#define FX_STDAPI_(type)                EXTERN_C type FX_STDAPICALLTYPE
#define FX_STDAPIV                      EXTERN_C FX_NS(com)::FX_HRESULT FX_STDAPIVCALLTYPE
#define FX_STDAPIV_(type)               EXTERN_C type FX_STDAPIVCALLTYPE
#define FX_BEGIN_INTERFACE
#define FX_END_INTERFACE

#endif

#ifdef __cplusplus
#define FX_MAKE_HRESULT(x) ((FX_NS(com)::FX_HRESULT)(x))
#else
#define FX_MAKE_HRESULT(x) ((FX_HRESULT)(x))
#endif

//error info define
#define FX_S_OK                         FX_MAKE_HRESULT(0x00000000L)
#define FX_S_FALSE                      FX_MAKE_HRESULT(0x00000001L)

#define FX_CLASS_E_NOAGGREGATION        FX_MAKE_HRESULT(0x80040110L)
#define FX_CLASS_E_CLASSNOTAVAILABLE    FX_MAKE_HRESULT(0x80040111L)

#define FX_CO_S_NOTALLINTERFACES        FX_MAKE_HRESULT(0x00080012L)
#define FX_CO_E_CLASSSTRING             FX_MAKE_HRESULT(0x800401F3L)
#define FX_CO_E_DLLNOTFOUND             FX_MAKE_HRESULT(0x800401F8L)
#define FX_CO_E_OBJISREG                FX_MAKE_HRESULT(0x800401FCL)

#define FX_E_NOTIMPL                    FX_MAKE_HRESULT(0x80004001L)
#define FX_E_NOINTERFACE                FX_MAKE_HRESULT(0x80004002L)
#define FX_E_POINTER                    FX_MAKE_HRESULT(0x80004003L)
#define FX_E_ABORT                      FX_MAKE_HRESULT(0x80004004L)
#define FX_E_FAIL                       FX_MAKE_HRESULT(0x80004005L)
#define FX_E_ACCESSDENIED               FX_MAKE_HRESULT(0x80070005L)
#define FX_E_OUTOFMEMORY                FX_MAKE_HRESULT(0x8007000EL)
#define FX_E_INVALIDARG                 FX_MAKE_HRESULT(0x80070057L)
#define FX_E_UNEXPECTED                 FX_MAKE_HRESULT(0x8000FFFFL)

#define FX_RPC_S_UUID_LOCAL_ONLY        FX_MAKE_HRESULT(1824L)
#define FX_RPC_E_CHANGED_MODE           FX_MAKE_HRESULT(0x80010106L)

#define SEVERITY_SUCCESS    0
#define SEVERITY_ERROR      1
#define FX_SUCCEEDED(Status)   ((Status) >= 0)
#define FX_FAILED(Status)      ((Status) < 0)

#ifdef __cplusplus

FX_NS_DEF(com);

#endif

//////////////////////////////////////////////////////
// Typedefs
//////////////////////////////////////////////////////

typedef int32_t FX_HRESULT;
typedef char FX_CHAR;
typedef wchar_t FX_WCHAR;
typedef uint8_t FX_BOOL;
typedef int32_t FX_INT32;
typedef float FX_FLOAT;
typedef unsigned short FX_BYTE;
typedef wchar_t const* FX_BSTR;

struct FX_ARRAY
{
    FX_BYTE* data;
    FX_INT32 length;
};

struct FX_STR
{
    FX_CHAR* str;
    FX_INT32 length;
};

struct FX_WSTR
{
    FX_WCHAR* str;
    FX_INT32 length;
};

enum FX_VARTYPE
{
    FX_VT_EMPTY = 0,
    FX_VT_BOOL,
    FX_VT_INT32,
    FX_VT_FLOAT,
    FX_VT_ARRAY,
    FX_VT_STRING,   
    FX_VT_WSTRING,  
};

struct FX_VARIANT
{
    FX_VARTYPE vt;
    union 
    {
        FX_BOOL bVal;
        FX_INT32 i32Val;
        FX_FLOAT fVal;
        FX_ARRAY arVal;
        FX_STR sVal;
        FX_WSTR wsVal;
    };
};

//////////////////////////////////////////////////////
// FX_GUID, FX_IID, FX_CLSID
//////////////////////////////////////////////////////
#ifndef FX_GUID_DEFINED

struct FX_GUID
{
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t Data4[8];
};
#endif //FX_GUID_DEFINED

#ifdef __cplusplus
typedef FX_GUID const& FX_REFGUID;
typedef FX_GUID FX_IID;
typedef FX_IID const& FX_REFIID;
typedef FX_GUID FX_CLSID;
typedef FX_CLSID const& FX_REFCLSID;

inline bool FX_IsEqualGUID(FX_REFGUID rguid1, FX_REFGUID rguid2)
{
    return !memcmp(&rguid1, &rguid2, sizeof(FX_GUID));
}
inline bool FX_IsEqualIID(FX_REFIID rguid1, FX_REFIID rguid2)
{
    return !memcmp(&rguid1, &rguid2, sizeof(FX_GUID));
}
inline bool FX_IsEqualCLSID(FX_REFCLSID rguid1, FX_REFCLSID rguid2)
{
    return !memcmp(&rguid1, &rguid2, sizeof(FX_GUID));
}
#else
typedef struct FX_GUID const * FX_REFGUID;
typedef struct FX_GUID FX_IID;
typedef FX_IID const* FX_REFIID;
typedef struct FX_GUID FX_CLSID;
typedef FX_CLSID const* FX_REFCLSID;
#endif

//////////////////////////////////////////////////////
// Interface Forward Declares
//////////////////////////////////////////////////////

#ifdef __cplusplus
#define FORWARD_DECLARE(x)                      \
    class x;
#else
#define FORWARD_DECLARE(x)                      \
    struct x;                                   \
    typedef struct x x;
#endif

FORWARD_DECLARE( IUnknown );
FORWARD_DECLARE( IClassFactory );

#undef FORWARD_DECLARE

//////////////////////////////////////////////////////
// Helpful Defines
//////////////////////////////////////////////////////

#ifndef __cplusplus

#define START_C_INTERFACE( iname )                                      \
    struct iname##Vtbl {                                                \
    FX_BEGIN_INTERFACE                                                  \
    FX_HRESULT ( FX_STDMETHODCALLTYPE __RPC_FAR *QueryInterface )(      \
            struct iname __RPC_FAR * This,                              \
            FX_REFIID riid,                                             \
            void __RPC_FAR *__RPC_FAR *ppvObject);                      \
    uint32_t ( FX_STDMETHODCALLTYPE __RPC_FAR *AddRef )(                \
            struct iname __RPC_FAR * This);                             \
    uint32_t ( FX_STDMETHODCALLTYPE __RPC_FAR *Release )(               \
            struct iname __RPC_FAR * This);

#define END_C_INTERFACE( iname )                        \
    FX_END_INTERFACE                                    \
    };                                                  \
    struct iname {                                      \
        const struct iname##Vtbl __RPC_FAR *lpVtbl;     \
    };                                                  \
    EXTERN_C FX_IID const IID_##iname;

#endif // __cplusplus

//////////////////////////////////////////////////////
// IUnknown
//////////////////////////////////////////////////////

#ifdef __cplusplus
/* uuid {00000000-0000-0000-C000-000000000046} */
class IUnknown 
{
public:
    virtual ~IUnknown() {}
    static const FX_IID iid;

    virtual FX_HRESULT FX_STDMETHODCALLTYPE QueryInterface( /* [in] */ FX_REFIID riid, /* [out] */ void __RPC_FAR *__RPC_FAR *ppvObject ) = 0;
    virtual uint32_t FX_STDMETHODCALLTYPE AddRef( void ) = 0;
    virtual uint32_t FX_STDMETHODCALLTYPE Release( void ) = 0;
};
#else
START_C_INTERFACE( IUnknown )
END_C_INTERFACE( IUnknown );

#endif /* IUnknown */

//////////////////////////////////////////////////////
// IClassFactory
//////////////////////////////////////////////////////

#ifdef __cplusplus
/* uuid {00000001-0000-0000-C000-000000000046} */
class IClassFactory : public IUnknown  
{
public:
    static const FX_IID iid;

    virtual FX_HRESULT FX_STDMETHODCALLTYPE CreateInstance( /* [in] */ IUnknown __RPC_FAR* pUnkOuter,
            /* [in] */ FX_REFIID riid, /* [out] */ void __RPC_FAR *__RPC_FAR * ppvObject ) = 0;
    virtual FX_HRESULT FX_STDMETHODCALLTYPE LockServer( /* [in] */ int32_t fLock ) = 0;
};
#else
START_C_INTERFACE( IClassFactory )
FX_HRESULT ( FX_STDMETHODCALLTYPE __RPC_FAR *CreateInstance )(
        struct IClassFactory __RPC_FAR * This,
	/* [in] */ IUnknown __RPC_FAR* pUnkOuter, 
	/* [in] */ FX_REFIID riid, 
	/* [out] */ void __RPC_FAR *__RPC_FAR * ppvObject);

FX_HRESULT ( FX_STDMETHODCALLTYPE __RPC_FAR *LockServer )(
        struct IClassFactory __RPC_FAR * This,
	/* [in] */ int32_t fLock);
END_C_INTERFACE( IClassFactory );
#endif /* IClassFactory */

#ifdef __cplusplus

FX_NS_END

#endif

#endif // _FX_COM_H_
