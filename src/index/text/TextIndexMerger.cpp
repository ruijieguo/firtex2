#include "firtex/index/text/TextIndexMerger.h"
#include "firtex/index/TypedTerm.h"
#include "firtex/index/BarrelDirectory.h"

FX_NS_USE(store);
FX_NS_USE(document);

FX_NS_DEF(index);

SETUP_LOGGER(index, TextIndexMerger);

TextIndexMerger::TextIndexMerger() 
    : m_nTotalTerms(0)
{
}

TextIndexMerger::~TextIndexMerger() 
{
}

void TextIndexMerger::beginMerge(const IndexMergeInfos& mergeFieldInfos)
{
    const FieldSchema* pFieldSchema = getFieldSchema();
    std::string sSuffix = mergeFieldInfos.getSuffix();

    m_pDictOutStream = getFileSystem()->createFile(
            BarrelDirectory::getFilePath(pFieldSchema->getName(),
                    DICT_FILEEXT, sSuffix));
    m_pDocOutStream = getFileSystem()->createFile(
            BarrelDirectory::getFilePath(pFieldSchema->getName(),
                    DOC_POSTING_FILEEXT, sSuffix));
    m_pPosOutStream = getFileSystem()->createFile(
            BarrelDirectory::getFilePath(pFieldSchema->getName(),
                    POS_POSTING_FILEEXT, sSuffix));
    m_pDictWriter.reset(new DictWriter(m_pDictOutStream));

    m_nTotalTerms = 0;
    m_pDictWriter->reserve(10000); //TODO: optimize
}

void TextIndexMerger::mergeTerms(const MergingTermVector& terms)
{
    const Term* pTerm = terms[0]->getTerm();
    const TypedTerm<uint64_t>* pTypedTerm = pTerm->cast<uint64_t>();
    FX_TRACE("Merge term: [%llu]", (unsigned long long int)pTypedTerm->getValue());

    m_postMerger.init(m_pDocOutStream, m_pPosOutStream);

    offset_t off = m_postMerger.merge(terms);
    if (off != (offset_t)-1)
    {
        FX_TRACE("Merged term: offset: [%lld]", (long long int)off);
        m_pDictWriter->pushBack(pTypedTerm->getValue(), off);
    }

    ++m_nTotalTerms;
}

void TextIndexMerger::endMerge()
{
    m_pDictWriter->finalize();

    FX_DEBUG("End merge: total terms: %llu, distinct terms: %llu",
             m_nTotalTerms, m_pDictWriter->size());

    m_pDictOutStream->close();
    m_pDocOutStream->close();
    m_pPosOutStream->close();

    m_pDictOutStream.reset();
    m_pDocOutStream.reset();
    m_pPosOutStream.reset();
}

FX_NS_END

