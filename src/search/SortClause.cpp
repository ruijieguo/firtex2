#include "firtex/search/SortClause.h"
#include "firtex/utility/StringTokenizer.h"

using namespace std;

FX_NS_USE(utility);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, SortClause);

const string SortClause::PREFIX = "sort_by";
const string SortClause::SORTER_SEPARATOR = ",";

SortClause::SortClause(const string& sName,
                       const string& sValue)
    : Clause(sName, sValue)
{
    fromString(sValue);
}

SortClause::~SortClause() 
{
}

void SortClause::fromString(const string& sValue)
{
    StringTokenizer st(sValue, SORTER_SEPARATOR,
                       StringTokenizer::TOKEN_IGNORE_EMPTY 
                       | StringTokenizer::TOKEN_TRIM);
    if (st.getNumTokens() == 0)
    {
        FX_LOG(WARN, "Sort clause [%s] is empty or invalid.", sValue.c_str());
        return ;
    }
    
    m_sorters.assign(st.begin(), st.end());
}

FX_NS_END

