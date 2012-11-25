//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author       : Ruijie Guo
// Email        : ruijieguo@gmail.com
// Created      : 2006/5/8
//
#ifndef _FIRTEXCOM_H
#define _FIRTEXCOM_H

#include "../com/Com.h"

#ifdef __cplusplus

FX_NS_DEF(plugin);

// {6D0B428B-3779-4d74-85AE-3F4DB4E29AE5}
class IFieldTag : public FX_NS(com)::IUnknown
{
public:
    static const FX_NS(com)::FX_IID iid;

public:
    virtual ~IFieldTag() {}
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE getId(
            /* [out] */ FX_NS(com)::FX_INT32* pTagId) = 0;
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE getTagName(
            /* [out] */ FX_NS(com)::FX_VARIANT** ppTagName) = 0;
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE getAlias(
            /* [out] */ FX_NS(com)::FX_VARIANT** ppTagAlias) = 0;
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE getFieldName(
            /* [out] */ FX_NS(com)::FX_VARIANT** ppFieldName) = 0;
};

// {F3EFAAE3-BD25-4f2a-835D-D36367EF44AD}
class IRawDocument : public FX_NS(com)::IUnknown
{
public:
    virtual ~IRawDocument() {}

public:
    static const FX_NS(com)::FX_IID iid;

public:
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE isPath(
            /* [out] */ FX_NS(com)::FX_BOOL* pPath) = 0;
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE isContent(
            /* [out] */ FX_NS(com)::FX_BOOL* pContent) = 0;
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE getPath(
            /* [out] */ FX_NS(com)::FX_VARIANT** ppPath) = 0;
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE setPath(
            /* [in] */ FX_NS(com)::FX_VARIANT* pPath) = 0;
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE getContent(
            /* [out] */ FX_NS(com)::FX_VARIANT** pContent) = 0;
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE setContent(
            /* [in] */ FX_NS(com)::FX_VARIANT* pContent) = 0;
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE clear() = 0;
};

// {C92FA9F4-86FD-48c1-AB00-B16C78D2983F}
class IDocument : public FX_NS(com)::IUnknown
{
public:
    virtual ~IDocument() {} //avoid warnings

public:
    static const FX_NS(com)::FX_IID iid;

    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE setBoost(
            /* [in] */ FX_NS(com)::FX_FLOAT boost) = 0;

    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE addField(
            /* [in] */ FX_NS(com)::FX_INT32 fieldId,
            /* [in] */ FX_NS(com)::FX_VARIANT* pVar,
            /* [in] */ FX_NS(com)::FX_BOOL bNoCopy) = 0;
};

// {C0CCA7C8-79FE-40d6-AED8-1FD8FABD6F6A}
class IDocumentSource : public FX_NS(com)::IUnknown
{
public:
    virtual ~IDocumentSource() {}

public:
    static const FX_NS(com)::FX_IID iid;

public:
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE setRawDocument(
            /* [in] */ IRawDocument* pRawDoc) = 0;
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE getRawDocument(
            /* [out] */ IRawDocument** ppRawDoc) = 0;
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE acquireDocument(
            /* [in] */ FX_NS(com)::FX_BSTR processorName,
            /* [out] */ IDocument** ppDocument) = 0;
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE stealLastDocument(
            /* [out] */ IDocument** ppDocument) = 0;
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE toBeContinued(
            /* [out] */ FX_NS(com)::FX_BOOL* bContinue) = 0;
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE setToBeContinued(
            /* [in] */ FX_NS(com)::FX_BOOL bContinue) = 0;
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE reset() = 0;
};

// {2E15293F-ED12-44f0-B953-35DA4D51E0EC}
class IDocumentTemplate : public FX_NS(com)::IUnknown
{
public:
    virtual ~IDocumentTemplate() {}

public:
    static const FX_NS(com)::FX_IID iid;

public:
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE getDocTag(
            /* [out] */ FX_NS(com)::FX_VARIANT** ppDocTag) = 0;
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE getTagCount(
            /* [out] */ FX_NS(com)::FX_INT32* pTagCount) = 0;
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE getEmbeddedField(
            /* [out] */ FX_NS(com)::FX_VARIANT** ppField) = 0;
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE getTag(
            /* [in] */ FX_NS(com)::FX_INT32 tagIdx, 
            /* [out] */ IFieldTag** ppTag) = 0;
};

