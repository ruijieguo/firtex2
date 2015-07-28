#include "firtex/search/Statement.h"
#include "firtex/utility/StringTokenizer.h"
#include "firtex/utility/StringUtils.h"

using namespace std;

FX_NS_USE(utility);

FX_NS_DEF(search);

SETUP_LOGGER(search, Statement);

const std::string Statement::CLAUSE_SEPARATOR = ";";
const std::string Statement::CLAUSE_EQUAL_SYMBOL = "=";
const std::string Statement::CLAUSE_KV_SEPARATOR = ":";

Statement::Statement() 
{
    registerClause(QueryClause::PREFIX, new QueryClause::Creator);
    registerClause(ParameterClause::PREFIX, new ParameterClause::Creator);
    registerClause(SortClause::PREFIX, new SortClause::Creator);
    registerClause(FilterClause::PREFIX, new FilterClause::Creator);
    registerClause(FieldSelectClause::PREFIX, new FieldSelectClause::Creator);
    registerClause(DocSelectClause::PREFIX, new DocSelectClause::Creator);
    registerClause(ScorerProfileClause::PREFIX, new ScorerProfileClause::Creator);
}

Statement::~Statement() 
{
    for (ClauseCreatorMap::iterator it = m_creators.begin();
         it != m_creators.end(); ++it)
    {
        delete it->second;
    }
    m_creators.clear();
}

bool Statement::fromString(const string& sStat)
{
    if (sStat.empty())
    {
        return false;
    }

    size_t nCursor = 0;
    string sClause;
    while (nCursor < sStat.size())
    {
        bool bSkip = false;
        for (size_t nClauseStart = nCursor; nCursor < sStat.size();)
        {
            if (!bSkip && !sStat.compare(nCursor, 2, "{!"))
            {
                bSkip = true;
                nCursor += 2;
                continue;
            }

            if (bSkip && sStat[nCursor] == '}')
            {
                bSkip = false;
                if (((nCursor + 1)< sStat.size()) && sStat[nCursor + 1] != CLAUSE_SEPARATOR[0])
                {
                    ++nCursor;
                    continue;
                }
            }

            if (!bSkip && (sStat[nCursor] == CLAUSE_SEPARATOR[0] || nCursor == sStat.size() - 1))
            {
                sClause = sStat.substr(nClauseStart, nCursor - nClauseStart 
                        + (nCursor == (sStat.size() - 1)));
                ++nCursor; //skip separator

                size_t eqPos = sClause.find(CLAUSE_EQUAL_SYMBOL);
                if (eqPos == string::npos) 
                {
                    FX_LOG(WARN, "Missing '[%s]' in sub-clasue [%s] of [%s]",  
                            CLAUSE_EQUAL_SYMBOL.c_str(), sClause.c_str(), sStat.c_str());
                    m_invalidClauses.push_back(sClause);
                    continue;
                }
        
                string clauseName = sClause.substr(0, eqPos);
                string clauseValue = sClause.substr(eqPos + CLAUSE_EQUAL_SYMBOL.length());
                while (clauseValue.size() > 0 && *clauseValue.rbegin() == CLAUSE_SEPARATOR[0])
                {
                    clauseValue.resize(clauseValue.size() - 1);
                }

                if (!doAddClause(clauseName, clauseValue))
                {
                    continue;
                }
                break;
            } // end if
            else 
            {
                // forward
                ++nCursor;
            }
        } // end for
    } // end while
    extractBuiltInClause(true);
    return true;
}

void Statement::toString(stringstream& ss) const
{
    for (ClauseMap::const_iterator it = m_clauses.begin();
         it != m_clauses.end(); ++it)
    {
        it->second->toString(ss, CLAUSE_EQUAL_SYMBOL);
        ss << CLAUSE_SEPARATOR;
    }
}

void Statement::setParameter(const string& sKey,
                             const string& sValue)
{
    std::string sClauseValue = sKey + CLAUSE_KV_SEPARATOR + sValue;

    if (!m_pParamClause)
    {
        ClauseCreatorMap::const_iterator it = m_creators.find(ParameterClause::PREFIX);
        if (it == m_creators.end())
        {
            FX_LOG(WARN, "[%s] clause not register", ParameterClause::PREFIX.c_str());
        }
        else
        {
            ClausePtr pClause(it->second->create(ParameterClause::PREFIX, sClauseValue));
            m_clauses.insert(make_pair(ParameterClause::PREFIX, pClause));
            m_pParamClause = std::dynamic_pointer_cast<ParameterClause>(pClause);
        }
    }
    else
    {
        m_pParamClause->setParameter(sClauseValue);
    }
}

