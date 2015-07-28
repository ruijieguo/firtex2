#ifndef __FXNUMBERPARSER_H
#define __FXNUMBERPARSER_H

#include "firtex/common/StdHeader.h"
#include <string>
#include <cstdio>
#include <cctype>

FX_NS_DEF(utility);

/**
 * @class NumberParser
 * @brief  The NumberParser class provides static methods 
 *  for parsing numbers out of strings.
 */
class NumberParser
{
public:
    /**
     * Parses an integer value in decimal notation.
     * @throw throw SyntaxException if the string does not hold a number in decimal notation.
     */
    static int32_t parseInt32(const std::string& s);

    /**
     * Parses an integer value in decimal notation.
     * @return return true if a valid integer has been found, false otherwise. 	
     */
    static bool tryParseInt32(const std::string& s, int32_t& value);

    /**
     * Parses an unsigned integer value in decimal notation from the given string.
     * @throw throw SyntaxException if the string does not hold a number in decimal notation.	
     */
    static uint32_t parseUInt32(const std::string& s);

    /**
     * Parses an unsigned integer value in decimal notation from the given string.
     * @return return true if a valid integer has been found, false otherwise. 
     */
    static bool tryParseUInt32(const std::string& s, uint32_t& value);

    /**
     * Parses an integer value in hexadecimal notation from the given string.
     * @throw throw a SyntaxException if the string does not hold a number in
     *  hexadecimal notation.
     */
    static uint32_t parseHexUInt32(const std::string& s);

    /**
     * Parses an unsigned integer value in hexadecimal notation from the given string.
     * @return return true if a valid integer has been found, false otherwise. 
     */
    static bool tryParseHexUInt32(const std::string& s, uint32_t& value);

    /**
     * Parses a 64-bit integer value in decimal notation from the given string.
     * @throw throw a SyntaxException if the string does not hold a number in decimal notation.
     */
    static int64_t parseInt64(const std::string& s);

    /**
     * Parses a 64-bit integer value in decimal notation from the given string.
     * @return return true if a valid integer has been found, false otherwise. 
     */
    static bool tryParseInt64(const std::string& s, int64_t& value);

    /**
     * Parses an unsigned 64-bit integer value in decimal notation from the given string.
     * @throw throw a SyntaxException if the string does not hold a number in decimal notation.
     */
    static uint64_t parseUInt64(const std::string& s);

    /**
     * Parses an unsigned 64-bit integer value in decimal notation from the given string.
     * @return return true if a valid integer has been found, false otherwise. 
     */
    static bool tryParseUInt64(const std::string& s, uint64_t& value);

    /**
     * Parses a 64 bit-integer value in hexadecimal notation from the given string.
     * @throw throw a SyntaxException if the string does not hold a number in hexadecimal notation.
     */
    static uint64_t parseHexUInt64(const std::string& s);

    /**
     * Parses an unsigned 64-bit integer value in hexadecimal notation from the given string.
     * @return return true if a valid integer has been found, false otherwise. 
     */
    static bool tryParseHexUInt64(const std::string& s, uint64_t& value);

    /**
     * Parses a double value in decimal floating point notation from the given string. 
     * @throw throw a SyntaxException if the string does not hold a floating-point 
     * number in decimal notation.
     */
    static double parseDouble(const std::string& s);

    /**
     * Parses a double value in decimal floating point notation
     * from the given string. 
     * @return true if a valid floating point number has been found, 
     * false otherwise. 
     */
    static bool tryParseDouble(const std::string& s, double& value);

    /**
     * Parses a float value in decimal floating point notation from the given string. 
     * @throw throw a SyntaxException if the string does not hold a floating-point 
     * number in decimal notation.
     */
    static float parseFloat(const std::string& s);

    /**
     * Parses a float value in decimal floating point notation
     * from the given string. 
     * @return true if a valid floating point number has been found, 
     * false otherwise. 
     */
    static bool tryParseFloat(const std::string& s, float& value);
};

///////////////////////////////////////////////////////////////////
#if defined(_MSC_VER)
#define I64_FMT "I64"
#elif defined(__APPLE__) 
#define I64_FMT "q"
#else
#define I64_FMT "ll"
#endif

inline bool NumberParser::tryParseInt32(const std::string& s, int32_t& value)
{
    char temp;
    return std::sscanf(s.c_str(), "%d%c", &value, &temp) >= 1;
}

inline bool NumberParser::tryParseUInt32(const std::string& s, uint32_t& value)
{
    char temp;
    return std::sscanf(s.c_str(), "%u%c", &value, &temp) >= 1;
}

inline bool NumberParser::tryParseHexUInt32(const std::string& s, uint32_t& value)
{
    char temp;
    return std::sscanf(s.c_str(), "%x%c", &value, &temp) >= 1;
}

inline bool NumberParser::tryParseInt64(const std::string& s, int64_t& value)
{
    char temp;
    return std::sscanf(s.c_str(), "%" I64_FMT"d%c", (long long unsigned int*)&value, &temp) >= 1;
}

inline bool NumberParser::tryParseUInt64(const std::string& s, uint64_t& value)
{
    char temp;
    return std::sscanf(s.c_str(), "%" I64_FMT"u%c", (long long unsigned int*)&value, &temp) >= 1;
}

inline bool NumberParser::tryParseHexUInt64(const std::string& s, uint64_t& value)
{
    char temp;
    return std::sscanf(s.c_str(), "%" I64_FMT"x%c", (long long unsigned int*)&value, &temp) >= 1;
}

inline bool NumberParser::tryParseDouble(const std::string& s, double& value)
{
    char temp;
    return std::sscanf(s.c_str(), "%lf%c", &value, &temp) >= 1;
}

inline bool NumberParser::tryParseFloat(const std::string& s, float& value)
{
    double v2;
    bool ret = tryParseDouble(s, v2);
    value = (float)v2;
    return ret;
}

FX_NS_END

#endif 