//{09B42270-8ACD-4e8a-B125-A79B46003F5D}
class IDocumentSchema : public FX_NS(com)::IUnknown
{
public:
    virtual ~IDocumentSchema() {} // avoid warnings

public:
    static const FX_NS(com)::FX_IID iid;

    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE addField(
            /* [in] */ FX_NS(com)::FX_BSTR name, 
            /* [in] */ FX_NS(com)::FX_BSTR type, 
            /* [in] */ FX_NS(com)::FX_BOOL isStore,
            /* [in] */ FX_NS(com)::FX_FLOAT boost,
            /* [out] */ FX_NS(com)::FX_INT32* pFieldId) = 0;

    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE getFieldId(
            /* [in] */ FX_NS(com)::FX_BSTR name,
            /* [out] */ FX_NS(com)::FX_INT32* pFieldId) = 0;
};

// {3EF8289A-9514-4c24-B6C2-4A6A595C9333}
class IDocumentProcessorPlugin : public FX_NS(com)::IUnknown
{
public:
    virtual ~IDocumentProcessorPlugin() {}

public:
    static const FX_NS(com)::FX_IID iid;

public:
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE init(
            /* [in] */ IDocumentSchema* pDocSchema,
            /* [in] */ IDocumentTemplate* pDocTemp) = 0;
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE getIdentifier(
            /* [out] */ FX_NS(com)::FX_BSTR* pIdent) = 0;
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE getDocType(
            /* [out] */ FX_NS(com)::FX_BSTR* pDocType) = 0;
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE process(
            /* [in] */ IDocumentSource* pDocSource) = 0;
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE clone(
            /* [out] */IDocumentProcessorPlugin** ppPlugin) = 0;
};

enum TOKENENUM
{
    FX_TT_UNKNOWN = 0,
    FX_TT_NUMBER,      //< number
    FX_TT_ALPHA,
    FX_TT_CWORD,       //< Chinese word
    FX_TT_STRING,      //< String
    FX_TT_PUNCT,       //< Punctuation symbol
    FX_TT_WHITESPACE,  //< whitespace
    FX_TT_LINE_BREAK,   //< line break (\n \r)
    FX_TT_MAX,         //< Number of attribute
};

typedef uint32_t TOKENTYPE;

// {E6F0F10C-DF0B-411a-9A3E-EA2BCD4DF256}
class ITokenView : public FX_NS(com)::IUnknown
{
public:
    virtual ~ITokenView() {}

public:
    static const FX_NS(com)::FX_IID iid;

    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE addToken(
            /* [in] */ FX_NS(com)::FX_VARIANT* pTokenVar, 
            /* [in] */ FX_NS(com)::FX_INT32 nPosInc,
            /* [in] */ FX_NS(com)::FX_INT32 nStartOffset,
            /* [in] */ FX_NS(com)::FX_INT32 nEndOffset,
            /* [in] */ TOKENTYPE tokenType) = 0;

    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE getNumTokens(
            /* [out] [retval] */ FX_NS(com)::FX_INT32* pNumTokens) = 0;

    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE clear() = 0;
};

// {8AD182C7-E4BD-41b5-B73E-E78FEB575A4C}
class ITokenSource : public FX_NS(com)::IUnknown
{
public:
    virtual ~ITokenSource() {}

public:
    static const FX_NS(com)::FX_IID iid;

    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE acquireCustomView(
            /* [in] */ FX_NS(com)::FX_BSTR viewName, 
            /* [out][retval */ ITokenView** ppTokenView) = 0;

    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE getOriginalView(
            /* [out][retval */ ITokenView** ppTokenView) = 0;

    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE getLastView(
            /* [out][retval */ ITokenView** ppTokenView) = 0;

    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE reset() = 0;
};

// {7D7D876B-CCE4-4fe0-8389-A20106A828A3}
class IAnalyzerPlugin : public FX_NS(com)::IUnknown
{
public:
    virtual ~IAnalyzerPlugin() {}

public:
    static const FX_NS(com)::FX_IID iid;

    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE getIdentifier(
            /* [out] [retval] */FX_NS(com)::FX_BSTR* retVal) = 0;

    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE setMaxTokensToAnalyze(
            /* [in] */ FX_NS(com)::FX_INT32 nMaxTokens) = 0;

    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE getMaxTokensToAnalyze(
            /* [in] */ FX_NS(com)::FX_INT32* pMaxTokens) = 0;
    
    virtual FX_NS(com)::FX_HRESULT FX_STDMETHODCALLTYPE tokenize(
            /* [in] */ ITokenSource* pTokenSource) = 0;
};

FX_NS_END

#endif //__cplusplus

#endif
