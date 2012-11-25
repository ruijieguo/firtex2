#include "firtex/search/ParameterClause.h"
#include "firtex/utility/KeyValueParser.h"
#include "firtex/utility/NumberParser.h"

using namespace std;
FX_NS_USE(utility);
FX_NS_USE(common);

FX_NS_DEF(search);

SETUP_LOGGER(search, ParameterClause);

const std::string ParameterClause::PREFIX = "parameter";

const std::string ParameterClause::STARTDOC_PREFIX = "start";
const std::string ParameterClause::TOPDOCS_PREFIX = "top";
const std::string ParameterClause::PHASE_PREFIX = "phase";
const std::string ParameterClause::TRACE_PREFIX = "trace";
const std::string ParameterClause::SEPARATOR = ",";
const std::string ParameterClause::KV_SEPARATOR = ":";

ParameterClause::ParameterClause(const std::string& sName,
                                 const std::string& sValue)
    : Clause(sName, sValue)
    , m_nStartDoc(0)
    , m_nTopDocs(DEFAULT_TOPDOCS)
    , m_nQueryPhase(0)
    , m_traceLevel(LoggingLevel::LEVEL_MAX)
{
    fromString(sValue);
}

ParameterClause::~ParameterClause() 
{
}

void ParameterClause::fromString(const std::string& sValue)
{

#define PARSE_UINT32_PARAM(prefix, key, strVal, val)            \
    if (!strCompareNoCase(key.c_str(), prefix.c_str()))         \
    {                                                           \
        uint32_t tmpVal;                                        \
        if (!NumberParser::tryParseUInt32(strVal, tmpVal))      \
        {                                                       \
            FX_LOG(WARN, "Parse parameter [%s] FAILED", key.c_str());  \
            continue;                                           \
        }                                                       \
        val = tmpVal;                                           \
    }

#define PARSE_STR_PARAM(prefix, key, strVal, val)               \
    if (!strCompareNoCase(key.c_str(), prefix.c_str()))         \
    {                                                           \
        val = strVal;                                           \
    }
    
    KeyValueParser kvParser;
    if (!kvParser.parse(sValue, SEPARATOR, KV_SEPARATOR))
    {
        FX_LOG(WARN, "Parse parameter clause [%s]", sValue.c_str());
        return ;
    }

    string k, v;
    for (size_t i = 0; i < kvParser.getKVCount(); ++i)
    {
        kvParser.getKeyValue(i, k, v);
        PARSE_UINT32_PARAM(STARTDOC_PREFIX, k, v, m_nStartDoc)
        else PARSE_UINT32_PARAM(TOPDOCS_PREFIX, k, v, m_nTopDocs)
        else PARSE_UINT32_PARAM(PHASE_PREFIX, k, v, m_nQueryPhase)
        else if (!strCompareNoCase(k.c_str(), TRACE_PREFIX.c_str()))
        {
            try
            {
                m_traceLevel = LoggingLevel::strToLevel(v);
            } 
            catch(BadParameterException& e)
            {
                FX_LOG(WARN, e.what().c_str());
                m_traceLevel = LoggingLevel::LEVEL_MAX;
            }
        }
        else
        {
            FX_LOG(WARN, "Unknow parameter [%s]", k.c_str());
            continue;
        }
    }

#undef PARSE_UINT32_PARAM
}

void ParameterClause::toString(std::stringstream& ss, const std::string& sEqSymbol) const
{
    ss << "parameter" << sEqSymbol;
    ss << STARTDOC_PREFIX << KV_SEPARATOR << m_nStartDoc
       << SEPARATOR << TOPDOCS_PREFIX << KV_SEPARATOR << m_nTopDocs;
    if (m_nQueryPhase != 0)
    {
        ss << SEPARATOR << PHASE_PREFIX << KV_SEPARATOR << m_nQueryPhase;
    }
    if (m_traceLevel != LoggingLevel::LEVEL_MAX)
    {
        ss << SEPARATOR << TRACE_PREFIX << KV_SEPARATOR 
           << LoggingLevel::levelToStr(m_traceLevel);
    }
}

FX_NS_END

