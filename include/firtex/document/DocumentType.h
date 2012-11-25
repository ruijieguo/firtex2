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
// Created	: 2005/11/20
//
#ifndef __DOCUMENTTYPE_H
#define __DOCUMENTTYPE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/utility/Singleton.h"
#include "firtex/thread/SynchronizedObject.h"
#include <string>
#include <map>

FX_NS_DEF(document);

typedef int32_t doctype_t;

namespace doc_type
{
//////////////////////////////////////////////////////////////////////////
///document type define			
const static doctype_t UNKNOWN = 0;
}			

class DocumentType : public FX_NS(thread)::SynchronizedObject,
                     public FX_NS(utility)::Singleton<DocumentType>
{
protected:
    struct DOC_TYPE 
    {
        doctype_t	id;
        tstring		name;
        tstring		identifier;
    };	

    typedef std::map<tstring,DocumentType::DOC_TYPE*> doctype_map;
    typedef std::map<doctype_t,DocumentType::DOC_TYPE*> typeid_map;
public:
    DocumentType();
    virtual ~DocumentType();
public:
    /**
     * register document type
     * @param sName document extension name
     * @param sIdentifier identifier of document parser
     */
    void registerType(const tstring& sName,const tstring& sIdentifier);

    /**
     * convert document type to document extension name
     * @param _doctype document type
     * @return document extension name
     */
    tstring typeToName(doctype_t _doctype) const;

    /**
     * convert document extension name to document type
     * @param sName document extension name
     * @return document type
     */
    doctype_t nameToType(const tstring& sName) const;

    /**
     * get identifier of document parser by document type
     * @param _doctype document type
     * @return identifier of document parser
     */
    tstring typeToIdentifier(doctype_t _doctype);

    /**
     * get identifier of document parser by document extension name
     * @param _doctype document type
     * @return identifier of document parser
     */
    tstring nameToIdentifier(const tstring& sName);

    /**
     * get number of document type	 
     * @return number of document type	 
     */
    size_t	numTypes();
protected:
    doctype_map		m_nameMap;
    doctype_map		m_identifierMap;
    typeid_map		m_idMap;

private:
    DECLARE_LOGGER();
};

//////////////////////////////////////////////////////////////////////////
///document status define

typedef int32_t docstatus_t;
namespace doc_status
{
const static docstatus_t UNPARSED	= 1;		/// unparsed
const static docstatus_t PARSED		= 2;		/// parsed but not analyzed
const static docstatus_t ANALYZED	= 3;		/// analyzed but not indexed
const static docstatus_t INDEXED	= 4;		/// indexed			
}		

FX_NS_END

#endif
