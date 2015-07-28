#include "firtex/extension/collection/MonitorDirectoryFileFetcher.h"
#include "firtex/utility/TextFile.h"
#include "firtex/utility/DirectoryIterator.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/NumberParser.h"
#include "firtex/utility/NumberFormatter.h"
#include "firtex/thread/Thread.h"

FX_NS_USE(utility);
FX_NS_USE(thread);
FX_NS_USE(processor);

FX_NS_DEF(collection);

SETUP_STREAM_LOGGER(collection, MonitorDirectoryFileFetcher);

std::string MonitorDirectoryFileFetcher::INCREMENTAL_LIST = "INCREMENTAL_LIST";

MonitorDirectoryFileFetcher::MonitorDirectoryFileFetcher()
    : m_nLastOffset(-1)
    , m_fileQueue(DEFAULT_FILE_QUEUE_SIZE)
    , m_bStop(false)
{
}

MonitorDirectoryFileFetcher::~MonitorDirectoryFileFetcher() 
{
}

void MonitorDirectoryFileFetcher::init(const std::string& sDirectory,
                                       const FileFilterPtr& pFileFilter)
{
    m_sRootDir = sDirectory;
    m_sIncList = sDirectory + Path::separator() + INCREMENTAL_LIST;
    m_pFileFilter = pFileFilter;
    m_monitorThread.start(*this);
}

bool MonitorDirectoryFileFetcher::fetchNext(RawDocumentPtr& pRawDoc)
{
    std::string sFilePath = m_fileQueue.waitDequeue();
    if (!sFilePath.empty())
    {
        FX_TRACE("Fetched file: [%s]", sFilePath.c_str());
        pRawDoc->setPath(sFilePath);
        return true;
    }
    return false;
}

void MonitorDirectoryFileFetcher::run()
{
    while (!m_bStop)
    {
        checkIncrementList();

        for (size_t i = 0; i < DEFAULT_MONITOR_TIME_INTERVAL/1000 && !m_bStop; ++i)
        {
            Thread::sleep(1000); // sleep 1 second
        }
    }
}

void MonitorDirectoryFileFetcher::stop()
{
    if (!m_bStop)
    {
        FX_LOG(INFO, "Stop monitor directory file fetcher");
        m_bStop = true;
        m_fileQueue.enqueue("");
        m_monitorThread.join();
    }
}

void MonitorDirectoryFileFetcher::checkIncrementList()
{
    if (m_nLastOffset == -1)
    {
        File f(m_sIncList);
        if (!f.exists())
        {
            buildIncrementList();
        }
        m_nLastOffset = 0;
    }
    
    TextFile file;
    try
    {
        file.open(m_sIncList, "r");
        file.setPos(m_nLastOffset);

        char filePath[MAX_PATH_LENGTH];
        ssize_t readLen = -1;
        while ((readLen = file.readLine(filePath, MAX_PATH_LENGTH, true)) != -1)
        {
            if (filePath[readLen - 1] != '')
            {
                FX_LOG(WARN, "Found a invalid file path line: [%s], "
                       "missing end mark: []", filePath);
                continue;
            }
            filePath[readLen - 1] = '\0'; // Remove '' at tail

            if (m_pFileFilter)
            {
                Path p(filePath);
                if (m_pFileFilter->isFiltered(p))
                {
                    FX_DEBUG("File : [%s] is filtered.", filePath);
                    continue;
                }
            }

            
            FX_DEBUG("Found a new file: [%s].", filePath);

            m_fileQueue.enqueue(m_sRootDir + Path::separator() + filePath);
        }
        m_nLastOffset = file.getPos();
        file.close();
    }
    catch(const FirteXException& e)
    {
        FX_LOG(ERROR, "Read [%s] FAILED, reason: [%s].", 
               m_sIncList.c_str(), e.what().c_str());
    }
}

void MonitorDirectoryFileFetcher::buildIncrementList()
{
    TextFile listFile;
    try
    {
        listFile.open(m_sIncList, "w");

        char fileLine[MAX_PATH_LENGTH];
        DirectoryIterator dirIterator(m_sRootDir, true);
        while (dirIterator.hasNext())
        {
            const File& file = dirIterator.next();
            if (file.isDirectory())
            {
                continue;
            }
        
            const Path& filePath = dirIterator.getPath();
            if (!strCompare(filePath.getFileName().c_str(), INCREMENTAL_LIST.c_str()))
            {
                continue;
            }

            if (m_pFileFilter)
            {
                if (m_pFileFilter->isFiltered(filePath))
                {
                    continue;
                }
            }

            const std::string& sFilePath = file.getPath();
            SNPRINTF(fileLine, MAX_PATH_LENGTH, "%s\n", sFilePath.c_str() + m_sRootDir.length());
        
            FX_DEBUG("Found new file: [%s].", sFilePath.c_str());
            listFile.write(fileLine);
        }
        
        listFile.close();
    }
    catch(const FirteXException& e)
    {
        FX_LOG(ERROR, "Build increment list file: [%s] FAILED, reason: [%s].", 
               m_sIncList.c_str(), e.what().c_str());
        throw;
    }
}

// void MonitorDirectoryFileFetcher::checkUpdate(const std::string& sDirPath)
// {
//     TextFile file;
//     try
//     {
//         file.open(sDirPath + "/" + STATUS_FILE_NAME, "a");

//         char line[128];
//         ssize_t lastRead = -1;
//         ssize_t readLen = -1;
//         while ((readLen = file.readLine(line, 128)) != -1)
//         {
//             lastRead = readLen;
//         }

//         int64_t uFileId = -1;
//         if (lastRead != -1)
//         {
//             if (!NumberParser::tryParseInt64(line, uFileId))
//             {
//                 FX_LOG(ERROR, "Invalid file id [%line] in status file.", line);
//             }
//         }

//         ++uFileId;
//         std::string sFileName;
//         std::string sFilePath;
//         for (;;)
//         {
//             sFileName.clear();
//             NumberFormatter::append(sFileName, uFileId);
//             sFilePath = sDirPath + sFileName;
//             if (File::exists(sFilePath))
//             {
//                 if (m_pFileFilter)
//                 {
//                     Path filePath(sFilePath);
//                     if (m_pFileFilter->isFiltered(filePath))
//                     {
//                         continue;
//                     }
//                 }

//                 FX_DEBUG("Found new file: [%s].", sFilePath.c_str());
//                 m_fileQueue.enqueue(sFilePath);
//                 file.write(sFileName.c_str(), true);
//                 file.flush();
//             }
//             else
//             {
//                 break;
//             }
//             ++uFileId;
//         }
//         file.close();
//     } 
//     catch(const FileIOException& e)
//     {
//         FX_LOG(ERROR, e.what());
//     }    
// }

FX_NS_END

