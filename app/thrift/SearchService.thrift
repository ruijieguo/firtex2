/**
 * Thrift files can namespace, package, or prefix their output in various
 * target languages.
 */
namespace py firtex
namespace cpp firtex
namespace java firtex
namespace php firtex
namespace perl firtex

struct QueryCommand {
    1: string query,
    2: list<string> sortBy,
    3: string filter,
    4: i32 offset = 0,
    5: i32 limit = 10,
    6: list<string> fieldSelect,
    7: string encode,
    8: list<string> cluster,
    9: string traceLevel
}

/**
 * Define a search service. Services just need a request string
 */
service ThriftSearchService {
   string search(1:QueryCommand cmd),
   string syntaxSearch(1:string syntaxStr),
   bool ping();
}
