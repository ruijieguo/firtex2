#ifndef _COMPTR_H_
#define _COMPTR_H_

#include "Com.h"
#include "ComUtil.h"
#include <assert.h>
#include <stdexcept>

FX_NS_DEF(com)
	

/////////////////////////////////////////////////////////////////////////////
// ComPtr

template <class T>
class ComPtr
{
public:
    ComPtr()
    {
        ptr = 0; 
    };

    ComPtr( T* rhs )
    {
        ptr = rhs; if (ptr) ptr->AddRef(); 
    };
    ComPtr( ComPtr<T> const& rhs )
    {
        ptr = rhs.ptr; if (ptr) ptr->AddRef(); 
    };
    ~ComPtr()
    {
        if ( ptr )
            ptr->Release(); 
    };

    operator T*() const 
    {
        return ptr; 
    };
    T** operator&() 
    {
        return &ptr; 
    };
    T* operator->()
    {
        return ptr; 
    };
    ComPtr<T>&	operator=( T* rhs )
    {
        if ( ptr ) 
            ptr->Release();
        ptr=rhs; 
        if (ptr)
            ptr->AddRef();
        return *this; 
    };
    ComPtr<T>&	operator=( ComPtr<T> const& rhs )
    { 
        if ( ptr )
            ptr->Release(); 
        ptr=rhs.ptr;
        if (ptr) 
            ptr->AddRef();
        return *this; 
    };

    void attach( T* rhs )
    {
        if ( ptr ) 
            ptr->Relese();
        ptr = rhs; 
    }; 
    T*	detach() 
    { 
        T* ret = ptr; 
        ptr = 0;
        return ret; 
    };
    T*	get()
    {
        return ptr; 
    };

private:
    T*	ptr;
};

/////////////////////////////////////////////////////////////////////////////
// ComStr

class ComStr
{
public:
    ComStr()
    {
        m_str = NULL;
    }
    /*explicit*/ ComStr( int nSize )
    {
        m_str = FX_SysAllocStringLen(NULL, nSize);
    }
    /*explicit*/ ComStr(int nSize, wchar_t const* sz)
    {
        m_str = FX_SysAllocStringLen(sz, nSize);
    }
    /*explicit*/ ComStr(wchar_t const* pSrc)
    {
        m_str = FX_SysAllocString(pSrc);
    }
    /*explicit*/ ComStr(const ComStr& src)
    {
        m_str = src.copy();
    }
    /*explicit*/ ComStr( FX_REFGUID src )
    {
        wchar_t guid[64];
        FX_StringFromGUID2( src, guid, sizeof(guid)/sizeof(*guid) );
        m_str = FX_SysAllocString(guid);
    }

    /*explicit*/ ComStr(FX_VARIANT var)
    {
        if (var.vt == FX_VT_WSTRING)
        {
            FX_BSTR tmp = FX_SysAllocString(var.wsVal.str);
            if ( !tmp ) throw std::bad_alloc();
            m_str = tmp;
        }
        else if (var.vt == FX_VT_STRING)
        {
            FX_BSTR tmp = FX_SysAllocStringA(var.sVal.str);
            if ( !tmp ) throw std::bad_alloc();
            m_str = tmp;
        }
        
        m_str = NULL;
    }

    ComStr& operator=(const ComStr& src)
    {
        if (m_str != src.m_str) {
            FX_BSTR tmp = src.copy();
            if ( !tmp ) throw std::bad_alloc();
            FX_SysFreeString(m_str);
            m_str = tmp;
        }
        return *this;
    }
    ComStr& operator=(wchar_t const* pSrc)
    {
        FX_BSTR tmp = FX_SysAllocString(pSrc);
        if ( !tmp ) throw std::bad_alloc();
        FX_SysFreeString(m_str);
        m_str = tmp;
        return *this;
    }
    ~ComStr()
    {
        FX_SysFreeString(m_str);
    }

    uint16_t getLength() const
    {
        return (m_str == NULL)? 0 : FX_SysStringLen(m_str);
    }
    operator FX_BSTR() const
    {
        return m_str;
    }
    FX_BSTR* operator&()
    {
        return &m_str;
    }
    FX_BSTR copy() const
    {
        return FX_SysAllocStringLen(m_str, FX_SysStringLen(m_str));
    }
    FX_HRESULT copyTo(FX_BSTR* pbstr)
    {
        assert(pbstr != NULL);
        if (pbstr == NULL) return FX_E_POINTER;
        *pbstr = FX_SysAllocStringLen(m_str, FX_SysStringLen(m_str));
        if (*pbstr == NULL) return FX_E_OUTOFMEMORY;
        return FX_S_OK;
    }

