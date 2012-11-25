#ifndef __FX_URLDECODER_H
#define __FX_URLDECODER_H

#include "firtex/common/StdHeader.h"
#include <string>

FX_NS_DEF(utility);

class URLDecoder
{
public:
    inline static std::string decode(const std::string& str);
    inline static std::string encode(const std::string& str);

    static void decode(std::string& result, const std::string& str);
    static void encode(std::string& result, const std::string& str);
};

/////////////////////////////////////////////////////
///
inline std::string URLDecoder::decode(const std::string& str)
{
    std::string result;
    decode(result, str);
    return result;
}

inline std::string URLDecoder::encode(const std::string& str)
{
    std::string result;
    encode(result, str);
    return result;
}

FX_NS_END

#endif //__FX_URLDECODER_H
