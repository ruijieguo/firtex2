#include "firtex/index/StoredFieldsMerger.h"
#include "firtex/index/BarrelDirectory.h"

using namespace std;
FX_NS_USE(store);
FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, StoredFieldsMerger);

StoredFieldsMerger::StoredFieldsMerger() 
{
}

StoredFieldsMerger::~StoredFieldsMerger() 
{
}

void StoredFieldsMerger::merge(FileSystemPtr& pFileSys, 
                               const MergeInfos& mergeInfos)
{
    string sOutSuffix = mergeInfos.getSuffix();

    OutputStreamPtr pIdxOutStream = pFileSys->createFile(
            BarrelDirectory::getFilePath(STORED_FIELDS_IDX_FILENAME, sOutSuffix));
    OutputStreamPtr pValueOutStream = pFileSys->createFile(
            BarrelDirectory::getFilePath(STORED_FIELDS_VALUE_FILENAME, sOutSuffix));

    int32_t nBarrel = 0;
    for (size_t i = 0; i < mergeInfos.size(); i++)
    {
        const MergeInfo& mergeInfo = mergeInfos[i];
        if (mergeInfo.getBarrelInfo().getDocCount() <= 0)
        {
            continue;
        }

        string sSuffix = mergeInfo.getBarrelInfo().getSuffix();
        InputStreamPtr pIdxInStream = pFileSys->openFile(
                BarrelDirectory::getFilePath(STORED_FIELDS_IDX_FILENAME, sSuffix));
        InputStreamPtr pValueInStream = pFileSys->openFile(
                BarrelDirectory::getFilePath(STORED_FIELDS_VALUE_FILENAME, sSuffix));

        const DocIdRecycling* pDocIdRecy = mergeInfo.getDocIdRecycling();

        if ((nBarrel == 0) && !(pDocIdRecy->hasDeletions()))
        {
            pIdxOutStream->write(*pIdxInStream, pIdxInStream->getSize());
            pValueOutStream->write(*pValueInStream, pValueInStream->getSize());
        }
        else 
        {
            if (pDocIdRecy->hasDeletions())
            {
                int64_t nCurOff = pIdxInStream->readInt64();
                int64_t nNextOff = -1;
                df_t docCount = pDocIdRecy->getDocCount();
                docid_t docId = 0;
                for (; docId < (docid_t)docCount; ++docId)
                {
                    nNextOff = ((docId + 1) == docCount) 
                               ? pValueInStream->getSize() 
                               : pIdxInStream->readInt64();

                    if (pDocIdRecy->remap(docId) != INVALID_DOCID)
                    {
                        pIdxOutStream->writeInt64(pValueOutStream->tell());
                        pValueOutStream->write(*pValueInStream,
                                (size_t)(nNextOff - nCurOff));
                    }
                    pValueInStream->seek(nNextOff);
                    nCurOff = nNextOff;
                }
                if ((nNextOff == -1) && 
                    (pDocIdRecy->remap(docId) != INVALID_DOCID))
                {
                    pIdxOutStream->writeInt64(pValueOutStream->tell());
                    pValueOutStream->write(*pValueInStream,
                            pValueInStream->getSize());
                }
            } //end if (pDocIdRecy->hasDeletions())
            else 
            {
                offset_t nOffset = pValueOutStream->tell();
                int64_t nCurOff = 0;
                while (!pIdxInStream->isEof()) 
                {
                    nCurOff = (offset_t)pIdxInStream->readInt64();
                    if (nCurOff != -1)
                    {
                        nCurOff += nOffset;
                        FX_TRACE("idx off: %lld", nCurOff);
                        pIdxOutStream->writeInt64(nCurOff);
                    }
                    else 
                    {
                        pIdxOutStream->writeInt64(nCurOff);
                    }
                }
                pValueOutStream->write(*pValueInStream, pValueInStream->getSize());
            }
        }
        pIdxInStream->close();
        pValueInStream->close();
        nBarrel++;
    }

    pIdxOutStream->close();
    pValueOutStream->close();
}

FX_NS_END

