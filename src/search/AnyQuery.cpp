#include "firtex/search/AnyQuery.h"
#include "firtex/search/AnyQueryExecutor.h"
#include <sstream>

using namespace std;
FX_NS_USE(utility);
FX_NS_USE(index);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, AnyQuery);

AnyQuery::AnyQuery() 
{
}

AnyQuery::AnyQuery(const AnyQuery& src) 
{
}

AnyQuery::~AnyQuery() 
{
}

string AnyQuery::getIdentifier() const
{
    return "AnyQuery";
}

size_t AnyQuery::size() const
{
    return 1;
}

QueryExecutorPtr AnyQuery::createExecutor(IndexReaderPtr& pIndexReader,
        FeatureProviderPtr& pProvider, PoolPtr& pPool) const
{
    QueryExecutorPtr pExe(new AnyQueryExecutor(pIndexReader,
                    pProvider, pPool));
    return pExe;
}

Query* AnyQuery::clone() const
{
    return new AnyQuery(*this);
}

string AnyQuery::toString() const
{
    stringstream ss;
    ss << "AnyQuery: [*:*]";
    return ss.str();
}

FX_NS_END

