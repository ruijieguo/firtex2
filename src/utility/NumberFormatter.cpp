#include "firtex/utility/NumberFormatter.h"
#include "firtex/utility/Exception.h"
#include <cstdio>
#include <cctype>

#if defined(_MSC_VER)
#define I64_FMT "I64"
typedef int64_t I64_VAL;
typedef uint64_t UI64_VAL;
#elif defined(__APPLE__) 
#define I64_FMT "q"
typedef long long int I64_VAL;
typedef long long unsigned int UI64_VAL;
#else
#define I64_FMT "ll"
typedef long long int I64_VAL;
typedef long long unsigned int UI64_VAL;
#endif

FX_NS_DEF(utility);

void NumberFormatter::append(tstring& str, int32_t value)
{
    char buffer[64];
    std::sprintf(buffer, "%d", value);
    str.append(buffer);
}

void NumberFormatter::append(tstring& str, int32_t value, int width)
{
    FIRTEX_ASSERT2 (width > 0 && width < 64);

    char buffer[64];
    std::sprintf(buffer, "%*d", width, value);
    str.append(buffer);
}

void NumberFormatter::append0(tstring& str, int32_t value, int width)
{
    FIRTEX_ASSERT2 (width > 0 && width < 64);

    char buffer[64];
    std::sprintf(buffer, "%0*d", width, value);
    str.append(buffer);
}

void NumberFormatter::append(tstring& str, uint32_t value)
{
    char buffer[64];
    std::sprintf(buffer, "%u", value);
    str.append(buffer);
}

void NumberFormatter::append(tstring& str, uint32_t value, int width)
{
    FIRTEX_ASSERT2 (width > 0 && width < 64);

    char buffer[64];
    std::sprintf(buffer, "%*u", width, value);
    str.append(buffer);
}

void NumberFormatter::append0(tstring& str, uint32_t value, int width)
{
    FIRTEX_ASSERT2 (width > 0 && width < 64);

    char buffer[64];
    std::sprintf(buffer, "%0*u", width, value);
    str.append(buffer);
}

void NumberFormatter::append(tstring& str, int64_t value)
{
    char buffer[64];
    std::sprintf(buffer, "%" I64_FMT"d", (I64_VAL)value);
    str.append(buffer);
}

void NumberFormatter::append(tstring& str, int64_t value, int width)
{
    FIRTEX_ASSERT2 (width > 0 && width < 64);

    char buffer[64];
    std::sprintf(buffer, "%*" I64_FMT"d", width, (I64_VAL)value);
    str.append(buffer);
}

void NumberFormatter::append0(tstring& str, int64_t value, int width)
{
    FIRTEX_ASSERT2 (width > 0 && width < 64);

    char buffer[64];
    std::sprintf(buffer, "%0*" I64_FMT"d", width, (I64_VAL)value);
    str.append(buffer);
}

void NumberFormatter::append(tstring& str, uint64_t value)
{
    char buffer[64];
    std::sprintf(buffer, "%" I64_FMT"u", (UI64_VAL)value);
    str.append(buffer);
}

void NumberFormatter::append(tstring& str, uint64_t value, int width)
{
    FIRTEX_ASSERT2 (width > 0 && width < 64);

    char buffer[64];
    std::sprintf(buffer, "%*" I64_FMT"u", width, (UI64_VAL)value);
    str.append(buffer);
}

void NumberFormatter::append0(tstring& str, uint64_t value, int width)
{
    FIRTEX_ASSERT2 (width > 0 && width < 64);

    char buffer[64];
    std::sprintf(buffer, "%0*" I64_FMT"u", width, (UI64_VAL)value);
    str.append(buffer);
}

void NumberFormatter::append(tstring& str, float value)
{
    char buffer[64];
    std::sprintf(buffer, "%.*g", 8, (double) value);
    str.append(buffer);
}

void NumberFormatter::append(tstring& str, double value)
{
    char buffer[64];
    std::sprintf(buffer, "%.*g", 16, value);
    str.append(buffer);
}

void NumberFormatter::append(tstring& str, double value, int precision)
{
    FIRTEX_ASSERT2 (precision >= 0 && precision < 32);

    char buffer[64];
    std::sprintf(buffer, "%.*f", precision, value);
    str.append(buffer);
}

void NumberFormatter::append(tstring& str, double value, int width, int precision)
{
    FIRTEX_ASSERT2 (width > 0 && width < 64 && precision >= 0 && precision < width);

    char buffer[64];
    std::sprintf(buffer, "%*.*f", width, precision, value);
    str.append(buffer);
}

FX_NS_END
