#include "firtex/index/KeepByCommitIndexCleaner.h"
#include "firtex/utility/NumberFormatter.h"
#include "firtex/utility/KeyValueParser.h"
#include "firtex/index/BarrelsInfo.h"
#include "firtex/index/CommitList.h"

FX_NS_USE(utility);
FX_NS_USE(store);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, KeepByCommitIndexCleaner);

KeepByCommitIndexCleaner::KeepByCommitIndexCleaner()
    : m_uKeepCount(0)
{
    m_sFileBuffer.reserve(DEFAULT_FILE_BUFFER);
}

KeepByCommitIndexCleaner::~KeepByCommitIndexCleaner() 
{
}

void KeepByCommitIndexCleaner::init(const FX_NS(store)::FileSystemPtr& pFileSys,
                                    const std::string& sParam)
{
    m_pFileSys = pFileSys;
    if (sParam.empty())
    {
        return;
    }

    KeyValueParser kvParser;
    uint32_t uKeepCount = 0;
    if (!kvParser.parse(sParam, ";", "=") || !kvParser.getValue("keep_count", uKeepCount))
    {
        FX_LOG(ERROR, "Invalid cleaner parameter [%s]", sParam.c_str());
        return;
    }

    m_uKeepCount = uKeepCount;
}

void KeepByCommitIndexCleaner::cleanup()
{
    if (m_uKeepCount == 0)
    {
        return;
    }

    CommitList commits;
    commits.load(m_pFileSys);

    CommitSet removeCommits;
    uint32_t nCommits = (uint32_t)commits.size();
    CommitList::Iterator it = commits.iterator();
    while (nCommits > m_uKeepCount)
    {
        commitid_t comId = it.next();
        string sComFile = BARREL_COMMIT_FILENAME_PREFIX;
        NumberFormatter::append(sComFile, comId);
        extractCommits(comId, removeCommits);

        FX_LOG(INFO, "Clean up staled barrel: [%s]", sComFile.c_str());
        m_pFileSys->deleteFile(sComFile);
        --nCommits;
    }

    CommitSet reserveCommits;
    while (it.hasNext())
    {
        commitid_t comId = it.next();
        string sComFile = BARREL_COMMIT_FILENAME_PREFIX;
        NumberFormatter::append(sComFile, comId);
        if (!m_pFileSys->fileExists(sComFile))
        {
            FIRTEX_THROW(IndexCollapseException, "Barrel file: [%s] not exist",
                         sComFile.c_str());
        }

        extractCommits(comId, reserveCommits);
    }
    
    for (CommitSet::const_iterator comIt = removeCommits.begin();
         comIt != removeCommits.end(); ++comIt)
    {
        if (reserveCommits.find(*comIt) == reserveCommits.end())
        {
            string sDir = BARREL_DIRECTORY_PREFIX;
            NumberFormatter::append(sDir, *comIt);
            
            FX_LOG(INFO, "Clean up staled index data: [%s]", sDir.c_str());
            m_pFileSys->deleteDirectory(sDir, true);
        }
    }
}

void KeepByCommitIndexCleaner::extractCommits(commitid_t comId, CommitSet& commits)
{
    BarrelsInfo barrelsInfo;
    barrelsInfo.read(m_pFileSys, comId);
    BarrelsInfo::Iterator barrelIt = barrelsInfo.iterator();
    while (barrelIt.hasNext())
    {
        const BarrelInfo& barrelInfo = barrelIt.next();
        commitid_t comId = barrelInfo.getCommitId();
        if (commits.find(comId) == commits.end())
        {
            commits.insert(comId);
        }
    }
}

FX_NS_END

