#include "firtex/search/Sorter.h"
#include "firtex/utility/StringUtils.h"

FX_NS_USE(index);
FX_NS_USE(utility);
FX_NS_USE(queryparser);

FX_NS_DEF(search);

SETUP_STREAM_LOGGER(search, Sorter);

string Sorter::SORT_BY_RELEVANCE = "RELEVANCE";
string Sorter::SORT_BY_INDEXORDER = "INDEXORDER";
string Sorter::SORT_BY_DOCID = "DOCID";
string Sorter::REVERSE_SEPARATOR = ":";
string Sorter::SORT_ASCENDING = "ASC";
string Sorter::SORT_DESCENDING = "DES";

Sorter::Sorter() 
    : m_lastValueIdx(0)
{
}

Sorter::~Sorter() 
{
    for (CompVector::iterator it = m_comparators.begin();
         it != m_comparators.end(); ++it)
    {
        delete *it;
    }
    m_comparators.clear();
    m_evaluators.clear();
}

bool Sorter::init(const IndexReaderPtr& pIndexReader,
                  const SortClausePtr& clause)
{
    m_pParser.reset(new ExprParser(pIndexReader));
    string str;
    SortClause::Iterator it = clause->iterator();
    while (it.hasNext())
    {
        str = it.next();
        bool bReverse = false;
        string::size_type pos = str.find(REVERSE_SEPARATOR);
        if (pos != string::npos)
        {
            if (!strCompareNoCase(str.c_str() + pos + 1, SORT_ASCENDING.c_str()))
            {
                bReverse = true;
            }
            str = str.substr(0, pos);
        }

        if (!strCompareNoCase(str.c_str(), SORT_BY_RELEVANCE.c_str()))
        {
            m_comparators.push_back(new RelevanceComparator(bReverse));
        }
        else if (!strCompareNoCase(str.c_str(), SORT_BY_INDEXORDER.c_str())
                 || !strCompareNoCase(str.c_str(), SORT_BY_DOCID.c_str()))
        {
            m_comparators.push_back(new IndexOrderComparator(bReverse));
        }
        else 
        {
            ExprEvaluatorPtr pEval = m_pParser->parse(str);
            if (pEval)
            {
                m_evaluators.push_back(make_pair(pEval, m_lastValueIdx));
                switch(pEval->getType())
                {
                case VALUE_INT32:
                    m_comparators.push_back(new Int32ExprComparator(bReverse, m_lastValueIdx));
                    m_lastValueIdx += sizeof(int32_t);
                    break;
                case VALUE_INT64:
                    m_comparators.push_back(new Int64ExprComparator(bReverse, m_lastValueIdx));
                    m_lastValueIdx += sizeof(int64_t);
                    break;
                case VALUE_FLOAT:
                    m_comparators.push_back(new FloatExprComparator(bReverse, m_lastValueIdx));
                    m_lastValueIdx += sizeof(float);
                    break;
                case VALUE_DOUBLE:
                    m_comparators.push_back(new DoubleExprComparator(bReverse, m_lastValueIdx));
                    m_lastValueIdx += sizeof(double);
                    break;
                default:
                    FIRTEX_ASSERT2(false);
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
    }
    return (m_comparators.size() > 0);
}

FX_NS_END

