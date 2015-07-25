#include "SearchExecutor.h"
#include "../common/SnippetGenerator.h"
#include "firtex/utility/TimeProbe.h"
#include "firtex/queryparser/QueryParser.h"

using namespace std;
FX_NS_USE(search);
FX_NS_USE(utility);
FX_NS_USE(index);

FX_NS_DEF(app);

SETUP_LOGGER(app, SearchExecutor);

SearchExecutor::SearchExecutor(IndexSearcher& searcher,
                               ResultFormatter& formatter)
    : m_searcher(searcher)
    , m_formatter(formatter)
{
}

SearchExecutor::~SearchExecutor() 
{
}

void SearchExecutor::search(stringstream& ss, const string& sState)
{
    FX_NS(search)::Statement state;
    bool bValid = state.fromString(sState);
    if (!bValid)
    {
        FIRTEX_THROW(SyntaxException, "Syntax Error: [%s]", 
                     state.getErrorMessage().c_str());
    }
    search(ss, state);
}

void SearchExecutor::search(stringstream& ss, const Statement& state)
{
    TimeProbe probe;
    probe.start();

    IndexReaderPtr pIndexReader = m_searcher.getIndexReader();
    QueryParser parser(pIndexReader->getAnalyzerMapper(), "");

    QueryHitsPtr pHits = m_searcher.search(state, parser);

    QueryResult result;

    if (pHits.isNotNull())
    {
        FieldSelectClausePtr pFieldClause = state.getFieldSelectClause();
        QueryClausePtr pQueryClause = state.getQueryClause();
        if (pFieldClause.isNotNull() && pQueryClause.isNotNull())
        {
            QueryPtr pQuery = parser.parse(pQueryClause->getQueryString());
            FIRTEX_ASSERT2(pQuery.isNotNull());

            FieldSelector selector(pIndexReader->getDocSchema());
                
            for (size_t i = 0; i < pFieldClause->getFieldCount(); ++i)
            {
                const FieldSelectClause::SnippetParam& param = pFieldClause->getField(i);
                FieldFilterPtr pFieldFilter;
                if (param.snippet)
                {
                    SnippetGenerator* pSnippetGen = new SnippetGenerator();
                    pFieldFilter.reset(pSnippetGen);
                        
                    if (!pSnippetGen->init(pQuery, parser.getAnalyzerMapper(), param.field,
                                    param.preTag, param.postTag, param.separator))
                    {
                        FIRTEX_THROW_AND_LOG(BadParameterException, 
                                "Init snippet generator for field: [%s] FAILED", 
                                param.field.c_str());
                    }                        
                }

                if (!selector.addField(param.field, pFieldFilter))
                {
                    FX_LOG(ERROR, "Invalid field: [%s]", param.field.c_str());
                }
            }
            result.init(selector, pIndexReader, *pHits);
        }
        else
        {
            result.init(pIndexReader, *pHits);
        }
    }

    probe.stop();
    result.setTimeCost(probe.elapsed() / 1000);
    FX_QUERY_TRACE(INFO, result.getTracer(), "search phase time [%d]",
                   (int32_t)result.getTimeCost());

    m_formatter.format(result, ss);
}

FX_NS_END

