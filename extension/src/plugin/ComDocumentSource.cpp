#include "firtex/extension/plugin/ComDocumentSource.h"
#include "firtex/extension/plugin/ComRawDocument.h"
#include "firtex/extension/plugin/ComDocument.h"
#include "firtex/extension/com/ComPtr.h"

FX_NS_USE(processor);
FX_NS_USE(document);
FX_NS_USE(com);

FX_NS_DEF(plugin);

SETUP_STREAM_LOGGER(plugin, ComDocumentSource);

ComDocumentSource::ComDocumentSource(DocumentSource* pDocSrc) 
    : m_pDocSrc(pDocSrc)
{
}

ComDocumentSource::~ComDocumentSource() 
{
    m_pDocSrc = NULL;
}

IDocumentSource* ComDocumentSource::wrap(DocumentSource* pDocSrc)
{
    ComDocumentSource* pWrap = new ComDocumentSource(pDocSrc);
    CLASS_TO_INTERFACE(ComDocumentSource, DocumentSource, pWrap, pXThis);
    IDocumentSource* pCom = NULL;
    pXThis->QueryInterface(IDocumentSource::iid, (void**)&pCom);
    return pCom;
}

DocumentSource* ComDocumentSource::unwrap(IDocumentSource* pInterface)
{
    if ( pInterface == NULL ) return NULL;
    INTERFACE_TO_CLASS(ComDocumentSource, DocumentSource, pInterface, pWrapper)
    return (DocumentSource*)pWrapper->m_pDocSrc;
}

IDocumentSource* ComDocumentSource::getInterface()
{
    CLASS_TO_INTERFACE(ComDocumentSource, DocumentSource, this, pXThis);
    IDocumentSource* pCom = NULL;
    pXThis->QueryInterface(IDocumentSource::iid, (void**)&pCom);
    return pCom;
}

//////////////////////////////////////////////////////////////////////////
//ComDocumentSource IDocumentSource
IMPLEMENT_UNKNOWN(ComDocumentSource, DocumentSource)

FX_STDMETHODIMP ComDocumentSource::XDocumentSource::setRawDocument(/* [in] */ IRawDocument* pRawDoc)
{
    FX_METHOD_PROLOGUE(ComDocumentSource, DocumentSource);
    RawDocument* pUnwrappedRawDoc = ComRawDocument::unwrap(pRawDoc);
    pThis->m_pDocSrc->setRawDocument(pUnwrappedRawDoc);
    return FX_S_OK;
}

FX_STDMETHODIMP ComDocumentSource::XDocumentSource::getRawDocument(/* [out] */ IRawDocument** ppRawDoc)
{
    FX_METHOD_PROLOGUE(ComDocumentSource, DocumentSource);
    const RawDocumentPtr& pOrgRawDoc = pThis->m_pDocSrc->getRawDocument();
    *ppRawDoc = ComRawDocument::wrap(const_cast<RawDocument*>(pOrgRawDoc.get()));
    return FX_S_OK;
}

FX_STDMETHODIMP ComDocumentSource::XDocumentSource::acquireDocument(
        /* [in] */ FX_NS(com)::FX_BSTR processorName,
        /* [out] */ IDocument** ppDocument)
{
    ComStr comStrName(processorName);
    string sProcName;
    comStrName.copyTo(sProcName);

    FX_METHOD_PROLOGUE(ComDocumentSource, DocumentSource);
    DocumentPtr pDoc = pThis->m_pDocSrc->acquireDocument(sProcName);
    *ppDocument = ComDocument::wrap(pDoc.get());
    return FX_S_OK;
}

FX_STDMETHODIMP ComDocumentSource::XDocumentSource::stealLastDocument(/* [out] */ IDocument** ppDocument)
{
    FX_METHOD_PROLOGUE(ComDocumentSource, DocumentSource);
    DocumentPtr pDoc = pThis->m_pDocSrc->stealLastDocument();
    *ppDocument = ComDocument::wrap(pDoc.get());
    return FX_S_OK;
}

FX_STDMETHODIMP ComDocumentSource::XDocumentSource::toBeContinued(/* [out] */ FX_NS(com)::FX_BOOL* bContinue)
{
    FX_METHOD_PROLOGUE(ComDocumentSource, DocumentSource);
    *bContinue = (pThis->m_pDocSrc->toBeContinued() == true) ? 1 : 0;
    return FX_S_OK;    
}

FX_STDMETHODIMP ComDocumentSource::XDocumentSource::setToBeContinued(
        /* [in] */ FX_NS(com)::FX_BOOL bContinue)
{
    FX_METHOD_PROLOGUE(ComDocumentSource, DocumentSource);
    pThis->m_pDocSrc->setToBeContinued((bContinue == 1) ? true : false);
    return FX_S_OK;
}

FX_STDMETHODIMP ComDocumentSource::XDocumentSource::reset()
{
    FX_METHOD_PROLOGUE(ComDocumentSource, DocumentSource);
    pThis->m_pDocSrc->reset();
    return FX_S_OK;
}

FX_NS_END

