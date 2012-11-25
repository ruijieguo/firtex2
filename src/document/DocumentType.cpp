//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn,ruijieguo@gmail.com
// Created	: 2008/12/2
//

#include "firtex/document/DocumentType.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/common/Logger.h"

using namespace std;
FX_NS_USE(utility);

FX_NS_DEF(document);

SETUP_LOGGER(document, DocumentType);

DocumentType::DocumentType()
{					
    registerType(_T("unknow"),_T(""));
}

DocumentType::~DocumentType()
{
    doctype_map::iterator iter = m_nameMap.begin();
    while (iter != m_nameMap.end())
    {
        delete iter->second;
        iter++;
    }
    m_nameMap.clear();
    m_identifierMap.clear();
    m_idMap.clear();
}

void DocumentType::registerType(const tstring& sName,const tstring& sIdentifier)
{
    SCOPED_LOCK();

    StringTokenizer st;
    st.tokenize(sName, _T(";"), StringTokenizer::TOKEN_TRIM
                | StringTokenizer::TOKEN_IGNORE_EMPTY);
    doctype_t dt = (doctype_t)m_nameMap.size();
    for (StringTokenizer::Iterator iter = st.begin(); iter != st.end(); iter++)
    {
        DOC_TYPE* pType = new DOC_TYPE();
        pType->id = dt;
        pType->name = *iter;
        pType->identifier = sIdentifier;
        pType->name = FX_NS(utility)::toLower(pType->name);
        doctype_map::iterator it = m_nameMap.find(pType->name);
        if (it != m_nameMap.end())
        {
            FX_DEBUG("Register type: %s two times.", pType->name.c_str());
            delete it->second;
            m_nameMap.erase(it);
            m_nameMap.insert(make_pair(pType->name, pType));
        }
        else 
        {
            FX_DEBUG("Register type: %s", pType->name.c_str());
            m_nameMap.insert(make_pair(pType->name, pType));
            m_identifierMap.insert(make_pair(pType->identifier, pType));
            m_idMap.insert(make_pair(pType->id, pType));
        }
    }
}

tstring DocumentType::typeToName(doctype_t _doctype) const
{
    typeid_map::const_iterator iter = m_idMap.find(_doctype);
    if(iter != m_idMap.end())
    {
        return iter->second->name;
    }
    return _T("");
}	

doctype_t DocumentType::nameToType(const tstring& sName) const
{
    tstring str = FX_NS(utility)::toLower(sName);
    doctype_map::const_iterator iter = m_nameMap.find(str);
    if(iter != m_nameMap.end())
    {
        return iter->second->id;
    }
    return doc_type::UNKNOWN;
}

tstring DocumentType::typeToIdentifier(doctype_t _doctype)
{
    SCOPED_LOCK();
    typeid_map::iterator iter = m_idMap.find(_doctype);
    if(iter != m_idMap.end())
    {
        return iter->second->identifier;
    }
    return _T("");
}

tstring DocumentType::nameToIdentifier(const tstring& sName)
{
    SCOPED_LOCK();
    tstring str = FX_NS(utility)::toLower(sName);
    doctype_map::iterator iter = m_nameMap.find(str);
    if(iter != m_nameMap.end())
    {
        return iter->second->identifier;
    }
    return _T("");
}

size_t DocumentType::numTypes()
{
    SCOPED_LOCK();
    return m_nameMap.size();
}

FX_NS_END
