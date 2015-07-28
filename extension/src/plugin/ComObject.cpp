//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2006/5/15
//
#include "firtex/extension/plugin/ComObject.h"
#include "firtex/utility/StringUtils.h"
#include <assert.h>
#include <cstdio>

using namespace std;

FX_NS_USE(utility);

FX_NS_DEF(plugin);

ComObject::ComObject(void)
{
}

ComObject::~ComObject(void)
{
}

FX_STDMETHODIMP_(uint32_t) ComObject::AddRef()
{ 
//	if ( !m_refCount ) 
//		FX_NS(com)::DllLockServerImpl(true); 
    return (uint32_t)++m_refCount; 
};

FX_STDMETHODIMP_(uint32_t) ComObject::Release()
{ 
    --m_refCount; 
    if ( !m_refCount) 
    {
        delete this;
    }
    return (uint32_t)std::atomic_load(&m_refCount);
};	

/////////////////////////////////////////////////////////////////////////////
// FX_GUID tools

tstring GUIDX::encode(const void * pInGUID)
{
    const FX_NS(com)::FX_GUID* pGUID = reinterpret_cast<const FX_NS(com)::FX_GUID*>(pInGUID);
    tchar tmp[512];
    sprintf(tmp,_T("{%.8X-%.4X-%.4X-%.2X%.2X-%.2X%.2X%.2X%.2X%.2X%.2X}"),
            pGUID->Data1, pGUID->Data2, pGUID->Data3,
            pGUID->Data4[0], pGUID->Data4[1], pGUID->Data4[2], pGUID->Data4[3],
            pGUID->Data4[4], pGUID->Data4[5], pGUID->Data4[6], pGUID->Data4[7] );

    tstring str = tmp;

    return str;
}

bool GUIDX::decode(const tchar* pszIn, void* pOutGUID)
{
    assert( pOutGUID != NULL );

    if ( pszIn == NULL ) return false;

    if (strLength(pszIn) == 38 )
    {
        if ( pszIn[0] != '{' || pszIn[37] != '}' ) return false;
    }
    else if ( strLength(pszIn) == 36 )
    {
        pszIn --;
    }
    else return false;

    byte* pGUID = reinterpret_cast<byte*>(pOutGUID);

    if ( ! unhex( pszIn + 1, pGUID + 3 ) ) return false;
    if ( ! unhex( pszIn + 3, pGUID + 2 ) ) return false;
    if ( ! unhex( pszIn + 5, pGUID + 1 ) ) return false;
    if ( ! unhex( pszIn + 7, pGUID + 0 ) ) return false;
    if ( ! unhex( pszIn + 10, pGUID + 5 ) ) return false;
    if ( ! unhex( pszIn + 12, pGUID + 4 ) ) return false;
    if ( ! unhex( pszIn + 15, pGUID + 7 ) ) return false;
    if ( ! unhex( pszIn + 17, pGUID + 6 ) ) return false;
    if ( ! unhex( pszIn + 20, pGUID + 8 ) ) return false;
    if ( ! unhex( pszIn + 22, pGUID + 9 ) ) return false;
    if ( ! unhex( pszIn + 25, pGUID + 10 ) ) return false;
    if ( ! unhex( pszIn + 27, pGUID + 11 ) ) return false;
    if ( ! unhex( pszIn + 29, pGUID + 12 ) ) return false;
    if ( ! unhex( pszIn + 31, pGUID + 13 ) ) return false;
    if ( ! unhex( pszIn + 33, pGUID + 14 ) ) return false;
    if ( ! unhex( pszIn + 35, pGUID + 15 ) ) return false;

    return true;
}

bool GUIDX::unhex(const tchar* psz, byte* pOut)
{
    tchar c = *psz++;
    if ( c >= '0' && c <= '9' )
        *pOut = ( c - '0' ) << 4;
    else if ( c >= 'A' && c <= 'F' )
        *pOut = ( c - 'A' + 10 ) << 4;
    else if ( c >= 'a' && c <= 'f' )
        *pOut = ( c - 'a' + 10 ) << 4;
    else
        return false;
    c = *psz;
    if ( c >= '0' && c <= '9' )
        *pOut |= ( c - '0' );
    else if ( c >= 'A' && c <= 'F' )
        *pOut |= ( c - 'A' + 10 );
    else if ( c >= 'a' && c <= 'f' )
        *pOut |= ( c - 'a' + 10 );
    else
        return false;
    return true;
}

FX_NS_END
