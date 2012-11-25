#include "MSOfficeParserPlugin.h"
#include "firtex/extension/com/ComPtr.h"
#include "firtex/extension/com/DllServer.h"
#include "firtex/extension/com/Variant.h"

FX_NS_USE(com)
FX_NS_DEF(plugin)

namespace MSOfficeParserPluginNS
{
// {CEA6A13B-397B-439a-B1F2-DF447F9C5BB7}
FX_CLSID const clsid = { 0xcea6a13b, 0x397b, 0x439a, { 0xb1, 0xf2, 0xdf, 0x44, 0x7f, 0x9c, 0x5b, 0xb7 } };
wchar_t const VersionIndependentProgId[] = L"FirteX.MSOfficeParserPlugin";
wchar_t const ProgId[] = L"FirteX.MSOfficeParserPlugin.1";
}; // namespace MSOfficeParserPlugin 

//////////////////////////////////////////////////////////////////////////
//MSOfficeParserPlugin
MSOfficeParserPlugin::MSOfficeParserPlugin()
    : m_count(0)
    , m_schemaContentID(-1)
    , m_bNeedMeta(false)
{
    m_fieldName2Id[L"content"] = CONTENT;
    m_fieldName2Id[L"CONTENT"] = CONTENT;
    m_fieldName2Id[L"BODY"] = CONTENT;
    m_fieldName2Id[L"body"] = CONTENT;
}

MSOfficeParserPlugin::MSOfficeParserPlugin(const MSOfficeParserPlugin& src)
    : m_count(0)
    , m_schemaContentID(src.m_schemaContentID)
    , m_bNeedMeta(src.m_bNeedMeta)
{
    m_fieldName2Id[L"content"] = CONTENT;
    m_fieldName2Id[L"CONTENT"] = CONTENT;
    m_fieldName2Id[L"BODY"] = CONTENT;
    m_fieldName2Id[L"body"] = CONTENT;
}

MSOfficeParserPlugin::~MSOfficeParserPlugin()
{
}

FX_STDMETHODIMP_(uint32_t) MSOfficeParserPlugin::AddRef()
{ 
    if (!m_count) 
        DllServer::DllLockServerImpl(true); 
    return ++m_count; 
};

FX_STDMETHODIMP_(uint32_t) MSOfficeParserPlugin::Release()
{ 
    uint32_t c = --m_count; 
    if (!c) 
    {
        DllServer::DllLockServerImpl(false); 
        delete this;
    }
    return c; 
};

FX_STDMETHODIMP MSOfficeParserPlugin::QueryInterface( FX_IID const& riid, void** ptr )
{
    if ( FX_IsEqualIID( riid, IUnknown::iid ) ) 
    {
        *ptr = static_cast<IUnknown*>( this );
        AddRef();
        return FX_S_OK;
    }

    if ( FX_IsEqualIID( riid, IDocumentProcessorPlugin::iid ) ) 
    {
        *ptr = static_cast<IDocumentProcessorPlugin*>( this );
        AddRef();
        return FX_S_OK;
    }

    return FX_E_NOINTERFACE;
}


FX_STDMETHODIMP MSOfficeParserPlugin::getIdentifier(/* [out] */FX_BSTR* pIdent)
{
    if (!pIdent)
        return FX_E_POINTER;
    ComStr tmp(L"processor.msoffice");
    if (!tmp)
        return FX_E_OUTOFMEMORY;
    *pIdent = tmp.detach();
    return FX_S_OK;
}

FX_STDMETHODIMP MSOfficeParserPlugin::getDocType(/* [out] */FX_BSTR* retVal)
{
    if (!retVal)
        return FX_E_POINTER;
    ComStr tmp(L"doc;ppt;xls");
    if (!tmp)
        return FX_E_OUTOFMEMORY;
    *retVal = tmp.detach();
    return FX_S_OK;
}

FX_STDMETHODIMP MSOfficeParserPlugin::init(/* [in] */ IDocumentSchema* pDocSchema,
        /* [in] */ IDocumentTemplate* pDocTemp)
{
    for (NameMap::const_iterator it = m_fieldName2Id.begin();
         it != m_fieldName2Id.end(); ++it)
    {
        FX_INT32 fieldId = -1;
        if (pDocSchema->getFieldId(it->first, &fieldId) == FX_S_OK
            && fieldId != -1)
        {
            m_schemaContentID = fieldId;
            break;
        }
    }
    return FX_S_OK;
}

FX_STDMETHODIMP MSOfficeParserPlugin::process(/* [in] */ IDocumentSource* pDocSource)
{
    IRawDocument* pRawDoc = NULL;
    if (pDocSource->getRawDocument(&pRawDoc) != FX_S_OK)
    {
        return FX_S_FALSE;
    }

    FX_BOOL bIsPath = 0;
    if (pRawDoc->isPath(&bIsPath) != FX_S_OK)
    {
        return FX_S_FALSE;
    }

    IDocument* pResultDoc = NULL;
    ComStr procName("msoffice_plugin");
    if (pDocSource->acquireDocument(procName.get(), &pResultDoc) != FX_S_OK)
    {
        pRawDoc->Release();
        return FX_S_FALSE;
    }

    if (!bIsPath)
    {
        return FX_E_FAIL;
    }

    FX_VARIANT* pPath;
    if (pRawDoc->getPath(&pPath) == FX_S_OK)
    {
        if (pPath->vt != FX_VT_EMPTY)
        {
            Variant str(pPath);
            std::string sTmpPath;
            str.copyTo(sTmpPath);
            char* pContent = m_stripper.strip(sTmpPath.c_str());
            if (pContent)
            {
                FX_VARIANT varStr;
                varStr.vt = FX_VT_STRING;
                varStr.sVal.str = pContent;
                varStr.sVal.length = m_stripper.getLength();
                pResultDoc->addField(m_schemaContentID, &varStr, 0);
                return FX_S_OK;
            }
        }
    }

    return FX_S_FALSE;
}

FX_STDMETHODIMP MSOfficeParserPlugin::clone(/* [out] */IDocumentProcessorPlugin** ppPlugin)
{
    MSOfficeParserPlugin* pClone = new MSOfficeParserPlugin(*this);
    pClone->AddRef();
    *ppPlugin = pClone;
    return FX_S_OK;
}

FX_NS_END
