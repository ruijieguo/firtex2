#include "firtex/index/text/TextComponentCreator.h"
#include "firtex/index/text/TextIndexer.h"
#include "firtex/index/text/TextIndexMerger.h"
#include "firtex/index/text/TextTermReader.h"

FX_NS_USE(utility);
FX_NS_USE(document);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, TextComponentCreator);

TextComponentCreator::TextComponentCreator() 
{
    m_identifier += _T(".text");
}

TextComponentCreator::~TextComponentCreator() 
{
}

FieldIndexer* TextComponentCreator::createIndexer(
        const FieldSchema* pFieldSchema) const
{
    return new TextIndexer(pFieldSchema);
}

FieldMerger* TextComponentCreator::createMerger() const
{
    return new TextIndexMerger();
}

TermReader* TextComponentCreator::createTermReader() const
{
    return new TextTermReader();
}

FX_NS_END

