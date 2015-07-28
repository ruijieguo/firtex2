#include "firtex/index/InMemIndexBarrelReader.h"
#include "firtex/index/IndexBarrelWriter.h"
#include "firtex/utility/StringUtils.h"
#include "firtex/index/IndexBarrelKeeper.h"
#include "firtex/index/StoredFieldsReaderImpl.h"
#include "firtex/index/MultiFieldTermReader.h"

FX_NS_USE(document);
FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, InMemIndexBarrelReader);

InMemIndexBarrelReader::InMemIndexBarrelReader(
        const IndexBarrelWriter* pBarrelWriter, IndexBarrelKeeper* pKeeper) 
    : m_pKeeper(pKeeper)
    , m_bHasMultiIndexFields(false)
{
    init(pBarrelWriter);
}

InMemIndexBarrelReader::~InMemIndexBarrelReader() 
{
}

void InMemIndexBarrelReader::init(const IndexBarrelWriter* pBarrelWriter)
{
    m_pInStreamPool.reset(new InputStreamPool(m_pKeeper->getFileSystem()));
    m_pDocFilter = pBarrelWriter->getDocFilter();

    if (pBarrelWriter->getDocCount() > 0)
    {
        const DocumentSchema* pDocSchema = m_pKeeper->getDocSchema();

        /// Initialize in-memory stored fields reader
        m_sBarrelName = pBarrelWriter->getBarrelInfo().getSuffix();

        /// Initialize in-memory index/forward index reader
        LengthNormWriterPtr pNormWriter = pBarrelWriter->getLengthNormWriter();
        vector<TermReaderPtr> termReaders;
        DocumentSchema::Iterator it = pDocSchema->iterator();
        termReaders.reserve(it.size());
        m_forwardIndexes.reserve(it.size());
        while (it.hasNext())
        {
            const FieldSchema* pFieldSchema = it.next();
            fieldid_t fieldId = pFieldSchema->getId();
            if (pFieldSchema->isIndexed())
            {
                FieldIndexerPtr pIndexer = pBarrelWriter->getFieldIndexer(fieldId);
                TermReaderPtr pTermReader = pIndexer->termReader();
                termReaders.push_back(pTermReader);

                if (pFieldSchema->hasLengthNorm())
                {
                    LengthNormIteratorPtr pNormIt = pNormWriter->iterator(fieldId);
                    if (!m_pLengthNormReader)
                    {
                        m_pLengthNormReader.reset(new LengthNormReader());
                    }
                    m_pLengthNormReader->addNormIterator(fieldId, pNormIt);
                }
            }
        
            if (pFieldSchema->hasForwardIndex())
            {
                ForwardIndexWriterPtr pFdIndexWriter = pBarrelWriter->getForwardIndexWriter(fieldId);
                FIRTEX_ASSERT2(pFdIndexWriter);
                if (fieldId >= (fieldid_t)m_forwardIndexes.size())
                {
                    m_forwardIndexes.resize((size_t)fieldId + 1);
                }
                m_forwardIndexes[fieldId] = pFdIndexWriter->iterator();
            }
        }
    
        if (termReaders.size() > 1)
        {
            MultiFieldTermReader* pMultiTermReader =
                new MultiFieldTermReader(m_pKeeper->getComponentBuilder());
            m_pTermReader.reset(pMultiTermReader);
            for (size_t i = 0; i < termReaders.size(); ++i)
            {
                TermReaderPtr& p = termReaders[i];
                pMultiTermReader->addTermReader(p->getFieldSchema()->getName(), p);
            }
            m_bHasMultiIndexFields = true;
        }
        else if (termReaders.size() == 1) 
        {
            m_pTermReader = termReaders[0];
            m_bHasMultiIndexFields = false;
        }
    }
}

StoredFieldsReaderPtr InMemIndexBarrelReader::createStoredFieldsReader() const
{
    const DocumentSchema* pDocSchema = m_pKeeper->getDocSchema();
    StoredFieldsReaderImpl* pStoredReader = new StoredFieldsReaderImpl(pDocSchema);
    StoredFieldsReaderPtr pStoredReaderPtr(pStoredReader);
    pStoredReader->open(m_pInStreamPool, m_sBarrelName);
    return pStoredReaderPtr;
}

TermReaderPtr InMemIndexBarrelReader::termReader() const
{
    TermReaderPtr pTermReader(m_pTermReader->clone());
    return pTermReader;
}

TermReaderPtr InMemIndexBarrelReader::termReader(const std::string& sField) const
{
    if (!m_pTermReader)
        return TermReaderPtr();

    if (m_bHasMultiIndexFields)
    {
        MultiFieldTermReaderPtr pMultiFieldTermReader = 
            std::dynamic_pointer_cast<MultiFieldTermReader>(m_pTermReader);
        FIRTEX_ASSERT2(!!pMultiFieldTermReader);
        return pMultiFieldTermReader->termReader(sField);
    }
    else 
    {
        if (!strCompareNoCase(m_pTermReader->getFieldSchema()->getName().c_str(), sField.c_str()))
        {
            return TermReaderPtr(m_pTermReader->clone());
        }
    }
    return TermReaderPtr();
}

ForwardIndexIteratorPtr InMemIndexBarrelReader::forwardIndexReader(const std::string& sField) const
{
    const DocumentSchema* pDocSchema = m_pKeeper->getDocSchema();
    fieldid_t fieldId = pDocSchema->getFieldId(sField);
    return forwardIndexReader(fieldId);
}

ForwardIndexIteratorPtr InMemIndexBarrelReader::forwardIndexReader(fieldid_t fieldId) const
{
    if (fieldId < 0 || fieldId >= (fieldid_t)m_forwardIndexes.size())
    {
        return ForwardIndexIteratorPtr();
    }
    return m_forwardIndexes[fieldId];
}

LengthNormIteratorPtr InMemIndexBarrelReader::lengthNorm(const std::string& sField) const
{
    if (m_pLengthNormReader)
    {
        return m_pLengthNormReader->lengthNorm(sField);
    }
    return LengthNormIteratorPtr();
}

IndexBarrelReader* InMemIndexBarrelReader::clone() const
{
    return new InMemIndexBarrelReader(*this);
}

FX_NS_END