    void copyTo(std::string& str)
    {
        if (m_str == NULL) return ;

        size_t len1 = wideCharToMultiByte(NULL, 0,
                m_str, getLength());
        str.resize(len1);
        
        size_t len2 = wideCharToMultiByte(const_cast<char*>(str.c_str()), 
                len1, m_str, getLength());
        assert(len1 == len2);
        len1 = len2;//avoid warning in release mode
    }

    void copyTo(std::wstring& str)
    {
        if (m_str == NULL) return ;

        str.resize(getLength());
        str.assign(m_str);
    }

    void attach(FX_BSTR src)
    {
        assert(m_str == NULL);
        m_str = src;
    }
    FX_BSTR detach()
    {
        FX_BSTR s = m_str;
        m_str = NULL;
        return s;
    }
    FX_BSTR get()
    {
        return m_str;
    }
    void empty()
    {
        FX_SysFreeString(m_str);
        m_str = NULL;
    }
    bool operator!() const
    {
        return (m_str == NULL);
    }
    FX_HRESULT append(const ComStr& bstrSrc)
    {
        return append(bstrSrc.m_str, FX_SysStringLen(bstrSrc.m_str));
    }
    FX_HRESULT append(wchar_t const* lpsz)
    {
        return append(lpsz, (int)wcslen(lpsz));
    }
    // a FX_BSTR is just a LPCOLESTR so we need a special version to signify
    // that we are appending a FX_BSTR
    FX_HRESULT appendBSTR(FX_BSTR p)
    {
        return append(p, FX_SysStringLen(p));
    }
    FX_HRESULT append(wchar_t const* lpsz, int nLen)
    {
        int n1 = getLength();
        FX_BSTR b;
        b = FX_SysAllocStringLen(NULL, n1+nLen);
        if (b == NULL)
            return FX_E_OUTOFMEMORY;
        memcpy(const_cast<wchar_t*>(b), m_str, n1*sizeof(wchar_t));
        memcpy(const_cast<wchar_t*>(b+n1), lpsz, nLen*sizeof(wchar_t));
        assert( b[n1+nLen] == L'\0' );
        FX_SysFreeString(m_str);
        m_str = b;
        return FX_S_OK;
    }

    ComStr& operator+=(const ComStr& bstrSrc)
    {
        appendBSTR(bstrSrc.m_str);
        return *this;
    }
    bool operator<(FX_BSTR bstrSrc) const
    {
        if (bstrSrc == NULL && m_str == NULL)
            return false;
        if (bstrSrc != NULL && m_str != NULL)
            return wcscmp(m_str, bstrSrc) < 0;
        return m_str == NULL;
    }
    bool operator==(FX_BSTR bstrSrc) const
    {
        if (bstrSrc == NULL && m_str == NULL)
            return true;
        if (bstrSrc != NULL && m_str != NULL)
            return wcscmp(m_str, bstrSrc) == 0;
        return false;
    }
    bool operator<(char const* pszSrc) const
    {
        if (pszSrc == NULL && m_str == NULL)
            return false;
        ComStr tmp( pszSrc );
        if (tmp.m_str != NULL && m_str != NULL)
            return wcscmp(m_str, tmp.m_str) < 0;
        return m_str == NULL;
    }
    bool operator==(char const* pszSrc) const
    {
        if (pszSrc == NULL && m_str == NULL)
            return true;
        ComStr tmp( pszSrc );
        if ( !tmp ) throw std::bad_alloc();
        if (tmp.m_str != NULL && m_str != NULL)
            return wcscmp(m_str, tmp.m_str) == 0;
        return false;
    }

    /* explicit */ ComStr(char const* pSrc)
    {
        m_str = FX_SysAllocStringA( pSrc );
    }

    FX_HRESULT append( char const* pSrc)
    {
        ComStr tmp( pSrc );
        if ( !tmp ) return FX_E_OUTOFMEMORY;
        return append( tmp );
    }

    ComStr& operator=(char const* pSrc)
    {
        ComStr tmp( pSrc );
        if ( !tmp ) throw std::bad_alloc();
        FX_SysFreeString(m_str);
        m_str = tmp.detach();
        return *this;
    }

private:
    static size_t wideCharToMultiByte(char *mbs, size_t mlen,
            const wchar_t *wcs, size_t wlen)
    {
        if(!wcs || wlen <= 0)
            return 0;

        mbstate_t state;
        memset( &state, 0, sizeof(state) );
        wchar_t const* tmp = wcs;
        return wcsrtombs(mbs, &tmp, mlen, &state);
    }

private:
    FX_BSTR m_str;
}; // class ComStr

FX_NS_END

#endif // _COMPTR_H_
