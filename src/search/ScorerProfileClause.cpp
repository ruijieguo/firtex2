#include "firtex/search/ScorerProfileClause.h"
#include "firtex/utility/KeyValueParser.h"

using namespace std;
FX_NS_USE(utility);

FX_NS_DEF(search);
SETUP_LOGGER(search, ScorerProfileClause);

const std::string ScorerProfileClause::PREFIX = "scorer_profile";

ScorerProfileClause::ScorerProfileClause(const std::string& sName,
        const std::string& sValue)
    : Clause(sName, sValue)
{
    fromString(sValue);
}

ScorerProfileClause::~ScorerProfileClause() 
{
}

void ScorerProfileClause::fromString(const string& sClauseValue)
{
    string sKey, sValue, sSkipBlock;
    Clause::parseClauseValue(sKey, sValue, sSkipBlock, sClauseValue, 0);
    m_sProfileName = sKey;
    if (!sSkipBlock.empty())
    {
        KeyValueParser kvParser;
        if (!kvParser.parse(sSkipBlock, ",", "="))
        {
            FX_LOG(ERROR, "Parse scorer_profile parameter FAILED: [%s]", 
                   sSkipBlock.c_str());
        }
        for (size_t i = 0; i < kvParser.getKVCount(); ++i)
        {
            string k, v;
            if (kvParser.getKeyValue(i, k, v))
            {
                m_parameters.insert(make_pair(k, v));
            }
        }
    }
}

FX_NS_END

