#include "firtex/search/QueryTracer.h"

FX_NS_USE(common);
FX_NS_DEF(search);

std::string QueryTracer::PATH_SEPARATOR = "/";

QueryTracer::QueryTracer(const std::string& sPath, LevelType lvl) 
    : LoggerBase("query_tracer", AppenderPtr(new QueryTracerAppender), lvl)
    , m_sPath(sPath)
{
    m_pTraceAppender = std::dynamic_pointer_cast<QueryTracerAppender>(getAppender());
    FIRTEX_ASSERT2(lvl != LoggingLevel::LEVEL_MAX);
}

QueryTracer::~QueryTracer() 
{
}

FX_NS_END

