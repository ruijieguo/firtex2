//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2011-07-09 10:41:51

#ifndef __FX_VARIANT_H
#define __FX_VARIANT_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include <string>

FX_NS_DEF(com);

class Variant
{
public:
    Variant(const std::string& str)
        : m_pVar(new FX_VARIANT)
    {
        m_pVar->sVal.length = (FX_INT32)str.length();
        m_pVar->sVal.str = new char[str.length() + 1];
        strcpy(m_pVar->sVal.str, str.c_str());
        m_pVar->vt = FX_VT_STRING;
    }

    Variant(const char* str)
        : m_pVar(new FX_VARIANT)
    {
        m_pVar->sVal.length = (FX_INT32)strlen(str);
        m_pVar->sVal.str = new char[m_pVar->sVal.length + 1];
        strcpy(m_pVar->sVal.str, str);
        m_pVar->vt = FX_VT_STRING;
    }
    
    Variant(FX_VARIANT* pVar)
        : m_pVar(pVar)
    {
    }

    Variant(const Variant& src)
        : m_pVar(NULL)
    {
        if (src.m_pVar)
        {
            assign(*(src.m_pVar));
        }
    }

    ~Variant()
    {
        destroy();
    }

public:
    void copyTo(std::string& str)
    {
        if (m_pVar->vt == FX_VT_STRING)
        {
            str.assign(m_pVar->sVal.str, m_pVar->sVal.length);
        }
        else if (m_pVar->vt == FX_VT_WSTRING)
        {
            size_t len1 = wideCharToMultiByte(NULL, 0,
                    m_pVar->wsVal.str, m_pVar->wsVal.length);
            str.resize(len1);
            size_t len2 = wideCharToMultiByte(const_cast<char*>(str.c_str()),
                    len1, m_pVar->wsVal.str, m_pVar->wsVal.length);
            assert(len1 == len2);
            (void)len2;//avoid warning in release mode
        }
    }

    void attach(FX_VARIANT* pVar)
    {
        destroy();
        m_pVar = pVar;
    }
    
    FX_VARIANT* detach()
    {
        FX_VARIANT* pVar = m_pVar;
        m_pVar = NULL;
        return pVar;
    }

    void assign(const FX_VARIANT& var)
    {
        destroy();

        m_pVar = new FX_VARIANT;
        m_pVar->vt = var.vt;
        switch (var.vt)
        {
        case FX_VT_BOOL:
            m_pVar->bVal = var.bVal;
            break;
        case FX_VT_INT32:
            m_pVar->i32Val = var.i32Val;
            break;
        case FX_VT_FLOAT:
            m_pVar->fVal = var.fVal;
            break;
        case FX_VT_ARRAY:
            m_pVar->arVal.data = new FX_BYTE[var.arVal.length];
            memcpy(m_pVar->arVal.data, var.arVal.data, var.arVal.length);
            m_pVar->arVal.length = var.arVal.length;
            break;
        case FX_VT_STRING:
            m_pVar->sVal.str = new FX_CHAR[var.sVal.length + 1];
            strcpy(m_pVar->sVal.str, var.sVal.str);
            m_pVar->sVal.length = var.sVal.length;
            break;
        case FX_VT_WSTRING:
            m_pVar->wsVal.str = new FX_WCHAR[var.wsVal.length + 1];
            memcpy(m_pVar->wsVal.str, var.wsVal.str, var.wsVal.length * sizeof(FX_WCHAR));
            m_pVar->wsVal.str[var.wsVal.length] = 0;
            m_pVar->wsVal.length = var.wsVal.length;
            break;
        default:
            break;
        }
    }

    void destroy()
    {
        if (m_pVar)
        {
            clear();
            delete m_pVar;
            m_pVar = NULL;
        }
    }

    void clear()
    {
        if (!m_pVar)
        {
            return ;
        }

        switch(m_pVar->vt)
        {
        case FX_VT_STRING:
            delete[] m_pVar->sVal.str;
            m_pVar->sVal.str = NULL;
            m_pVar->sVal.length = 0;
            break;
        case FX_VT_WSTRING:
            delete[] m_pVar->wsVal.str;
            m_pVar->wsVal.str = NULL;
            m_pVar->wsVal.length = 0;
            break;
        case FX_VT_ARRAY:
            delete[] m_pVar->arVal.data;
            m_pVar->arVal.data = NULL;
            m_pVar->arVal.length = 0;
            break;
        default:
            break;
        }
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
    FX_VARIANT* m_pVar;
};

FX_NS_END

#endif //__FX_VARIANT_H
