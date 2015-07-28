#include "firtex/index/LengthNormMerger.h"
#include "firtex/index/BarrelDirectory.h"

FX_NS_USE(store);
FX_NS_USE(utility);
FX_NS_USE(document);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, LengthNormMerger);

////////////////////////////////////////
///
LengthNormMerger::StandaloneNormMerger::StandaloneNormMerger(
        const FieldSchema& fieldSchema)
{
    m_sNormField = fieldSchema.getName();
}

void LengthNormMerger::StandaloneNormMerger::merge(FileSystemPtr& pFileSys,
        const MergeInfos& mergeInfos)
{
    std::string sSuffix = mergeInfos.getSuffix();
    OutputStreamPtr pNormOutStream = pFileSys->createFile(BarrelDirectory::getFilePath(
                    m_sNormField, NORM_FILEEXT, sSuffix));

    for (size_t i = 0; i < mergeInfos.size(); i++)
    {
        const MergeInfo& mergeInfo = mergeInfos[i];
            
        std::string sName = BarrelDirectory::getFilePath(m_sNormField, 
                NORM_FILEEXT, mergeInfo.getBarrelInfo().getSuffix());
        InputStreamPtr pNormInStream = pFileSys->openFile(sName);
        const DocIdRecycling* pDocIdRecy = mergeInfo.getDocIdRecycling();
        if (!pDocIdRecy->hasDeletions())
        {
            pNormOutStream->write(*pNormInStream, (size_t)pNormInStream->getSize());
        }
        else
        {
            df_t docCount = pDocIdRecy->getDocCount();
            for (docid_t docId = 0; docId < (docid_t)docCount; ++docId)
            {
                norm_t norm = pNormInStream->readInt32();
                if (pDocIdRecy->remap(docId) != INVALID_DOCID)
                {
                    pNormOutStream->writeInt32(norm);
                }
            }
        }
        pNormInStream->close();
    }
    pNormOutStream->close();
}

//////////////////////////////////////////////
//
LengthNormMerger::LengthNormMerger() 
{
}

LengthNormMerger::~LengthNormMerger() 
{
}

void LengthNormMerger::init(const DocumentSchema* pDocSchema)
{
    DocumentSchema::Iterator it = pDocSchema->iterator();
    while (it.hasNext())
    {
        const FieldSchema* pFieldSchema = it.next();
        if (pFieldSchema->isIndexed() && pFieldSchema->hasLengthNorm())
        {
            StandaloneNormMergerPtr merger(new StandaloneNormMerger(*pFieldSchema));
            m_merges.push_back(merger);
        }
    }
}

void LengthNormMerger::merge(FileSystemPtr& pFileSys,
                             const MergeInfos& mergeInfos)
{
    for (MergerList::iterator it = m_merges.begin();
         it != m_merges.end(); ++it)
    {
        (*it)->merge(pFileSys, mergeInfos);
    }
}

FX_NS_END

