#include "firtex/extension/network/HTTPTypes.h"
#include "firtex/thread/FastMutex.h"
#include "firtex/utility/NumberFormatter.h"

FX_NS_USE(utility);
FX_NS_USE(thread);

FX_NS_DEF(network);

// generic strings used by HTTP
const std::string HTTPTypes::STRING_EMPTY;
const std::string HTTPTypes::STRING_CRLF("\x0D\x0A");
const std::string HTTPTypes::STRING_HTTP_VERSION("HTTP/");
const std::string HTTPTypes::HEADER_NAME_VALUE_DELIMITER(": ");

// common HTTP header names
const std::string HTTPTypes::HEADER_HOST("Host");
const std::string HTTPTypes::HEADER_COOKIE("Cookie");
const std::string HTTPTypes::HEADER_SET_COOKIE("Set-Cookie");
const std::string HTTPTypes::HEADER_CONNECTION("Connection");
const std::string HTTPTypes::HEADER_CONTENT_TYPE("Content-Type");
const std::string HTTPTypes::HEADER_CONTENT_LENGTH("Content-Length");
const std::string HTTPTypes::HEADER_CONTENT_LOCATION("Content-Location");
const std::string HTTPTypes::HEADER_CONTENT_ENCODING("Content-Encoding");
const std::string HTTPTypes::HEADER_LAST_MODIFIED("Last-Modified");
const std::string HTTPTypes::HEADER_IF_MODIFIED_SINCE("If-Modified-Since");
const std::string HTTPTypes::HEADER_TRANSFER_ENCODING("Transfer-Encoding");
const std::string HTTPTypes::HEADER_LOCATION("Location");
const std::string HTTPTypes::HEADER_AUTHORIZATION("Authorization");
const std::string HTTPTypes::HEADER_REFERER("Referer");
const std::string HTTPTypes::HEADER_USER_AGENT("User-Agent");
const std::string HTTPTypes::HEADER_X_FORWARDED_FOR("X-Forwarded-For");
const std::string HTTPTypes::HEADER_CLIENT_IP("Client-IP");

// common HTTP content types
const std::string HTTPTypes::CONTENT_TYPE_HTML("text/html");
const std::string HTTPTypes::CONTENT_TYPE_TEXT("text/plain");
const std::string HTTPTypes::CONTENT_TYPE_XML("text/xml");
const std::string HTTPTypes::CONTENT_TYPE_URLENCODED("application/x-www-form-urlencoded");

// common HTTP request methods
const std::string HTTPTypes::REQUEST_METHOD_HEAD("HEAD");
const std::string HTTPTypes::REQUEST_METHOD_GET("GET");
const std::string HTTPTypes::REQUEST_METHOD_PUT("PUT");
const std::string HTTPTypes::REQUEST_METHOD_POST("POST");
const std::string HTTPTypes::REQUEST_METHOD_DELETE("DELETE");

// common HTTP response messages
const std::string HTTPTypes::RESPONSE_MESSAGE_OK = "OK";
const std::string HTTPTypes::RESPONSE_MESSAGE_CREATED = "Created";
const std::string HTTPTypes::RESPONSE_MESSAGE_ACCEPTED = "Accepted";
const std::string HTTPTypes::RESPONSE_MESSAGE_NO_CONTENT = "Content";
const std::string HTTPTypes::RESPONSE_MESSAGE_MULTIPLE_CHOICES = "Multiple Choices";
const std::string HTTPTypes::RESPONSE_MESSAGE_MOVED_PERMANENTLY = "Moved Permanently";
const std::string HTTPTypes::RESPONSE_MESSAGE_MOVED_TEMPORARILY = "Moved Temporarily";
const std::string HTTPTypes::RESPONSE_MESSAGE_NOT_MODIFIED = "Not Modified";
const std::string HTTPTypes::RESPONSE_MESSAGE_BAD_REQUEST = "Bad Request";
const std::string HTTPTypes::RESPONSE_MESSAGE_UNAUTHORIZED = "Unauthorized";
const std::string HTTPTypes::RESPONSE_MESSAGE_FORBIDDEN = "Forbidden";
const std::string HTTPTypes::RESPONSE_MESSAGE_NOT_FOUND = "Not Found";
const std::string HTTPTypes::RESPONSE_MESSAGE_INTERNAL_SERVER_ERROR = "Internal Server Error";
const std::string HTTPTypes::RESPONSE_MESSAGE_NOT_IMPLEMENTED = "Not Implemented";
const std::string HTTPTypes::RESPONSE_MESSAGE_BAD_GATEWAY = "Bad Gateway";
const std::string HTTPTypes::RESPONSE_MESSAGE_SERVICE_UNAVAILABLE = "Service Unavailable";

FX_NS_END
