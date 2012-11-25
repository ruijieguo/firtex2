#include "firtex/extension/plugin/ComRawDocument.h"
#include "firtex/extension/com/Variant.h"

FX_NS_USE(processor);
FX_NS_USE(com);

FX_NS_DEF(plugin);

SETUP_STREAM_LOGGER(plugin, ComRawDocument);

ComRawDocument::ComRawDocument(RawDocument* pRawDoc) 
    : m_pRawDoc(pRawDoc)
{
}

ComRawDocument::~ComRawDocument() 
{
    m_pRawDoc = NULL;
}

IRawDocument* ComRawDocument::wrap(RawDocument* pRawDoc)
{
    ComRawDocument* pWrap = new ComRawDocument(pRawDoc);
    CLASS_TO_INTERFACE(ComRawDocument, RawDocument, pWrap, pXThis);
    IRawDocument* pCom = NULL;
    pXThis->QueryInterface(IRawDocument::iid, (void**)&pCom);
    return pCom;
}

RawDocument* ComRawDocument::unwrap(IRawDocument* pInterface)
{
    if ( pInterface == NULL ) return NULL;
    INTERFACE_TO_CLASS(ComRawDocument, RawDocument, pInterface, pWrapper)
    return (RawDocument*)pWrapper->m_pRawDoc;
}

IRawDocument* ComRawDocument::getInterface()
{
    CLASS_TO_INTERFACE(ComRawDocument, RawDocument, this, pXThis);
    IRawDocument* pCom = NULL;
    pXThis->QueryInterface(IRawDocument::iid, (void**)&pCom);
    return pCom;
}

//////////////////////////////////////////////////////////////////////////
//ComRawDocument IRawDocument
IMPLEMENT_UNKNOWN(ComRawDocument, RawDocument)
FX_STDMETHODIMP ComRawDocument::XRawDocument::isPath(/* [out] */ FX_NS(com)::FX_BOOL* pPath)
{
    FX_METHOD_PROLOGUE(ComRawDocument, RawDocument);
    *pPath = (pThis->m_pRawDoc->isPath() == true) ? 1 : 0;
    return FX_S_OK;
}

FX_STDMETHODIMP ComRawDocument::XRawDocument::isContent(/* [out] */ FX_NS(com)::FX_BOOL* pContent)
{
    FX_METHOD_PROLOGUE(ComRawDocument, RawDocument);
    *pContent = (pThis->m_pRawDoc->isContent() == true) ? 1 : 0;
    return FX_S_OK;
}

FX_STDMETHODIMP ComRawDocument::XRawDocument::getPath(/* [out] */ FX_NS(com)::FX_VARIANT** ppPath)
{
    FX_METHOD_PROLOGUE(ComRawDocument, RawDocument);
    const std::string& sPath = pThis->m_pRawDoc->getPath();
    Variant var(sPath);
    *ppPath = var.detach();
    return FX_S_OK;
}

FX_STDMETHODIMP ComRawDocument::XRawDocument::setPath(/* [in] */ FX_NS(com)::FX_VARIANT* pPath)
{
    FX_METHOD_PROLOGUE(ComRawDocument, RawDocument);
    std::string sPath;
    Variant var(pPath);
    var.copyTo(sPath);
    var.detach();
    pThis->m_pRawDoc->setPath(sPath);

    return FX_S_OK;
}

FX_STDMETHODIMP ComRawDocument::XRawDocument::getContent(/* [out] */ FX_NS(com)::FX_VARIANT** ppContent)
{
    FX_METHOD_PROLOGUE(ComRawDocument, RawDocument);
    const std::string& sCont = pThis->m_pRawDoc->getContent();
    Variant var(sCont);
    *ppContent = var.detach();
    return FX_S_OK;
}

FX_STDMETHODIMP ComRawDocument::XRawDocument::setContent(/* [in] */ FX_NS(com)::FX_VARIANT* pContent)
{
    FX_METHOD_PROLOGUE(ComRawDocument, RawDocument);
    Variant var(pContent);
    std::string str;
    var.copyTo(str);
    pThis->m_pRawDoc->setContent(str.c_str(), str.length());
    return FX_S_OK;
}

FX_STDMETHODIMP ComRawDocument::XRawDocument::clear()
{
    FX_METHOD_PROLOGUE(ComRawDocument, RawDocument);
    pThis->m_pRawDoc->clear();
    return FX_S_OK;
}

FX_NS_END

