#include "firtex/index/OptimizeMergePolicy.h"

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, OptimizeMergePolicy);

OptimizeMergePolicy::OptimizeMergePolicy() 
{
}

OptimizeMergePolicy::~OptimizeMergePolicy() 
{
}

MergeProposalPtr OptimizeMergePolicy::createProposals(
        const BarrelsInfo& barrelsInfo)
{
    MergeProposalPtr pProp(new MergeProposal());
    pProp->push_back(BarrelsInfoPtr(barrelsInfo.clone()));
    return pProp;
}

FX_NS_END

