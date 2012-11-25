#include "firtex/index/datetime/DateTimeIndexMerger.h"
#include "firtex/index/TypedTerm.h"

FX_NS_USE(store);
FX_NS_USE(document);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, DateTimeIndexMerger);

DateTimeIndexMerger::DateTimeIndexMerger() 
    : m_nTotalTerms(0)
{
}

DateTimeIndexMerger::~DateTimeIndexMerger() 
{
}

void DateTimeIndexMerger::beginMerge(const IndexMergeInfos& mergeFieldInfos)
{
    std::string sSuffix = mergeFieldInfos.getSuffix();
    m_pDictOutStream = getFileSystem()->createFile(
            BarrelDirectory::getFilePath(getFieldSchema()->getName(),
                    DICT_FILEEXT, sSuffix));
    m_pDocOutStream = getFileSystem()->createFile(
            BarrelDirectory::getFilePath(getFieldSchema()->getName(),
                    DOC_POSTING_FILEEXT, sSuffix));
    m_pDictWriter.assign(new DictWriter(m_pDictOutStream));

    m_pDictWriter->reserve(10000); //TODO:
}

void DateTimeIndexMerger::mergeTerms(const MergingTermVector& terms)
{
    const Term* pTerm = terms[0]->getTerm();
    const TypedTerm<int64_t>* pTypedTerm = pTerm->cast<int64_t>();
    DocPostingMerger postMerger;
    postMerger.init(m_pDocOutStream);

    offset_t off = postMerger.merge(terms);
    if (off != (offset_t)-1)
    {
        m_pDictWriter->pushBack(pTypedTerm->getValue(), off);
    }
}

void DateTimeIndexMerger::endMerge()
{
    m_pDictWriter->finalize();

    FX_DEBUG("End merge: total terms: %llu, distinct terms: %llu",
             m_nTotalTerms, m_pDictWriter->size());

    m_pDictOutStream->close();
    m_pDocOutStream->close();

    m_pDictOutStream.reset();
    m_pDocOutStream.reset();
}

FX_NS_END

