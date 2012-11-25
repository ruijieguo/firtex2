#ifndef _MSOFFICEPARSERPLUGIN_H
#define _MSOFFICEPARSERPLUGIN_H

#include "firtex/extension/com/Com.h"
#include "firtex/extension/com/ComPtr.h"
#include "firtex/extension/plugin/FirteXCOM.h"
#include "MSOfficeTextStripper.h"
#include <map>

FX_NS_DEF(plugin)
	
class MSOfficeParserPlugin : public IDocumentProcessorPlugin
{
public:
    const static fieldid_t CONTENT = 1;

public:
    MSOfficeParserPlugin();
	MSOfficeParserPlugin(const MSOfficeParserPlugin& src);
    ~MSOfficeParserPlugin();

public:
    FX_STDMETHOD_(uint32_t,AddRef)();
    FX_STDMETHOD_(uint32_t,Release)();
    FX_STDMETHOD(QueryInterface)( FX_NS(com)::FX_IID const& riid, void** ptr );

    FX_STDMETHOD(init)(/* [in] */ IDocumentSchema* pDocSchema,
                       /* [in] */ IDocumentTemplate* pDocTemp);
    FX_STDMETHOD(getIdentifier)(/* [out] */ FX_NS(com)::FX_BSTR* pIdent);
    FX_STDMETHOD(getDocType)(/* [out] */ FX_NS(com)::FX_BSTR* pDocType);
    FX_STDMETHOD(process)(/* [in] */ IDocumentSource* pDocSource);
    FX_STDMETHOD(clone)(/* [out] */IDocumentProcessorPlugin** ppPlugin);
    
private:
    typedef std::map<FX_NS(com)::ComStr, fieldid_t> NameMap;

    unsigned long m_count;
    int32_t m_schemaContentID;
    MSOfficeTextStripper m_stripper;

    bool m_bNeedMeta;
    NameMap m_fieldName2Id;
};

/* uuid:  {CEA6A13B-397B-439a-B1F2-DF447F9C5BB7} */
/* VersionIndependentProgId: FirteX.MSOfficeParserPlugin */
/* ProgId: FirteX.MSOfficeParserPlugin.1 */
namespace MSOfficeParserPluginNS
{
extern const FX_NS(com)::FX_CLSID clsid;
extern const wchar_t VersionIndependentProgId[];
extern const wchar_t ProgId[];
};


class CMSOfficeParserFactory: public FX_NS(com)::IClassFactory
{
public:
    CMSOfficeParserFactory() { m_count = 0; };

    FX_STDMETHOD_(uint32_t,AddRef)();
    FX_STDMETHOD_(uint32_t,Release)();
    FX_STDMETHOD(QueryInterface)( FX_NS(com)::FX_IID const& riid, void** ptr );
    FX_STDMETHOD(LockServer)(int32_t fLock);
    FX_STDMETHOD(CreateInstance)(FX_NS(com)::IUnknown* pUnkOuter,
                                 FX_NS(com)::FX_REFIID riid, void** ppvObject);
private:
    unsigned long m_count;
};

FX_NS_END

#endif
