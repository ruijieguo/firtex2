#include "firtex/index/SequenceMergeScheduler.h"

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, SequenceMergeScheduler);

SequenceMergeScheduler::SequenceMergeScheduler() 
{
}

SequenceMergeScheduler::~SequenceMergeScheduler() 
{
}

void SequenceMergeScheduler::merge(const MergeablePtr& pMergeObj)
{
    MergeablePtr pTmpObj = pMergeObj;
    pTmpObj->merge();
}

FX_NS_END

