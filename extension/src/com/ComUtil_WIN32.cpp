#include "firtex/extension/com/Com.h"
#include <assert.h>

#ifndef _WIN32_WINNT		// 允许使用 Windows NT 4 或更高版本的特定功能。
#define _WIN32_WINNT 0x0400		//为 Windows98 和 Windows 2000 及更新版本改变为适当的值。
#endif	

#include <objbase.h>
#include <wchar.h>

FX_NS_DEF(com);

FX_WINOLEAPI FX_CLSIDFromString(wchar_t const* idstr, FX_CLSID* id)
{
	return ::CLSIDFromString((LPOLESTR)idstr,(CLSID*)id);
}

FX_WINOLEAPI_(int) FX_StringFromGUID2(FX_REFGUID rguid, wchar_t* lpsz, int cbMax)
{
	return ::StringFromGUID2((REFIID)rguid,lpsz,cbMax);
}

FX_WINOLEAUTAPI_(FX_BSTR) FX_SysAllocString(const wchar_t * str)
{
 	return ::SysAllocString(str);
}

FX_WINOLEAUTAPI_(FX_BSTR) FX_SysAllocStringLen(const wchar_t *str, uint16_t len)
{
 	return ::SysAllocStringLen(str, len);
}

FX_WINOLEAUTAPI_(void) FX_SysFreeString(FX_BSTR str)
{
 	return ::SysFreeString((::BSTR)str);
}
FX_WINOLEAUTAPI_(uint16_t) FX_SysStringLen(FX_BSTR str)
{
	return ::SysStringLen((::BSTR)str);
}

FX_WINOLEAUTAPI_(FX_BSTR) FX_SysAllocStringA( const char* pSrc )
{
	// check for null argument
	// if NULL, defer to wide character version
	if ( !pSrc ) 
    {
        return FX_SysAllocString( 0 );
    }

	// get length of the wchar_t string
	mbstate_t state;
	memset( &state, 0, sizeof(state) );
	char const* tmp = pSrc;
	size_t wl = mbsrtowcs( NULL, &tmp, 0, &state );

	// create the buffer
	FX_BSTR bstr = FX_SysAllocStringLen( 0, (uint16_t)wl );
	if ( !bstr ) return 0;

	// copy the buffer
	memset( &state, 0, sizeof(state) );
	tmp = pSrc;
	size_t wl2 = mbsrtowcs( const_cast<wchar_t*>(bstr), &tmp, wl, &state );
	assert( wl == wl2 );
	return bstr;
}

FX_NS_END

