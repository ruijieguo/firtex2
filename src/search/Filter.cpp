#include "firtex/search/Filter.h"

using namespace std;
FX_NS_USE(index);
FX_NS_USE(queryparser);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, Filter);

Filter::Filter() 
{
}

Filter::~Filter() 
{
}

bool Filter::init(const IndexReaderPtr& pIndexReader,
                  const FilterClausePtr& clause)
{
    const string& sFilterStr = clause->getFilterString();

    m_pParser.reset(new ExprParser(pIndexReader));
    m_pEvaluator = m_pParser->parse(sFilterStr);
    if (!m_pEvaluator)
    {
        return false;
    }
    return true;
}

FX_NS_END

