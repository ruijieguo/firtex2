#include "firtex/utility/Number.h"
#include <cstdio>

#if defined(_MSC_VER)
	#define I64_FMT "I64"
#elif defined(__APPLE__) 
	#define I64_FMT "q"
#else
#if __WORDSIZE == 64
	#define I64_FMT "l"
#else
	#define I64_FMT "ll"
#endif
#endif

FX_NS_DEF(utility);

#define FX_IF_TOSTRING(tp, str, fmt)                     \
    if (type == typeid(tp))                              \
    {                                                    \
        tchar buffer[64];                                \
        std::sprintf(buffer, fmt, (tp)(*this));          \
        str.append(buffer);                              \
    }

#define FX_IF_TOSTRING2(tp, str, fmt, width)             \
    if (type == typeid(tp))                              \
    {                                                    \
        tchar buffer[64];                                \
        std::sprintf(buffer, fmt, width, (tp)(*this));   \
        str.append(buffer);                              \
    }

#define FX_IF_TOSTRING3(tp, str, fmt, width, precision)             \
    if (type == typeid(tp))                                         \
    {                                                               \
        tchar buffer[64];                                           \
        std::sprintf(buffer, fmt, width, precision, (tp)(*this));   \
        str.append(buffer);                                         \
    }

tstring Number::toString()
{			
    tstring str;
    const std::type_info& type = m_anyValue.getType();
    FX_IF_TOSTRING(int8_t, str, "%d")
    else
    FX_IF_TOSTRING(uint8_t, str, "%u")
    else
    FX_IF_TOSTRING(int16_t, str, "%d")
    else
    FX_IF_TOSTRING(uint16_t, str, "%u")
    else
    FX_IF_TOSTRING(int32_t, str, "%d")
    else
    FX_IF_TOSTRING(uint32_t, str, "%u")
    else
    FX_IF_TOSTRING(int64_t, str, "%" I64_FMT"d")
    else
    FX_IF_TOSTRING(uint64_t, str, "%" I64_FMT"u")
    else
    FX_IF_TOSTRING2(float, str, "%.*g", 8)
    else
    FX_IF_TOSTRING2(double, str, "%.*g", 16)
    return str;
}	

tstring Number::toString(int nWidth)
{
    tstring str;
    const std::type_info& type = m_anyValue.getType();
    FX_IF_TOSTRING2(int8_t, str, "%*d", nWidth)
    else
    FX_IF_TOSTRING2(uint8_t, str, "%*u", nWidth)
    else
    FX_IF_TOSTRING2(int16_t, str, "%*d", nWidth)
    else
    FX_IF_TOSTRING2(uint16_t, str, "%*u", nWidth)
    else
    FX_IF_TOSTRING2(int32_t, str, "%*d", nWidth)
    else
    FX_IF_TOSTRING2(uint32_t, str, "%*u", nWidth)
    else
    FX_IF_TOSTRING2(int64_t, str, "%*" I64_FMT"d", nWidth)
    else
    FX_IF_TOSTRING2(uint64_t, str, "%*" I64_FMT"u", nWidth)
    else
    FX_IF_TOSTRING2(float, str, "%.*f", nWidth)
    else
    FX_IF_TOSTRING2(double, str, "%.*f", nWidth)
    return str;
}

tstring Number::toString(int nWidth, int precision)
{
    const std::type_info& type = m_anyValue.getType();
    tstring str;
    FX_IF_TOSTRING2(int8_t, str, "%*d", nWidth)
    else
    FX_IF_TOSTRING2(uint8_t, str, "%*u", nWidth)
    else
    FX_IF_TOSTRING2(int16_t, str, "%*d", nWidth)
    else
    FX_IF_TOSTRING2(uint16_t, str, "%*u", nWidth)
    else
    FX_IF_TOSTRING2(int32_t, str, "%*d", nWidth)
    else
    FX_IF_TOSTRING2(uint32_t, str, "%*u", nWidth)
    else
    FX_IF_TOSTRING2(int64_t, str, "%*" I64_FMT"d", nWidth)
    else
    FX_IF_TOSTRING2(uint64_t, str, "%*" I64_FMT"u", nWidth)
    else
    FX_IF_TOSTRING3(float, str, "%*.*f", nWidth, precision)
    else
    FX_IF_TOSTRING3(double, str, "%*.*f", nWidth, precision)
    return str;
}

FX_NS_END

