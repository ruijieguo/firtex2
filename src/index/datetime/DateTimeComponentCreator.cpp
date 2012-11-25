#include "firtex/index/datetime/DateTimeComponentCreator.h"
#include "firtex/index/datetime/DateTimeIndexer.h"
#include "firtex/index/datetime/DateTimeIndexMerger.h"
#include "firtex/index/datetime/DateTimeTermReader.h"

FX_NS_USE(utility);
FX_NS_USE(document);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, DateTimeComponentCreator);

DateTimeComponentCreator::DateTimeComponentCreator() 
{
    m_identifier += ".datetime";
}

DateTimeComponentCreator::~DateTimeComponentCreator() 
{
}

FieldIndexer* DateTimeComponentCreator::createIndexer(
        const FieldSchema* pFieldSchema) const
{
    return new DateTimeIndexer(pFieldSchema);
}

FieldMerger* DateTimeComponentCreator::createMerger() const
{
    return new DateTimeIndexMerger();
}

TermReader* DateTimeComponentCreator::createTermReader() const
{
    return new DateTimeTermReader();
}

FX_NS_END

