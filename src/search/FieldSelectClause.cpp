#include "firtex/search/FieldSelectClause.h"
#include "firtex/utility/KeyValueParser.h"

using namespace std;
FX_NS_USE(utility);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, FieldSelectClause);

const string FieldSelectClause::PREFIX = "field_select";

FieldSelectClause::FieldSelectClause(const string& sName,
                                     const string& sValue)
    : Clause(sName, sValue)
{
    fromString(sValue);
}

FieldSelectClause::~FieldSelectClause() 
{
}

void FieldSelectClause::fromString(const string& sClauseValue)
{
    size_t n = 0;
    while (n < sClauseValue.size())
    {
        string sKey, sValue, sSkipBlock;
        n = Clause::parseClauseValue(sKey, sValue, sSkipBlock, sClauseValue, n);
        if (sValue == "snippet")
        {
            SnippetParam p(sKey, true);
            if (!sSkipBlock.empty())
            {
                KeyValueParser kvParser;
                if (!kvParser.parse(sSkipBlock, ",", "="))
                {
                    FX_LOG(WARN, "Parse parameter clause [%s] FAILED", 
                            sSkipBlock.c_str());
                }
                else
                {
                    kvParser.getValue("pre_tag", p.preTag);
                    kvParser.getValue("post_tag", p.postTag);
                    kvParser.getValue("separator", p.separator);
                }
            }
            m_fields.push_back(p);
        }
        else
        {
            m_fields.push_back(SnippetParam(sKey, false));
        }
    }
}

FX_NS_END