void Statement::extractBuiltInClause(bool bOverride)
{
#define FX_ADD_CONDITION(prefix, clause, type, override)                \
    cIt = m_clauses.find(prefix);                                       \
    if (cIt != m_clauses.end())                                         \
    {                                                                   \
        if (!clause || override)                                        \
        {                                                               \
            clause = std::dynamic_pointer_cast<type>(cIt->second);      \
        }                                                               \
        FIRTEX_ASSERT2(clause);                                         \
    }

    ClauseMap::const_iterator cIt;
    FX_ADD_CONDITION(QueryClause::PREFIX, m_pQueryClause, QueryClause, bOverride);
    FX_ADD_CONDITION(ParameterClause::PREFIX, m_pParamClause,
                     ParameterClause, bOverride);
    FX_ADD_CONDITION(SortClause::PREFIX, m_pSortClause, SortClause, bOverride);
    FX_ADD_CONDITION(FilterClause::PREFIX, m_pFilterClause, FilterClause, bOverride);
    FX_ADD_CONDITION(FieldSelectClause::PREFIX, m_pFieldSelectClause,
                     FieldSelectClause, bOverride);
    FX_ADD_CONDITION(DocSelectClause::PREFIX, m_pDocSelectClause,
                     DocSelectClause, bOverride);
    FX_ADD_CONDITION(ScorerProfileClause::PREFIX, m_pScorerProfileClause,
                     ScorerProfileClause, bOverride);
#undef FX_ADD_CONDITION
}

void Statement::registerClause(const string& sClausePrefix,
                               Clause::Creator* pCreator)
{
    ClauseCreatorMap::const_iterator it = m_creators.find(sClausePrefix);
    if (it != m_creators.end())
    {
        FX_LOG(WARN, "Registering a duplicated clause prefix[%s]", 
               sClausePrefix.c_str());
        delete pCreator;
        return;
    }
    m_creators.insert(make_pair(sClausePrefix, pCreator));
}

string Statement::getErrorMessage() const
{
    stringstream ss;
    for (StringVector::const_iterator it = m_invalidClauses.begin();
         it != m_invalidClauses.end(); ++it)
    {
        ss << *it << endl;
    }
    return ss.str();
}

void Statement::removeClause(const string& sClauseName)
{
    ClauseMap::iterator it = m_clauses.find(sClauseName);
    if (it != m_clauses.end())
    {
        m_clauses.erase(it);
    }
}

bool Statement::addClause(const string& sClauseName,
                          const string& sClauseValue)
{
    bool ret = doAddClause(sClauseName, sClauseValue);
    if (!ret)
    {
        return false;
    }
    extractBuiltInClause(false);
    return true;
}

bool Statement::doAddClause(const string& sClauseName,
                            const string& sClauseValue)
{
    ClauseMap::const_iterator cIt = m_clauses.find(sClauseName);
    if (cIt != m_clauses.end())
    {
        FX_LOG(WARN, "Clause [%s] duplicated.", sClauseName.c_str()); 
        return false;
    }

    ClausePtr pClause;
    ClauseCreatorMap::const_iterator it = m_creators.find(sClauseName);
    if (it != m_creators.end())
    {
        pClause.reset(it->second->create(sClauseName, sClauseValue));
    }
    else 
    {
        pClause.reset(new Clause(sClauseName, sClauseValue));
    }
    m_clauses.insert(make_pair(sClauseName, pClause));
    return true;
}

void Statement::clearClauses()
{
    m_clauses.clear();
    m_sOrgStat.clear();
    m_invalidClauses.clear();

    m_pQueryClause.reset();
    m_pParamClause.reset();
    m_pSortClause.reset();
    m_pFilterClause.reset();
    m_pFieldSelectClause.reset();
    m_pDocSelectClause.reset();
    m_pScorerProfileClause.reset();
}

FX_NS_END

