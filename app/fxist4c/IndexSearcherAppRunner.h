#ifndef __FXINDEXSEARCHERAPPRUNNER_H
#define __FXINDEXSEARCHERAPPRUNNER_H

#include "firtex/common/StdHeader.h"
#include "firtex/extension/util/AppRunner.h"
#include "firtex/document/FieldSchema.h"
#include "firtex/contrib/highlighter/Highlighter.h"
#include "firtex/analyzer/AnalyzerMapper.h"
#include "firtex/queryparser/QueryParser.h"
#include "firtex/search/QueryHits.h"
#include "firtex/search/QueryResult.h"

FX_NS_DEF(util);
class Application;
FX_NS_END

FX_NS_DEF(app);

class IndexSearcherAppRunner : public FX_NS(util)::AppRunner
{
public:
    IndexSearcherAppRunner();
    Status run();
    void registerOption(FX_NS(util)::Application* pApp);

private:
    void optionCallback(const FX_NS(util)::Option& option, const tstring& sValue);
    void search();
    void formatErrorMsg(std::stringstream& ss, const std::string& sErrorMsg);

private:
    tstring m_sIndexPath;
    tstring m_sQueryFile;
    tstring m_sQueryExpr;
    tstring m_sDefField;
    tstring m_sLexiDir;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif
