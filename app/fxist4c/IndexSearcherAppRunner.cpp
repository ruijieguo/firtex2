#include <ostream>
#include <istream>
#include <sstream>
#include "IndexSearcherAppRunner.h"
#include "../common/SnippetGenerator.h"
#include "firtex/extension/FirteX-Ext.h"
#include "firtex/utility/TimeProbe.h"
#include "firtex/search/IndexSearcher.h"
#include "firtex/search/XMLResultFormatter.h"
#include "firtex/queryparser/QueryParser.h"
#include "firtex/extension/util/Application.h"
#include "firtex/extension/util/SharedOptionCallback.h"
#include "firtex/contrib/highlighter/QueryScorer.h"

using namespace std;

FX_ROOTNS_USE();
FX_NS_USE(util);
FX_NS_USE(index);
FX_NS_USE(utility);
FX_NS_USE(analyzer);
FX_NS_USE(highlight);
FX_NS_USE(document);
FX_NS_USE(search);
FX_NS_USE(queryparser);
FX_NS_USE(collection);

FX_NS_DEF(app);

SETUP_STREAM_LOGGER(app, IndexSearcherAppRunner);

IndexSearcherAppRunner::IndexSearcherAppRunner()
{
}

AppRunner::Status IndexSearcherAppRunner::run()
{
    if (m_sIndexPath.empty() || (m_sQueryExpr.empty() && m_sQueryFile.empty()))
        return S_NONE;
    if (!m_sQueryExpr.empty())
        search();
    return S_EXIT;
}
 
void IndexSearcherAppRunner::registerOption(Application* pApp)
{
    pApp->addDescription("Search over index : ");
    pApp->addDescription(tstring("  ") + pApp->getProgName() + 
                         " -s [INDEX_PATH] -e [QUERY_EXPR] [-d [DEF_FIELD]] [-l [LEXICON_DIR]]"
                         "search from query expression.");
    pApp->addDescription(tstring("  ") + pApp->getProgName() + 
                         " -s [INDEX_PATH] -q [QUERY_FILE] [-d [DEF_FIELD]] [-l [LEXICON_DIR]]"
                         "search from query file");

    pApp->addOption(Option("search-index", "s", "search over index database")
                    .required(true)
                    .repeatable(false)
                    .argument("[INDEX_PATH]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option("query-file", "q", "query file, one query per line")
                    .required(true)
                    .repeatable(false)
                    .argument("[QUERY_FILE]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option("query-expr", "e", "query expression")
                    .required(true)
                    .repeatable(false)
                    .argument("[QUERY_EXPR]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option("def-field", "d", "default field")
                    .required(false)
                    .repeatable(false)
                    .argument("[DEF_FILED]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback)));

    pApp->addOption(Option("lexicon-dir", "l", "lexicon directory")
                    .required(false)
                    .repeatable(false)
                    .argument("[LEXICON_DIR]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback)));
}

void IndexSearcherAppRunner::optionCallback(const Option& option, const tstring& sValue)
{
    if (option.matchFull("search-index") || option.matchShort("s"))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--search-index/-s");
        }
        m_sIndexPath = sValue;
    }
    else if (option.matchFull("query-file") || option.matchShort("q"))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--query-file/-q");
        }
        m_sQueryFile = sValue;
    }
    else if (option.matchFull("query-expr") || option.matchShort("e"))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--query-expr/-e");
        }
        m_sQueryExpr = sValue;
    }
    else if (option.matchFull("def-field") || option.matchShort("d"))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--def-field/-d");
        }
        m_sDefField = sValue;
    }
    else if (option.matchFull("lexicon-dir") || option.matchShort("l"))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--def-field/-d");
        }
        m_sLexiDir = sValue;
    }
}

void IndexSearcherAppRunner::search()
{
    try
    {
        if (!m_sLexiDir.empty())
        {
            GLOBAL_CONF().General.dictionaryPath = m_sLexiDir;
        }

        Index index;
        index.open(m_sIndexPath, Index::READ, NULL);
        IndexSearcher searcher(index.acquireReader());
        QueryParser queryParser(index.getAnalyzerMapper(), m_sDefField);

        FX_LOG(INFO, "Loading index: [%s]", m_sIndexPath.c_str());
        FX_LOG(INFO, "Index load success.");
        FX_LOG(INFO, "Searching: [%s]", m_sQueryExpr.c_str());
        FX_LOG_FLUSH_ALL();

        TimeProbe probe;
        probe.start();

        Statement stat;
        if (!stat.fromString(m_sQueryExpr))
        {
            cout << "[ERROR] Parse search statements: [" << m_sQueryExpr 
                 << "] FAILED." << endl;
            return ;
        }
        QueryHitsPtr pHits(searcher.search(stat, queryParser));

        probe.stop();

        stringstream ss;
        if (pHits.isNotNull())
        {
            QueryResult result;

            FieldSelectClausePtr pFieldClause = stat.getFieldSelectClause();
            if (pFieldClause.isNotNull())
            {
                FieldSelector selector(index.getDocSchema());
                
                for (size_t i = 0; i < pFieldClause->getFieldCount(); ++i)
                {
                    const FieldSelectClause::SnippetParam& param =
                        pFieldClause->getField(i);
                    FieldFilterPtr pFieldFilter;
                    if (param.snippet)
                    {
                        SnippetGenerator* pSnippetGen = new SnippetGenerator();
                        pFieldFilter.reset(pSnippetGen);
                        
                        QueryPtr pQuery = queryParser.parse(m_sQueryExpr);
                        FIRTEX_ASSERT2(pQuery.isNotNull());

                        if (!pSnippetGen->init(pQuery, queryParser.getAnalyzerMapper(),
                                        param.field, param.preTag, param.postTag, param.separator ))
                        {
                            cout << "Init snippet generator for field: [" << param.field.c_str() << "]";
                            return;
                        }                        
                    }

                    if (!selector.addField(param.field, pFieldFilter))
                    {
                        cout << "Invalid field: " << param.field << endl;
                    }
                }
                
                result.init(selector, index.acquireReader(), *pHits);
            }
            else
            {
                result.init(index.acquireReader(), *pHits);
            }
            result.setTimeCost(probe.elapsed() / 1000);

            XMLResultFormatter formatter;
            formatter.format(result, ss);
        }
        else 
        {
            formatErrorMsg(ss, "No result");
        }
        cout << ss.str() << endl;
    }
    catch(const FirteXException& fe)
    {
        cout << "Search FAILED: " << fe.what() << endl;
    }
}

void IndexSearcherAppRunner::formatErrorMsg(
        stringstream& ss, const string& sErrorMsg)
{
    QueryResult result;
    result.setError(sErrorMsg);
    XMLResultFormatter formatter;
    formatter.format(result, ss);
}

FX_NS_END
