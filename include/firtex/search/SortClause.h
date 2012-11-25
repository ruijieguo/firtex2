//
// Copyright(C) 2005--2010 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2011-01-08 13:21:42

#ifndef __FX_SORTCLAUSE_H
#define __FX_SORTCLAUSE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/Clause.h"

FX_NS_DEF(search);

class SortClause : public Clause
{
public:
    DECLARE_CLAUSE_CREATOR(SortClause);

public:
    const static std::string SORTER_SEPARATOR;
    const static std::string PREFIX;

public:
    typedef std::vector<std::string> SorterVector;

    class Iterator
    {
    public:
        Iterator(const SorterVector& sorters)
            : m_sorters(sorters)
            , m_iter(m_sorters.begin())
        {
        }

    public:
        bool hasNext() const
        {
            return m_iter != m_sorters.end();
        }

        const std::string& next()
        {
            return *m_iter++;
        }

        size_t size() const 
        {
            return m_sorters.size();
        }
    private:
        const SorterVector& m_sorters;
        SorterVector::const_iterator m_iter;
    };

public:
    SortClause(const std::string& sName,
               const std::string& sValue);
    ~SortClause();

public:
    /**
     * Return iterator of sorter strings
     */
    Iterator iterator() const;

private:
    void fromString(const std::string& sValue);

private:
    SorterVector m_sorters;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(SortClause);

/////////////////////////////////////////////////////
//
inline SortClause::Iterator SortClause::iterator() const
{
    return Iterator(m_sorters);
}

FX_NS_END

#endif //__FX_SORTCLAUSE_H
