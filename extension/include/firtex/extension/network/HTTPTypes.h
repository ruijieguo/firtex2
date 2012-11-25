#ifndef __FX_HTTPTYPES_H
#define __FX_HTTPTYPES_H

#include <string>
#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(network);

struct HTTPTypes
{
    virtual ~HTTPTypes() {}

    // Generic strings used by HTTP
    static const std::string STRING_EMPTY;
    static const std::string STRING_CRLF;
    static const std::string STRING_HTTP_VERSION;
    static const std::string HEADER_NAME_VALUE_DELIMITER;

    // Common HTTP header names
    static const std::string HEADER_HOST;
    static const std::string HEADER_COOKIE;
    static const std::string HEADER_SET_COOKIE;
    static const std::string HEADER_CONNECTION;
    static const std::string HEADER_CONTENT_TYPE;
    static const std::string HEADER_CONTENT_LENGTH;
    static const std::string HEADER_CONTENT_LOCATION;
    static const std::string HEADER_CONTENT_ENCODING;
    static const std::string HEADER_LAST_MODIFIED;
    static const std::string HEADER_IF_MODIFIED_SINCE;
    static const std::string HEADER_TRANSFER_ENCODING;
    static const std::string HEADER_LOCATION;
    static const std::string HEADER_AUTHORIZATION;
    static const std::string HEADER_REFERER;
    static const std::string HEADER_USER_AGENT;
    static const std::string HEADER_X_FORWARDED_FOR;
    static const std::string HEADER_CLIENT_IP;

    // Common HTTP content types
    static const std::string CONTENT_TYPE_HTML;
    static const std::string CONTENT_TYPE_TEXT;
    static const std::string CONTENT_TYPE_XML;
    static const std::string CONTENT_TYPE_URLENCODED;
    
    // Common HTTP request methods
    static const std::string REQUEST_METHOD_HEAD;
    static const std::string REQUEST_METHOD_GET;
    static const std::string REQUEST_METHOD_PUT;
    static const std::string REQUEST_METHOD_POST;
    static const std::string REQUEST_METHOD_DELETE;
    
    // Common HTTP response messages
    static const std::string RESPONSE_MESSAGE_OK;
    static const std::string RESPONSE_MESSAGE_CREATED;
    static const std::string RESPONSE_MESSAGE_ACCEPTED;
    static const std::string RESPONSE_MESSAGE_NO_CONTENT;
    static const std::string RESPONSE_MESSAGE_MULTIPLE_CHOICES;
    static const std::string RESPONSE_MESSAGE_MOVED_PERMANENTLY;
    static const std::string RESPONSE_MESSAGE_MOVED_TEMPORARILY;
    static const std::string RESPONSE_MESSAGE_NOT_MODIFIED;
    static const std::string RESPONSE_MESSAGE_BAD_REQUEST;
    static const std::string RESPONSE_MESSAGE_UNAUTHORIZED;
    static const std::string RESPONSE_MESSAGE_FORBIDDEN;
    static const std::string RESPONSE_MESSAGE_NOT_FOUND;
    static const std::string RESPONSE_MESSAGE_INTERNAL_SERVER_ERROR;
    static const std::string RESPONSE_MESSAGE_NOT_IMPLEMENTED;
    static const std::string RESPONSE_MESSAGE_BAD_GATEWAY;
    static const std::string RESPONSE_MESSAGE_SERVICE_UNAVAILABLE;

    // Common HTTP response codes
    enum ResponseCode
    {
        RESPONSE_CODE_OK = 200,
        RESPONSE_CODE_CREATED = 201,
        RESPONSE_CODE_ACCEPTED = 202,
        RESPONSE_CODE_NO_CONTENT = 204,
        RESPONSE_CODE_MULTIPLE_CHOICES = 300,
        RESPONSE_CODE_MOVED_PERMANENTLY = 301,
        RESPONSE_CODE_MOVED_TEMPORARILY = 302,
        RESPONSE_CODE_NOT_MODIFIED = 304,
        RESPONSE_CODE_BAD_REQUEST = 400,
        RESPONSE_CODE_UNAUTHORIZED = 401,
        RESPONSE_CODE_FORBIDDEN = 403,
        RESPONSE_CODE_NOT_FOUND = 404,
        RESPONSE_CODE_INTERNAL_SERVER_ERROR = 500,
        RESPONSE_CODE_NOT_IMPLEMENTED = 501,
        RESPONSE_CODE_BAD_GATEWAY = 502,
        RESPONSE_CODE_SERVICE_UNAVAILABLE = 503
    };

public:
    static const std::string& toMessage(ResponseCode code);
};


///////////////////////////////////////////////////
//
inline const std::string& HTTPTypes::toMessage(ResponseCode code)
{
    switch (code)
    {
    case RESPONSE_CODE_OK:
        return RESPONSE_MESSAGE_OK;
    case RESPONSE_CODE_CREATED:
        return RESPONSE_MESSAGE_CREATED;
    case RESPONSE_CODE_ACCEPTED:
        return RESPONSE_MESSAGE_ACCEPTED;
    case RESPONSE_CODE_NO_CONTENT:
        return RESPONSE_MESSAGE_NO_CONTENT;
    case RESPONSE_CODE_MULTIPLE_CHOICES:
        return RESPONSE_MESSAGE_MULTIPLE_CHOICES;
    case RESPONSE_CODE_MOVED_PERMANENTLY:
        return RESPONSE_MESSAGE_MOVED_PERMANENTLY;
    case RESPONSE_CODE_MOVED_TEMPORARILY:
        return RESPONSE_MESSAGE_MOVED_TEMPORARILY;
    case RESPONSE_CODE_NOT_MODIFIED:
        return RESPONSE_MESSAGE_NOT_MODIFIED;
    case RESPONSE_CODE_BAD_REQUEST:
        return RESPONSE_MESSAGE_BAD_REQUEST;
    case RESPONSE_CODE_UNAUTHORIZED:
        return RESPONSE_MESSAGE_UNAUTHORIZED;
    case RESPONSE_CODE_FORBIDDEN:
        return RESPONSE_MESSAGE_FORBIDDEN;
    case RESPONSE_CODE_NOT_FOUND:
        return RESPONSE_MESSAGE_NOT_FOUND;
    case RESPONSE_CODE_INTERNAL_SERVER_ERROR:
        return RESPONSE_MESSAGE_INTERNAL_SERVER_ERROR;
    case RESPONSE_CODE_NOT_IMPLEMENTED:
        return RESPONSE_MESSAGE_NOT_IMPLEMENTED;
    case RESPONSE_CODE_BAD_GATEWAY:
        return RESPONSE_MESSAGE_BAD_GATEWAY;
    case RESPONSE_CODE_SERVICE_UNAVAILABLE:
        return RESPONSE_MESSAGE_SERVICE_UNAVAILABLE;
    default:
        return RESPONSE_MESSAGE_INTERNAL_SERVER_ERROR;
    }
}


FX_NS_END

#endif //__FX_HTTPTYPES_H
