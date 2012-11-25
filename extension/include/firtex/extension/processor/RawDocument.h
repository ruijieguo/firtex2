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
// Created : 2011-06-19 23:03:32

#ifndef __FX_RAWDOCUMENT_H
#define __FX_RAWDOCUMENT_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(processor);

class RawDocument
{
public:
    enum Form
    {
        FORM_UNKNOWN = 0,
        FORM_PATH,
        FORM_CONTENT,
    };

public:
    RawDocument() : m_eForm(FORM_UNKNOWN) {}
    ~RawDocument() {}

public:
    /// Return true if in PATH form
    bool isPath() const;

    /// Return true if in CONTENT form
    bool isContent() const;

    /// Return path value if the document is in PATH form
    const std::string& getPath() const;

    /// Set path value
    void setPath(const std::string& sPath);

    /// Return path value if the document is in CONTENT form
    const std::string& getContent() const;

    /// Set content value
    void setContent(const char* szContent, size_t length);

    /// Clear resource
    void clear();

private:
    Form m_eForm;
    std::string m_value;
    
private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(RawDocument);

/////////////////////////////////////////
inline bool RawDocument::isPath() const
{
    return (m_eForm == FORM_PATH);
}

inline bool RawDocument::isContent() const
{
    return m_eForm == FORM_CONTENT;
}

inline const std::string& RawDocument::getPath() const
{
    return m_value;
}

inline void RawDocument::setPath(const std::string& sPath)
{
    m_value = sPath;
    m_eForm = FORM_PATH;
}

inline const std::string& RawDocument::getContent() const
{
    return m_value;
}

inline void RawDocument::setContent(const char* szContent, size_t length)
{
    m_value.assign(szContent, length);
    m_eForm = FORM_CONTENT;
}

inline void RawDocument::clear()
{
    m_value.clear();
    m_eForm = FORM_UNKNOWN;
}

FX_NS_END

#endif //__FX_RAWDOCUMENT_H
