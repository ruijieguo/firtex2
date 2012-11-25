#include "firtex/utility/URLDecoder.h"
#include <cstdlib>
#include <cstdio>

FX_NS_DEF(utility);

//static 
void URLDecoder::decode(std::string& result, const std::string& str)
{
    char decode_buf[3];
    result.reserve(str.size());
    
    for (std::string::size_type pos = 0; pos < str.size(); ++pos) 
    {
        switch(str[pos]) 
        {
        case '+':
            // Convert to space character
            result += ' ';
            break;
        case '%':
            // Decode hexidecimal value
            if (pos + 2 < str.size()) 
            {
                decode_buf[0] = str[++pos];
                decode_buf[1] = str[++pos];
                decode_buf[2] = '\0';
                result += static_cast<char>( strtol(decode_buf, 0, 16) );
            }
            else 
            {
                // Recover from error by not decoding character
                result += '%';
            }
            break;
        default:
            // character does not need to be escaped
            result += str[pos];
        }
    };
}
 
//static
void URLDecoder::encode(std::string& result, const std::string& str)
{
    char encode_buf[4];
    encode_buf[0] = '%';
    result.reserve(str.size());

    // Reference: http://www.blooberry.com/indexdot/html/topics/urlencoding.htm

    for (std::string::size_type pos = 0; pos < str.size(); ++pos) 
    {
        switch(str[pos]) 
        {
        default:
            if (str[pos] > 32 && str[pos] < 127) 
            {
                // character does not need to be escaped
                result += str[pos];
                break;
            }
            // else pass through to next case
        case ' ':
        case '$': case '&': case '+': case ',': case '/': case ':':
        case ';': case '=': case '?': case '@': case '"': case '<':
        case '>': case '#': case '%': case '{': case '}': case '|':
        case '\\': case '^': case '~': case '[': case ']': case '`':
            // the character needs to be encoded
            sprintf(encode_buf+1, "%.2X", (unsigned char)(str[pos]));
        result += encode_buf;
        break;
        }
    };
}


FX_NS_END

