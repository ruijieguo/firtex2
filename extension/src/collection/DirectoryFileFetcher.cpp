#include "firtex/extension/collection/DirectoryFileFetcher.h"

FX_NS_USE(utility);
FX_NS_USE(processor);

FX_NS_DEF(collection);

SETUP_STREAM_LOGGER(collection, DirectoryFileFetcher);

DirectoryFileFetcher::DirectoryFileFetcher() 
{
}

DirectoryFileFetcher::~DirectoryFileFetcher() 
{
}

void DirectoryFileFetcher::init(const tstring& sDirectory,
                                const FileFilterPtr& pFileFilter)
{
    m_pDirIterator = new DirectoryIterator(sDirectory, true);
    m_pFileFilter = pFileFilter;
}

bool DirectoryFileFetcher::fetchNext(RawDocumentPtr& pRawDoc)
{
   while (m_pDirIterator->hasNext())
    {
        const File& file = m_pDirIterator->next();
        if (!file.isFile())
        {
            continue;
        }

        if (m_pFileFilter.isNotNull())
        {
            const Path& filePath = m_pDirIterator->getPath();
            if (m_pFileFilter->isFiltered(filePath))
            {
                FX_DEBUG("File : [%s] filtered", filePath.toString().c_str());
                continue;
            }
        }
        FX_DEBUG("Fetching file : [%s].", file.getPath().c_str());
        pRawDoc->setPath(file.getPath());
        return true;
    }

   return false;
}

FX_NS_END

