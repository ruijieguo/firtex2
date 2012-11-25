#include <memory>
#include "firtex/utility/KeyValueParser.h"
#include "firtex/extension/collection/StandardCollection.h"
#include "firtex/extension/collection/MonitorDirectoryFileFetcher.h"
#include "firtex/extension/collection/DirectoryFileFetcher.h"
#include "firtex/extension/collection/DefaultFileFilter.h"
#include "firtex/extension/processor/StandardDocumentProcessor.h"

FX_NS_USE(utility);
FX_NS_USE(processor);

FX_NS_DEF(collection);

SETUP_STREAM_LOGGER(collection, StandardCollection);

StandardCollection::StandardCollection()
    : m_bMonitor(false)
{
}

StandardCollection::~StandardCollection() 
{
}

FileFetcher* StandardCollection::createFileFetcher()
{
    CollectionConfigurePtr pConfigure = getConfigure();
    if (m_bMonitor)
    {
        MonitorDirectoryFileFetcher* pFetcher = new MonitorDirectoryFileFetcher();
        pFetcher->init(pConfigure->Collection.source, getFileFilter());
        return pFetcher;
    }
    else
    {
        DirectoryFileFetcher* pFetcher = new DirectoryFileFetcher();
        pFetcher->init(pConfigure->Collection.source, getFileFilter());
        return pFetcher;
    }
}

DocumentProcessor* StandardCollection::createDocumentProcessor()
{
    CollectionConfigurePtr pConfigure = getConfigure();
    StandardDocumentProcessor* pDocProcessor = new StandardDocumentProcessor();
    auto_ptr<StandardDocumentProcessor> ptr(pDocProcessor);
    pDocProcessor->init(pConfigure->getDocSchema().get());
    return ptr.release();
}

FileFilter* StandardCollection::createFileFilter(const std::string& sParam)
{
    DefaultFileFilter* pFileFilter = new DefaultFileFilter(
            getConfigure()->Collection.source);
    if (!pFileFilter->init(sParam))
    {
        delete pFileFilter;
        return NULL;
    }
    return pFileFilter;
}

void StandardCollection::doInit(const CollectionConfigurePtr& pConfigure)
{
}

void StandardCollection::doInitParam(const std::string& sParam)
{
    KeyValueParser pp;
    if (pp.parse(sParam))
    {
        std::string sValue;
        // format: monitor=true/false
        if (pp.getValue(_T("monitor"), sValue))
        {
            if (!strCompareNoCase(sValue.c_str(), "true"))
            {
                m_bMonitor = true;
            }
            else 
            {
                m_bMonitor = false;
            }
        }
    }
}

void StandardCollection::waitStop()
{
    if (m_bMonitor)
    {
        FileFetcherPtr& pFileFetcher = getFileFetcher();
        
        MonitorDirectoryFileFetcherPtr pMonFetcher =
            pFileFetcher.cast<MonitorDirectoryFileFetcher>();
        FIRTEX_ASSERT2(pMonFetcher.isNotNull());
        pMonFetcher->stop();
    }
    Collection::waitStop();
}

FX_NS_END

