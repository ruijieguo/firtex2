#include "firtex/index/DBTMergePolicy.h"

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, DBTMergePolicy);

DBTMergePolicy::DBTMergePolicy()
    : m_nCollisionFactor(DEFAULT_COLLISION_FACTOR)
    , m_nScaleFactor(DEFAULT_SCALE_FACTOR)
    , m_nMaxTreeLevel(MAX_TREE_LEVEL) 
{
}

DBTMergePolicy::~DBTMergePolicy() 
{
}

MergeProposalPtr DBTMergePolicy::createProposals(const BarrelsInfo& barrelsInfo)
{
    Tree mergeTree, resultTree; 
    BarrelsInfo::Iterator it = barrelsInfo.iterator();
    while (it.hasNext())
    {
        const BarrelInfo& barrelInfo = it.next();
        Node node(barrelInfo);

        size_t nLevel = getLevel(node.docCount);
        Tree::iterator treeIt = mergeTree.find(nLevel);
        while (true)
        {
            if (treeIt != mergeTree.end())
            {
                Nodes& nodes = treeIt->second;
                nodes.push_back(node);
                if (nodes.size() >= m_nCollisionFactor)
                {
                    node = Node(nodes);
                    addToMerge(resultTree, nodes, nLevel);

                    nLevel = getLevel(node.docCount);
                    treeIt = mergeTree.find(nLevel);
                }
                else
                {
                    break;
                }
            }
            else
            {
                Nodes nodes;
                nodes.push_back(node);
                mergeTree.insert(make_pair(nLevel, nodes));
                break;
            }
        }
    }

    return makeProposal(resultTree, barrelsInfo);
}

void DBTMergePolicy::addToMerge(Tree& resultTree, Nodes& nodes, size_t nLevel)
{
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        const Node& node = nodes[i];
        if (node.commitId != INVALID_COMMIT)
        {
            Tree::iterator treeIt = resultTree.find(nLevel);
            if (treeIt != resultTree.end())
            {
                Nodes& nodes2 = treeIt->second;
                nodes2.push_back(node);
            }
            else
            {
                Nodes nodes2;
                nodes2.push_back(node);
                resultTree.insert(make_pair(nLevel, nodes2));
            }
        }
    }
    nodes.clear();
}

MergeProposalPtr DBTMergePolicy::makeProposal(const Tree& resultTree,
        const BarrelsInfo& barrelsInfo)
{
    MergeProposalPtr pProposal = new MergeProposal();

    size_t nLastLevel = (size_t)-1;
    BarrelsInfoPtr pBarrelsInfo;
    for (Tree::const_iterator it = resultTree.begin();
         it != resultTree.end(); ++it)
    {
        if (it->first > (nLastLevel + 1))
        {
            if (pBarrelsInfo.isNotNull())
            {
                FX_TRACE("Add one merge proposal");
                pProposal->push_back(pBarrelsInfo);
            }
            pBarrelsInfo.assign(new BarrelsInfo());
        }

        const Nodes& nodes = it->second;
        for (size_t i = 0; i < nodes.size(); ++i)
        {
            const Node& node = nodes[i];
            FX_TRACE("Add one barrel to merge: [%d]", node.commitId);

            const BarrelInfo* pBarrelInfo = barrelsInfo.getBarrel(node.commitId);
            FIRTEX_ASSERT2(pBarrelInfo);
            pBarrelsInfo->addBarrel(*pBarrelInfo);
        }
        nLastLevel = it->first;
    }

    if (pBarrelsInfo.isNotNull())
    {
        pProposal->push_back(pBarrelsInfo);
    }

    if (pProposal->size() > 0)
    {
        return pProposal;
    }

    return MergeProposalPtr();
}

size_t DBTMergePolicy::getLevel(df_t docCount)
{
    size_t nLevel = 0;
    df_t levelDocCount = (df_t)(m_nCollisionFactor * m_nScaleFactor);
    while (docCount > levelDocCount)
    {
        levelDocCount *= (df_t)m_nCollisionFactor;
        ++nLevel;
    }
    return nLevel;
}

FX_NS_END

