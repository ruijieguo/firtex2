#ifndef __FX_COMUTIL_H
#define __FX_COMUTIL_H

FX_NS_DEF(com);

FX_WINOLEAPI FX_CLSIDFromString(wchar_t const* idstr, FX_CLSID* id);
FX_WINOLEAPI_(int) FX_StringFromGUID2(FX_REFGUID rguid, wchar_t* lpsz, int cbMax);

FX_WINOLEAUTAPI_(FX_BSTR) FX_SysAllocString(const wchar_t *);
FX_WINOLEAUTAPI_(FX_BSTR) FX_SysAllocStringLen(const wchar_t *, uint16_t);
FX_WINOLEAUTAPI_(void) FX_SysFreeString(FX_BSTR);
FX_WINOLEAUTAPI_(uint16_t) FX_SysStringLen(FX_BSTR str);

FX_WINOLEAUTAPI_(FX_BSTR) FX_SysAllocStringA(char const*);

FX_NS_END

#endif
