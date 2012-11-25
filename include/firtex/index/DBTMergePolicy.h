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
// Created : 2011-06-26 12:21:48

#ifndef __FX_DBTMERGEPOLICY_H
#define __FX_DBTMERGEPOLICY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/MergePolicy.h"

FX_NS_DEF(index);

class DBTMergePolicy : public MergePolicy
{
public:
    /// Default value of collision factor
    const static size_t DEFAULT_COLLISION_FACTOR = 3; 

    /// Default value of scale factor
    const static size_t DEFAULT_SCALE_FACTOR = 1;

    /// Max level of dynamic balance tree
    const static size_t MAX_TREE_LEVEL = 10;

public:
    DECLARE_MERGEPOLICY_CREATOR(DBTMergePolicy, "dbt");

public:
    DBTMergePolicy();
    ~DBTMergePolicy();

public:
    /**
     * Create merge proposal
     * @param barrelsInfo the barrels need to merge
     * @return merge proposals
     */
    MergeProposalPtr createProposals(const BarrelsInfo& barrelsInfo);

private:
    struct Node;
    typedef std::vector<Node> Nodes;

    struct Node
    {
        Node(const BarrelInfo& bi)
            : commitId(bi.getCommitId())
            , docCount(bi.getDocCount())
            , deletedDocCount(bi.getDeletedDocCount())
        {
        }
        Node(const Nodes& nodes)
            : commitId(INVALID_COMMIT)
        {
            docCount = 0;
            deletedDocCount = 0;
            for (size_t i = 0; i < nodes.size(); ++i)
            {
                docCount += (nodes[i].docCount - nodes[i].deletedDocCount);
            }
        }

        commitid_t commitId;
        df_t docCount;
        df_t deletedDocCount;
    };
    
    typedef std::map<size_t, Nodes> Tree;

private:
    void addToMerge(Tree& resultTree, Nodes& nodes, size_t nLevel);
    MergeProposalPtr makeProposal(const Tree& resultTree,
                                  const BarrelsInfo& barrelsInfo);
    size_t getLevel(df_t docCount);

private:
    size_t m_nCollisionFactor;
    size_t m_nScaleFactor;
    size_t m_nMaxTreeLevel;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_DBTMERGEPOLICY_H
