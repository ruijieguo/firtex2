#include "ThriftSearchServiceHelper.h"
#include "firtex/search/SortClause.h"
#include "firtex/search/QueryClause.h"
#include "firtex/search/ParameterClause.h"
#include "firtex/search/FieldSelectClause.h"
#include "firtex/utility/NumberFormatter.h"

using namespace std;
FX_NS_USE(search);
FX_NS_USE(utility);

FX_NS_DEF(app);

void ThriftSearchServiceHelper::queryCommandToStatmentString(string& sUri, 
        const QueryCommand& cmd)
{
    if (!cmd.query.empty())
    {
        sUri += QueryClause::PREFIX + "=" + cmd.query + ";";
    }

    if (!cmd.sortBy.empty())
    {
        sUri += SortClause::PREFIX + "=";
        vectorToString(sUri, cmd.sortBy, ",");
        sUri += ";";
    }

    if (!cmd.filter.empty())
    {
        sUri += FilterClause::PREFIX + "=" + cmd.filter + ";";
    }

    sUri += ParameterClause::PREFIX + "=start:";
    NumberFormatter::append(sUri, cmd.offset);
    sUri += ",top:";
    NumberFormatter::append(sUri, cmd.limit);

    if (!cmd.traceLevel.empty())
    {
        sUri += ParameterClause::TRACE_PREFIX + "=" + cmd.traceLevel;
    }

    sUri += ";";

    if (!cmd.fieldSelect.empty())
    {
        sUri += FieldSelectClause::PREFIX + "=";
        vectorToString(sUri, cmd.fieldSelect, ",");
        sUri += ";";
    }

    if (!cmd.encode.empty())
    {
        sUri += "urlencode=" + cmd.encode + ";";
    }

    if (!cmd.cluster.empty())
    {
        sUri += "cluster=";
        vectorToString(sUri, cmd.cluster, ",");
        sUri += ";";
    }
}

void ThriftSearchServiceHelper::vectorToString(string& sResult, 
        const vector<string>& strs, const string& sSep)
{
    for (vector<string>::const_iterator it = strs.begin();
         it != strs.end();)
    {
        sResult += *it;
        if (++it != strs.end())
        {
            sResult += sSep;
        }
    }   
}


FX_NS_END
