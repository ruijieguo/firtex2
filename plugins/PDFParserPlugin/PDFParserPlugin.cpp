#include "PDFParserPlugin.h"
#include "PDFDocReader.h"
#include "firtex/extension/com/DllServer.h"
#include "firtex/extension/com/Variant.h"

FX_NS_USE(com);

FX_NS_DEF(plugin);

namespace pdfparserplugin
{
// {DC8E0FBE-9DFF-4f99-8F2D-525EDE968392}
FX_CLSID const clsid = { 0xdc8e0fbe, 0x9dff, 0x4f99, { 0x8f, 0x2d, 0x52, 0x5e, 0xde, 0x96, 0x83, 0x92 } };
wchar_t const VersionIndependentProgId[] = L"FirteX.PDFParserPlugin";
wchar_t const ProgId[] = L"FirteX.PDFParserPlugin.1";
}; // namespace pdfparserplugin 

//////////////////////////////////////////////////////////////////////////
//PDFParserPlugin
PDFParserPlugin::PDFParserPlugin()
    : m_count(0)
    , m_pReader(NULL)
    , m_bNeedMeta(false)
{
    m_fieldName2Id[L"TITLE"] = TITLE;
    m_fieldName2Id[L"title"] = TITLE;

    m_fieldName2Id[L"SUBJECT"] = SUBJECT;
    m_fieldName2Id[L"KEYWORDS"] = KEYWORDS;
    m_fieldName2Id[L"AUTHOR"] = AUTHOR;
    m_fieldName2Id[L"CREATOR"] = CREATOR;
    m_fieldName2Id[L"PRODUCER"] = PRODUCER;
    m_fieldName2Id[L"CREATIONDATE"] = CREATIONDATE;
    m_fieldName2Id[L"LASTMODIFIEDDATE"] = LASTMODIFIEDDATE;

    m_fieldName2Id[L"CONTENT"] = CONTENT;
    m_fieldName2Id[L"content"] = CONTENT;
    m_fieldName2Id[L"BODY"] = CONTENT;
    m_fieldName2Id[L"body"] = CONTENT;

    for(size_t i = 0; i < CONTENT + 1; i++)
    {
        m_schemaIds[i] = -1;
    }
}

PDFParserPlugin::PDFParserPlugin(const PDFParserPlugin& src)
    : m_count(0)
    , m_pReader(NULL)
    , m_bNeedMeta(src.m_bNeedMeta)
{
    for(size_t i = 0; i < CONTENT + 1; i++)
    {
        m_schemaIds[i] = src.m_schemaIds[i];
    }

    for (NameMap::const_iterator it = src.m_fieldName2Id.begin();
         it != src.m_fieldName2Id.end(); ++it)
    {
        m_fieldName2Id.insert(make_pair(it->first, it->second));
    }
}

PDFParserPlugin::~PDFParserPlugin()
{
    if (m_pReader)
    {
        delete m_pReader;
        m_pReader = NULL;
    }
}

FX_STDMETHODIMP_(uint32_t) PDFParserPlugin::AddRef()
{ 
    if ( !m_count ) 
    {
        DllServer::DllLockServerImpl(true);
    }
    return ++m_count; 
};

FX_STDMETHODIMP_(uint32_t) PDFParserPlugin::Release()
{ 
    uint32_t c = --m_count; 
    if ( !c ) 
    {
        DllServer::DllLockServerImpl(false); 
        delete this;
    }
    return c; 
};

FX_STDMETHODIMP PDFParserPlugin::QueryInterface( FX_IID const& riid, void** ptr )
{
    if (FX_IsEqualIID( riid, IUnknown::iid ))
    {
        *ptr = static_cast<IUnknown*>(this);
        AddRef();
        return FX_S_OK;
    }

    if (FX_IsEqualIID( riid, FX_NS(plugin)::IDocumentProcessorPlugin::iid))
    {
        *ptr = static_cast<FX_NS(plugin)::IDocumentProcessorPlugin*>( this );
        AddRef();
        return FX_S_OK;
    }

    return FX_E_NOINTERFACE;
}

FX_STDMETHODIMP PDFParserPlugin::getIdentifier(/* [out] */FX_BSTR* pIdent)
{
    if (!pIdent)
        return FX_E_POINTER;
    ComStr tmp(L"processor.pdf");
    if (!tmp)
        return FX_E_OUTOFMEMORY;
    *pIdent = tmp.detach();
    return FX_S_OK;
}

FX_STDMETHODIMP PDFParserPlugin::getDocType(/* [out] */FX_BSTR* retVal)
{
    if (!retVal)
        return FX_E_POINTER;
    ComStr tmp(L"pdf");
    if (!tmp)
        return FX_E_OUTOFMEMORY;
    *retVal = tmp.detach();
    return FX_S_OK;
}

FX_STDMETHODIMP PDFParserPlugin::init(/* [in] */ IDocumentSchema* pDocSchema,
                                      /* [in] */ IDocumentTemplate* pDocTemp)
{
    for (NameMap::const_iterator it = m_fieldName2Id.begin();
         it != m_fieldName2Id.end(); ++it)
    {
        FX_INT32 fieldId = -1;
        if (pDocSchema->getFieldId(it->first, &fieldId) == FX_S_OK
            && fieldId != -1)
        {
            m_schemaIds[it->second] = fieldId;
        }
    }
    return FX_S_OK;
}

