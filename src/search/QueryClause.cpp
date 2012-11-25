#include "firtex/search/QueryClause.h"

FX_NS_DEF(search);

const std::string QueryClause::PREFIX = "query";

QueryClause::QueryClause(const std::string& sName,
                         const std::string& sValue) 
    : Clause(sName, sValue)
{    
}

QueryClause::~QueryClause() 
{
}

FX_NS_END

