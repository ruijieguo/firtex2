#include "firtex/index/keyword/KeywordComponentCreator.h"
#include "firtex/index/keyword/KeywordIndexer.h"
#include "firtex/index/keyword/KeywordIndexMerger.h"
#include "firtex/index/keyword/KeywordTermReader.h"

FX_NS_USE(utility);
FX_NS_USE(document);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, KeywordComponentCreator);

KeywordComponentCreator::KeywordComponentCreator() 
{
    m_identifier += _T(".keyword");
}

KeywordComponentCreator::~KeywordComponentCreator() 
{
}

FieldIndexer* KeywordComponentCreator::createIndexer(
        const FieldSchema* pFieldSchema) const
{
    return new KeywordIndexer(pFieldSchema);
}

FieldMerger* KeywordComponentCreator::createMerger() const
{
    return new KeywordIndexMerger();
}

TermReader* KeywordComponentCreator::createTermReader() const
{
    return new KeywordTermReader();
}

FX_NS_END

