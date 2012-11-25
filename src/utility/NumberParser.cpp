#include "firtex/utility/NumberParser.h"
#include "firtex/utility/Exception.h"

using namespace std;

FX_NS_DEF(utility);

int32_t NumberParser::parseInt32(const string& s)
{
	int32_t result;
	if (tryParseInt32(s, result))
    {
		return result;
    }
	else
    {
        throw NumberFormatException("Not a valid integer: ", s);
    }
}

uint32_t NumberParser::parseUInt32(const string& s)
{
	uint32_t result;
	if (tryParseUInt32(s, result))
    {
		return result;
    }
	else
    {
        throw NumberFormatException("Not a valid unsigned integer: ", s);
    }
}

uint32_t NumberParser::parseHexUInt32(const string& s)
{
	uint32_t result;
	if (tryParseHexUInt32(s, result))
    {
        return result;
    }
	else
    {
        throw NumberFormatException("Not a valid hexadecimal integer: ", s);
    }
}

int64_t NumberParser::parseInt64(const string& s)
{
	int64_t result;
	if (tryParseInt64(s, result))
    {
		return result;
    }
	else
    {
        throw NumberFormatException("Not a valid integer: ", s);
    }
}

uint64_t NumberParser::parseUInt64(const string& s)
{
	uint64_t result;
	if (tryParseUInt64(s, result))
		return result;
	else
		throw NumberFormatException("Not a valid unsigned integer: ", s);
}

uint64_t NumberParser::parseHexUInt64(const string& s)
{
	uint64_t result;
	if (tryParseHexUInt64(s, result))
    {
		return result;
    }
	else
    {
        throw NumberFormatException("Not a valid hexadecimal integer: ", s);
    }
}

double NumberParser::parseDouble(const string& s)
{
	double result;
	if (tryParseDouble(s, result))
	{
        return result;
    }
	else
    {
        throw NumberFormatException("Not a valid floating-point number: ", s);
    }
}

float NumberParser::parseFloat(const string& s)
{
	float result;
	if (tryParseFloat(s, result))
	{
        return result;
    }
	else
    {
        throw NumberFormatException("Not a valid floating-point number: ", s);
    }
}

FX_NS_END
