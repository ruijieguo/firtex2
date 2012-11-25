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
// Created : 2011-10-16 23:59:25

#ifndef __FX_FIELDSELECTCLAUSE_H
#define __FX_FIELDSELECTCLAUSE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/Clause.h"
#include <vector>
#include <map>

FX_NS_DEF(search);

class FieldSelectClause : public Clause
{
public:
    DECLARE_CLAUSE_CREATOR(FieldSelectClause);

    const static std::string PREFIX;

public:
    FieldSelectClause(const std::string& sName,
                      const std::string& sValue);
    ~FieldSelectClause();

public:
    struct SnippetParam
    {
        SnippetParam(const std::string& sField,
                     bool s = false) 
            : snippet(s)
            , field(sField)
        {
        }

        bool snippet;
        std::string field;
        std::string preTag;
        std::string postTag;
        std::string separator;
    };

    typedef std::vector<SnippetParam> FieldVector;

public:
    /// Return number of fields
    size_t getFieldCount() const;

    /**
     * Return field by index
     * 
     * @param idx the index of array
     * @param bSnippet set true if snippet field 
     * @return field name
     */
    const SnippetParam& getField(size_t idx) const;

protected:
    void fromString(const std::string& sClauseValue);
    
private:
    FieldVector m_fields;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(FieldSelectClause);

/////////////////////////////////
//
inline size_t FieldSelectClause::getFieldCount() const
{
    return m_fields.size();
}

inline const FieldSelectClause::SnippetParam&
FieldSelectClause::getField(size_t idx) const
{
    FIRTEX_ASSERT2(idx < m_fields.size());
    return m_fields[idx];
}

FX_NS_END

#endif //__FX_FIELDSELECTCLAUSE_H
