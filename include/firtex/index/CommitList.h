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
// Created : 2011-07-10 23:11:42

#ifndef __FX_COMMITLIST_H
#define __FX_COMMITLIST_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/FileSystem.h"
#include <set>

FX_NS_DEF(index);

class CommitList
{
public:
    typedef std::set<commitid_t> CommitSet;

    class Iterator
    {
    public:
        Iterator(const CommitSet& commits)
            : m_commits(commits)
            , m_iterator(m_commits.begin())
        {
        }

        Iterator(const Iterator& _iter)
            : m_commits(_iter.m_commits)
            , m_iterator(m_commits.begin())
        {
        }

        ~Iterator() 
        {
        }
    public:
        void reset() {m_iterator = m_commits.begin();}
        
        bool hasNext() const 
        {
            return (m_iterator != m_commits.end());
        }
        
        commitid_t next()
        {
            return *m_iterator++;
        }

        size_t size() const
        {
            return m_commits.size();
        }

    private:
        const CommitSet& m_commits;
        CommitSet::const_iterator m_iterator;
    };

public:
    CommitList();
    ~CommitList();

public:
    /**
     * Load commit list from file system
     * 
     * @param pFileSys file system instance
     * @throw throw IndexCollapseException if FAILED.
     */
    void load(FX_NS(store)::FileSystemPtr& pFileSys);

    /// Return commit id iterator
    Iterator iterator() const;

    /// Return number of commits
    size_t size() const;

    /// Return the last commit id 
    commitid_t getLastCommit() const;
    
private:
    CommitSet m_commits;

private:
    DECLARE_STREAM_LOGGER();
};

////////////////////////////////////////
///

inline CommitList::Iterator CommitList::iterator() const
{
    return Iterator(m_commits);
}

inline size_t CommitList::size() const
{
    return m_commits.size();
}

inline commitid_t CommitList::getLastCommit() const
{
    if (m_commits.size() > 0)
    {
        return *(m_commits.rbegin());
    }
    return INVALID_COMMIT;
}

FX_NS_END

#endif //__FX_COMMITLIST_H
