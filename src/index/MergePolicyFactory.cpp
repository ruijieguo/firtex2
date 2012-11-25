#include "firtex/index/MergePolicyFactory.h"
#include "firtex/index/OptimizeMergePolicy.h"
#include "firtex/index/NoMergePolicy.h"
#include "firtex/index/DBTMergePolicy.h"
#include "firtex/utility/StringUtils.h"

FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, MergePolicyFactory);


MergePolicyFactory::MergePolicyFactory()
{
    registerMergePolicy(OptimizeMergePolicy::Creator::instance());
    registerMergePolicy(NoMergePolicy::Creator::instance());
    registerMergePolicy(DBTMergePolicy::Creator::instance());
}

void MergePolicyFactory::registerMergePolicy(MergePolicy::Creator* pCreator)
{
    SCOPED_LOCK();
    tstring str = toLower(pCreator->getIdentifier());
    if (m_creatorMap.find(str) != m_creatorMap.end())
    {
        FX_LOG(WARN, _T("Merge policy: [%s] is already registered."), str.c_str());
        return ;
    }
    m_creatorMap.insert(make_pair(str, pCreator));
}

MergePolicy* MergePolicyFactory::createMergePolicy(const tstring& sIdentifier)
{
    SCOPED_LOCK();

    tstring sLowId = toLower(sIdentifier);

    if (!startsWithNoCase(sLowId, tstring("merge_policy.")))
    {
        sLowId = "merge_policy." + sLowId;
    }

    CreatorMap::const_iterator iter = m_creatorMap.find(sLowId);
    if (iter != m_creatorMap.end())
    {
        return iter->second->create();
    }
    return NULL;
}

FX_NS_END

