#include "firtex/search/FilterClause.h"

FX_NS_DEF(search);

const std::string FilterClause::PREFIX = "filter";

FilterClause::FilterClause(const std::string& sName,
                           const std::string& sValue)
    : Clause(sName, sValue)
{
}

FilterClause::~FilterClause() 
{
}

FX_NS_END

