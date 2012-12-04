#include "firtex/queryparser/QueryExprParser.h"
#include "firtex/queryparser/QueryExprLexer.h"
#include "QueryExprBisonParser.h"

using namespace std;
using namespace fx_bison;

FX_NS_DEF(queryparser);

SETUP_STREAM_LOGGER(queryparser, QueryExprParser);

QueryExprParser::QueryExprParser(const std::string& sDefField) 
    : m_sDefaultField(sDefField)
    , m_bTraceScanning(false)
    , m_bTraceParsing(false)
{
}

QueryExprParser::~QueryExprParser() 
{
}

QueryExprPtr QueryExprParser::parse(std::istream& input,
                                    const std::string& sName)
{
    m_sStreamName = sName;
    ostringstream oss;
    QueryExprLexer lexer(&input, &oss);
    lexer.set_debug(m_bTraceScanning);

    QueryExprBisonParser parser(lexer, *this);
    parser.set_debug_level(m_bTraceParsing);
    if (parser.parse() != 0)
    {
        return QueryExprPtr();
    }
    return m_pRootExpr;
}

QueryExprPtr QueryExprParser::parse(const std::string& input,
                                    const std::string& sName)
{
    istringstream iss(input);
    return parse(iss, sName);
}

void QueryExprParser::fail(const class location& l, const std::string& m)
{
    std::stringstream ss;
    ss << l << ": " << m;
    m_sErrorMsg = ss.str();
}

FX_NS_END

