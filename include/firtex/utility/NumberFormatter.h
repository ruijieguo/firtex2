#ifndef __FXNUMBERFORMATTER_H
#define __FXNUMBERFORMATTER_H

#include "firtex/common/StdHeader.h"

FX_NS_DEF(utility);

/**
 * @class 
 * @brief The NumberFormatter class provides static methods
 *  for formatting numeric values into strings.
 */
class NumberFormatter
{
public:
    /** Format an integer value in decimal notation. */
    static void append(std::string& str, int32_t value);

    /**
     * Format an integer value in decimal notation,
     * right justified in a field having at least
     * the specified width.
     */
    static void append(std::string& str, int32_t value, int width);

    /**
     * Format an integer value in decimal notation, 
     * right justified and zero-padded in a field
     * having at least the specified width.
     */
    static void append0(std::string& str, int32_t value, int width);

    /** Format an unsigned int value in decimal notation.*/
    static void append(std::string& str, uint32_t value);

    /**
     * Format an unsigned long int in decimal notation,
     * right justified in a field having at least the
     * specified width.
     */
    static void append(std::string& str, uint32_t value, int width);

    /**
     * Format an unsigned int value in decimal notation, 
     * right justified and zero-padded in a field having at 
     * least the specified width.
     */
    static void append0(std::string& str, uint32_t value, int width);

    /** Format a 64-bit integer value in decimal notation.*/
    static void append(std::string& str, int64_t value);

    /**
     * Format a 64-bit integer value in decimal notation,
     * right justified in a field having at least the specified width.
     */
    static void append(std::string& str, int64_t value, int width);

    /**
     * Format a 64-bit integer value in decimal notation, 
     * right justified and zero-padded in a field having at least 
     * the specified width.
     */
    static void append0(std::string& str, int64_t value, int width);

    /** Format an unsigned 64-bit integer value in decimal notation.*/
    static void append(std::string& str, uint64_t value);

    /**
     * Format an unsigned 64-bit integer value in decimal notation,
     * right justified in a field having at least the specified width.
     */
    static void append(std::string& str, uint64_t value, int width);

    /**
     * Format an unsigned 64-bit integer value in decimal notation, 
     * right justified and zero-padded in a field having at least the 
     * specified width.
     */
    static void append0(std::string& str, uint64_t value, int width);

    /**
     * Format a float value in decimal floating-point notation,
     * according to std::printf's %g format with a precision of 8 fractional digits.
     */
    static void append(std::string& str, float value);

    /**
     * Format a double value in decimal floating-point notation,
     * according to std::printf's %g format with a precision of 16 fractional digits.
     */
    static void append(std::string& str, double value);

    /**
     * Format a double value in decimal floating-point notation,
     * according to std::printf's %f format with the given precision.
     */
    static void append(std::string& str, double value, int precision);

    /**
     * Format a double value in decimal floating-point notation,
     * right justified in a field of the specified width,
     * with the number of fractional digits given in precision.
     */
    static void append(std::string& str, double value, int width, int precision);
};


FX_NS_END

#endif
