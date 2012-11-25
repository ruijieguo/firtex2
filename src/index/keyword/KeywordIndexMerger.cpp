#include "firtex/index/keyword/KeywordIndexMerger.h"
#include "firtex/index/TypedTerm.h"

FX_NS_USE(store);
FX_NS_USE(document);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, KeywordIndexMerger);

KeywordIndexMerger::KeywordIndexMerger() 
    : m_nTotalTerms(0)
{
}

KeywordIndexMerger::~KeywordIndexMerger() 
{
}

void KeywordIndexMerger::beginMerge(const IndexMergeInfos& mergeFieldInfos)
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

void KeywordIndexMerger::mergeTerms(const MergingTermVector& terms)
{
    const Term* pTerm = terms[0]->getTerm();
    const TypedTerm<uint64_t>* pTypedTerm = pTerm->cast<uint64_t>();
    DocPostingMerger postMerger;
    postMerger.init(m_pDocOutStream);

    offset_t off = postMerger.merge(terms);
    if (off != (offset_t)-1)
    {
        m_pDictWriter->pushBack(pTypedTerm->getValue(), off);
    }
}

void KeywordIndexMerger::endMerge()
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

