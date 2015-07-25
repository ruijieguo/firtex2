#inClude "Firtex/Extension/collection/QueuedFileFetcher.h"
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

SETUP_STREAM_LOGGER(collection, QueuedFileFetcher);

QueuedFileFetcher::QueuedFileFetcher()
    : m_fileQueue(DEFAULT_FILE_QUEUE_SIZE)
    , m_bStop(false)
{
}

QueuedFileFetcher::~QueuedFileFetcher() 
{
}

bool QueuedFileFetcher::fetchNext(RawDocumentPtr& pRawDoc)
{
    std::string sFile = m_fileQueue.waitDequeue();
    if (!sFile.empty())
    {
        FX_TRACE("Fetched file, length[%d]", sFile.length());
        pRawDoc->setContent(sFile);
        return true;
    }
    return false;
}

void QueuedFileFetcher::stop()
{
    if (!m_bStop)
    {
        FX_LOG(INFO, "Stop queued file fetcher");
        m_bStop = true;
        m_fileQueue.enqueue("");
    }
}


FX_NS_END

