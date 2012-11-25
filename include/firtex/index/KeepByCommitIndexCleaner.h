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
// Created : 2011-07-10 23:06:52

#ifndef __FX_KEEPBYCOMMITINDEXCLEANER_H
#define __FX_KEEPBYCOMMITINDEXCLEANER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/IndexCleaner.h"
#include "firtex/store/FileSystem.h"
#include "firtex/index/CommitList.h"

FX_NS_DEF(index);

class KeepByCommitIndexCleaner : public IndexCleaner
{
public:
    typedef CommitList::CommitSet CommitSet;

    DECLARE_INDEXCLEANER_CREATOR(KeepByCommitIndexCleaner, "keep_by_commit");

public:
    KeepByCommitIndexCleaner();
    ~KeepByCommitIndexCleaner();

public:
    /**
     * Initialize parameter 
     *
     * @param pFileSys file system instance
     * @param sParam parameter for cleaner
     */
    virtual void init(const FX_NS(store)::FileSystemPtr& pFileSys,
                      const std::string& sParam);

    /**
     * Clean up stale index data by commit.
     */
    virtual void cleanup();

private:
    // Extract commit ids from barrels info
    void extractCommits(commitid_t comId, CommitSet& commits);

private:
    const static size_t DEFAULT_FILE_BUFFER = 32 * 1024;

    /// File system instance
    FX_NS(store)::FileSystemPtr m_pFileSys;

    /// Number of commits to keep
    uint32_t m_uKeepCount;

    /// For commit files
    std::string m_sFileBuffer;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(KeepByCommitIndexCleaner);

FX_NS_END

#endif //__FX_KEEPBYCOMMITINDEXCLEANER_H