FX_STDMETHODIMP PDFParserPlugin::process(/* [in] */ IDocumentSource* pDocSource)
{
    if (m_pReader == NULL)
    {
        m_pReader = new PDFDocReader();
    }
    else 
    {
        m_pReader->close();
    }

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
    ComStr procName("pdf_plugin");
    if (pDocSource->acquireDocument(procName.get(), &pResultDoc) != FX_S_OK)
    {
        pRawDoc->Release();
        return FX_S_FALSE;
    }

    if (bIsPath)
    {
        FX_VARIANT* pPath;
        if (pRawDoc->getPath(&pPath) == FX_S_OK)
        {
            Variant tmp(pPath);
            std::string sTmp;
            tmp.copyTo(sTmp);
            if (m_pReader->open(sTmp.c_str(), (m_schemaIds[TITLE] != -1), false))
            {
                FX_VARIANT varStr;
                varStr.vt = FX_NS(com)::FX_VT_STRING;
                if (m_schemaIds[CONTENT] != -1)
                {
                    size_t length = 0;
                    varStr.sVal.str = m_pReader->getText(length);
                    varStr.sVal.length = length;
                    pResultDoc->addField(m_schemaIds[CONTENT], &varStr, 0);
                }

                if (m_pReader->getMetaData())
                {
                    PDFMetadata* pMeta = m_pReader->getMetaData();
                    if (m_schemaIds[TITLE] != -1 )
                    {
                        varStr.sVal.str = const_cast<char*>(pMeta->m_sTitle.c_str());
                        varStr.sVal.length = pMeta->m_sTitle.length();
                        pResultDoc->addField(m_schemaIds[TITLE], &varStr,0);
                    }
                    if (m_schemaIds[SUBJECT] != -1)
                    {
                        varStr.sVal.str = const_cast<char*>(pMeta->m_sTitle.c_str());
                        varStr.sVal.length = pMeta->m_sTitle.length();
                        pResultDoc->addField(m_schemaIds[SUBJECT], &varStr,0);
                    }
                    if (m_schemaIds[SUBJECT] != -1)
                    {
                        varStr.sVal.str = const_cast<char*>(pMeta->m_sSubject.c_str());
                        varStr.sVal.length = pMeta->m_sSubject.length();
                        pResultDoc->addField(m_schemaIds[SUBJECT], &varStr,0);
                    }
                    if (m_schemaIds[KEYWORDS] != -1)
                    {
                        varStr.sVal.str = const_cast<char*>(pMeta->m_sKeywords.c_str());
                        varStr.sVal.length = pMeta->m_sKeywords.length();
                        pResultDoc->addField(m_schemaIds[KEYWORDS], &varStr,0);
                    }
                    if (m_schemaIds[AUTHOR] != -1)
                    {
                        varStr.sVal.str = const_cast<char*>(pMeta->m_sAuthor.c_str());
                        varStr.sVal.length = pMeta->m_sAuthor.length();
                        pResultDoc->addField(m_schemaIds[AUTHOR], &varStr,0);
                    }
                    if (m_schemaIds[CREATOR] != -1)
                    {
                        varStr.sVal.str = const_cast<char*>(pMeta->m_sCreator.c_str());
                        varStr.sVal.length = pMeta->m_sCreator.length();
                        pResultDoc->addField(m_schemaIds[CREATOR], &varStr,0);
                    }
                    if (m_schemaIds[PRODUCER] != -1)
                    {
                        varStr.sVal.str = const_cast<char*>(pMeta->m_sProducer.c_str());
                        varStr.sVal.length = pMeta->m_sProducer.length();
                        pResultDoc->addField(m_schemaIds[PRODUCER], &varStr,0);
                    }
                    if (m_schemaIds[CREATIONDATE] != -1)
                    {
                        varStr.sVal.str = const_cast<char*>(pMeta->m_sCreationDate.c_str());
                        varStr.sVal.length = pMeta->m_sCreationDate.length();
                        pResultDoc->addField(m_schemaIds[CREATIONDATE], &varStr,0);
                    }
                    if (m_schemaIds[LASTMODIFIEDDATE] != -1)
                    {
                        varStr.sVal.str = const_cast<char*>(pMeta->m_sLastModifiedDate.c_str());
                        varStr.sVal.length = pMeta->m_sLastModifiedDate.length();
                        pResultDoc->addField(m_schemaIds[LASTMODIFIEDDATE], &varStr,0);
                    }
                }
                return FX_S_OK;
            }
        }
        return FX_E_FAIL;
    }
	return FX_E_FAIL;
}
 
FX_STDMETHODIMP PDFParserPlugin::clone(/* [out] */IDocumentProcessorPlugin** ppPlugin)
{
    PDFParserPlugin* pClone = new PDFParserPlugin(*this);
    pClone->AddRef();
    *ppPlugin = pClone;
    return FX_S_OK;
}

FX_NS_END
