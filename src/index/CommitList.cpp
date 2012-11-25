#include "firtex/index/CommitList.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/utility/NumberParser.h"

using namespace std;

FX_NS_USE(store);
FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, CommitList);

CommitList::CommitList() 
{
}

CommitList::~CommitList() 
{
}

void CommitList::load(FileSystemPtr& pFileSys)
{
    FileSystem::FileSet files = pFileSys->listFiles();

    commitid_t commit = INVALID_COMMIT;
    for (FileSystem::FileSet::const_iterator it = files.begin();
         it != files.end(); ++it)
    {
        string sFileName = *it;
        if (!strCompare(sFileName.c_str(), BARREL_COMMIT_FILENAME_PREFIX,
                        BARREL_COMMIT_FILENAME_PREFIX_LENGTH))
        {
            string sCom = sFileName.substr(BARREL_COMMIT_FILENAME_PREFIX_LENGTH);
            if (!NumberParser::tryParseInt32(sCom, commit))
            {
                FIRTEX_THROW_AND_LOG(IndexCollapseException,
                        "Invalid commit file: [%s]", sFileName.c_str());
            }
            m_commits.insert(commit);
        }
    }
}

FX_NS_END

