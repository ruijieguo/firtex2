#include "firtex/index/text/TextIndexer.h"
#include "firtex/index/text/InMemTextTermReader.h"
#include <limits>

FX_NS_DEF(index);

SETUP_LOGGER(index, TextIndexer);

TermReaderPtr TextIndexer::termReader() const
{
    InMemTextTermReader* pTermReader = new InMemTextTermReader();
    TermReaderPtr pTermReaderPtr(pTermReader);
    pTermReader->init(this->getFieldSchema(), this->m_pPostingTable, NULL);
    return pTermReaderPtr;
}

FX_NS_END

