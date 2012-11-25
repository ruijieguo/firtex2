#include "firtex/extension/plugin/ComDocumentTemplate.h"
#include "firtex/extension/com/Variant.h"
#include "firtex/extension/plugin/ComFieldTag.h"

FX_NS_USE(com);
FX_NS_USE(processor);

FX_NS_DEF(plugin);

SETUP_STREAM_LOGGER(plugin, ComDocumentTemplate);

ComDocumentTemplate::ComDocumentTemplate(DocumentTemplate* pTemp) 
    : m_pTemp(pTemp)
{
}

ComDocumentTemplate::~ComDocumentTemplate() 
{
    m_pTemp = NULL;
}

IDocumentTemplate* ComDocumentTemplate::wrap(DocumentTemplate* pTemp)
{
    ComDocumentTemplate* pWrap = new ComDocumentTemplate(pTemp);
    CLASS_TO_INTERFACE(ComDocumentTemplate, DocumentTemplate, pWrap, pXThis);
    IDocumentTemplate* pCom = NULL;
    pXThis->QueryInterface(IDocumentTemplate::iid, (void**)&pCom);
    return pCom;
}

DocumentTemplate* ComDocumentTemplate::unwrap(IDocumentTemplate* pInterface)
{
    if ( pInterface == NULL ) return NULL;
    INTERFACE_TO_CLASS(ComDocumentTemplate, DocumentTemplate, pInterface, pWrapper)
    return (DocumentTemplate*)pWrapper->m_pTemp;
}

IDocumentTemplate* ComDocumentTemplate::getInterface()
{
    CLASS_TO_INTERFACE(ComDocumentTemplate, DocumentTemplate, this, pXThis);
    IDocumentTemplate* pCom = NULL;
    pXThis->QueryInterface(IDocumentTemplate::iid, (void**)&pCom);
    return pCom;
}

//////////////////////////////////////////////////////////////////////////
//ComDocumentTemplate IDocumentTemplate
IMPLEMENT_UNKNOWN(ComDocumentTemplate, DocumentTemplate)

FX_STDMETHODIMP ComDocumentTemplate::XDocumentTemplate::getDocTag(/* [out] */ FX_NS(com)::FX_VARIANT** ppDocTag)
{
    FX_METHOD_PROLOGUE(ComDocumentTemplate, DocumentTemplate);
    const std::string& sDocTag = pThis->m_pTemp->getDocTag();
    Variant var(sDocTag);
    *ppDocTag = var.detach();
    return FX_S_OK;
}

FX_STDMETHODIMP ComDocumentTemplate::XDocumentTemplate::getTagCount(/* [out] */ FX_NS(com)::FX_INT32* pTagCount)
{
    FX_METHOD_PROLOGUE(ComDocumentTemplate, DocumentTemplate);
    *pTagCount = (FX_INT32)pThis->m_pTemp->getNumTags();
    return FX_S_OK;
}

FX_STDMETHODIMP ComDocumentTemplate::XDocumentTemplate::getEmbeddedField(/* [out] */ FX_NS(com)::FX_VARIANT** ppField)
{
    FX_METHOD_PROLOGUE(ComDocumentTemplate, DocumentTemplate);
    const std::string& sEmField = pThis->m_pTemp->getEmbeddedField();
    Variant var(sEmField);
    *ppField = var.detach();
    return FX_S_OK;
}

FX_STDMETHODIMP ComDocumentTemplate::XDocumentTemplate::getTag(/* [in] */ FX_NS(com)::FX_INT32 tagIdx, 
                         /* [out] */ IFieldTag** ppTag)
{
    FX_METHOD_PROLOGUE(ComDocumentTemplate, DocumentTemplate);
    const DocumentTemplate::Tag& tag = pThis->m_pTemp->getTag((size_t)tagIdx);
    *ppTag = ComFieldTag::wrap(const_cast<DocumentTemplate::Tag*>(&tag));
    return FX_S_OK;
}

FX_NS_END

