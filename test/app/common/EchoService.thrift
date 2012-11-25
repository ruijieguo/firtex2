/**
 * Thrift files can namespace, package, or prefix their output in various
 * target languages.
 */
namespace cpp firtex

/**
 * Define a search service. Services just need a request string
 */
service EchoService {
   string echo(1:string str),
}
