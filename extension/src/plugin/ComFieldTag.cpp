#include "firtex/extension/plugin/ComFieldTag.h"
#include "firtex/extension/com/Variant.h"

FX_NS_USE(com);

FX_NS_DEF(plugin);

SETUP_STREAM_LOGGER(plugin, ComFieldTag);

ComFieldTag::ComFieldTag(FieldTag* pTag) 
    : m_pTag(pTag)
{
}

ComFieldTag::~ComFieldTag() 
{
    m_pTag = NULL;
}

IFieldTag* ComFieldTag::wrap(FieldTag* pTag)
{
    ComFieldTag* pWrap = new ComFieldTag(pTag);
    CLASS_TO_INTERFACE(ComFieldTag, FieldTag, pWrap, pXThis);
    IFieldTag* pCom = NULL;
    pXThis->QueryInterface(IFieldTag::iid, (void**)&pCom);
    return pCom;
}

ComFieldTag::FieldTag* ComFieldTag::unwrap(IFieldTag* pInterface)
{
    if ( pInterface == NULL ) return NULL;
    INTERFACE_TO_CLASS(ComFieldTag, FieldTag, pInterface, pWrapper)
    return (FieldTag*)pWrapper->m_pTag;
}

IFieldTag* ComFieldTag::getInterface()
{
    CLASS_TO_INTERFACE(ComFieldTag, FieldTag, this, pXThis);
    IFieldTag* pCom = NULL;
    pXThis->QueryInterface(IFieldTag::iid, (void**)&pCom);
    return pCom;
}

//////////////////////////////////////////////////////////////////////////
//ComFieldTag IFieldTag
IMPLEMENT_UNKNOWN(ComFieldTag, FieldTag)

FX_STDMETHODIMP ComFieldTag::XFieldTag::getId(/* [out] */ FX_NS(com)::FX_INT32* pTagId)
{
    FX_METHOD_PROLOGUE(ComFieldTag, FieldTag);
    *pTagId = (FX_INT32)pThis->m_pTag->getId();

    return FX_S_OK;
}

FX_STDMETHODIMP ComFieldTag::XFieldTag::getTagName(/* [out] */ FX_NS(com)::FX_VARIANT** ppTagName)
{
    FX_METHOD_PROLOGUE(ComFieldTag, FieldTag);
    const std::string& sTagName = pThis->m_pTag->getTagName();
    Variant var(sTagName);
    *ppTagName = var.detach();
    return FX_S_OK;
}

FX_STDMETHODIMP ComFieldTag::XFieldTag::getAlias(/* [out] */ FX_NS(com)::FX_VARIANT** ppTagAlias)
{
    FX_METHOD_PROLOGUE(ComFieldTag, FieldTag);
    const std::string& sTagAlias = pThis->m_pTag->getAlias();
    Variant var(sTagAlias);
    *ppTagAlias = var.detach();
    return FX_S_OK;
}

FX_STDMETHODIMP ComFieldTag::XFieldTag::getFieldName(/* [out] */ FX_NS(com)::FX_VARIANT** ppFieldName)
{
    FX_METHOD_PROLOGUE(ComFieldTag, FieldTag);
    const std::string& sFieldName = pThis->m_pTag->getFieldName();
    Variant var(sFieldName);
    *ppFieldName = var.detach();
    return FX_S_OK;
}

FX_NS_END

