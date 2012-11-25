//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn,ruijieguo@gmail.com
// Created	: 2006/5/20
//
#ifndef _SMALLFLOAT_H
#define _SMALLFLOAT_H

#include "firtex/common/StdHeader.h"

FX_NS_DEF(utility)

class SmallFloat
{
public:
	SmallFloat(void)
	{
	}

	~SmallFloat(void)
	{
	}
protected:
	//float to bits conversion utilities...
	union ifvalue 
	{
		int32_t   i;
		float	f; 
	};
public:
	static int32_t floatToIntBits(float value)
	{
		ifvalue u;
		int32_t e, f;
		u.f = value;
		e = u.i & 0x7f800000;
		f = u.i & 0x007fffff;

		if (e == 0x7f800000 && f != 0)
			u.i = 0x7fc00000;

		return u.i;
	}

	static float intBitsToFloat(int32_t bits)
	{
		ifvalue u;
		u.i = bits;
		return u.f;
	}

	/**
	 * Converts a 32 bit float to an 8 bit float.
	 * <br>Values less than zero are all mapped to zero.
	 * <br>Values are truncated (rounded down) to the nearest 8 bit value.
	 * <br>Values between zero and the smallest representable value
	 *  are rounded up.
	 *
	 * @param f the 32 bit float to be converted to an 8 bit float (byte)
	 * @param numMantissaBits the number of mantissa bits to use in the byte, with the remainder to be used in the exponent
	 * @param zeroExp the zero-point in the range of exponent values
	 * @return the 8 bit float representation
	 */
	static byte floatToByte(float f, int numMantissaBits, int zeroExp) 
	{
		// Adjustment from a float zero exponent to our zero exponent,
		// shifted over to our exponent position.
		int fzero = (63-zeroExp)<<numMantissaBits;
		int bits = floatToIntBits(f);
		int smallfloat = bits >> (24-numMantissaBits);
		if (smallfloat < fzero) 
		{
			return (bits<=0) ?
				(byte)0   // negative numbers and zero both map to 0 byte
				:(byte)1;  // underflow is mapped to smallest non-zero number.
		} else if (smallfloat >= fzero + 0x100) 
		{
			return (byte)-1;  // overflow maps to largest number
		}
		else 
		{
			return (byte)(smallfloat - fzero);
		}
	}

	/** Converts an 8 bit float to a 32 bit float. */
	static float byteToFloat(byte b, int numMantissaBits, int zeroExp)
	{				
		if (b == 0) return 0.0f;
		int bits = (b&0xff) << (24-numMantissaBits);
		bits += (63-zeroExp) << 24;
		return intBitsToFloat(bits);
	}


	//
	// Some specializations of the generic functions follow.
	// The generic functions are just as fast with current (1.5)
	// -server JVMs, but still slower with client JVMs.
	//

	/** floatToByte(b, mantissaBits=3, zeroExponent=15)
	 * <br>smallest non-zero value = 5.820766E-10
	 * <br>largest value = 7.5161928E9
	 * <br>epsilon = 0.125
	 */
	static byte floatToByte315(float f) 
	{
		int bits = floatToIntBits(f);
		int smallfloat = bits >> (24-3);
		if (smallfloat < (63-15)<<3) {
			return (bits<=0) ? (byte)0 : (byte)1;
		}
		if (smallfloat >= ((63-15)<<3) + 0x100) 
		{
			return (byte)-1;
		}
		return (byte)(smallfloat - ((63-15)<<3));
	}

	/** byteToFloat(b, mantissaBits=3, zeroExponent=15) */
	static float byte315ToFloat(byte b) 
	{
		// on Java1.5 & 1.6 JVMs, prebuilding a decoding array and doing a lookup
		// is only a little bit faster (anywhere from 0% to 7%)
		if (b == 0) return 0.0f;
		int bits = (b&0xff) << (24-3);
		bits += (63-15) << 24;
		return intBitsToFloat(bits);
	}


	/** floatToByte(b, mantissaBits=5, zeroExponent=2)
	 * <br>smallest nonzero value = 0.033203125
	 * <br>largest value = 1984.0
	 * <br>epsilon = 0.03125
	 */
	static byte floatToByte52(float f)
	{
		int bits = floatToIntBits(f);
		int smallfloat = bits >> (24-5);
		if (smallfloat < (63-2)<<5) {
			return (bits<=0) ? (byte)0 : (byte)1;
		}
		if (smallfloat >= ((63-2)<<5) + 0x100) {
			return (byte)-1;
		}
		return (byte)(smallfloat - ((63-2)<<5));
	}

	/** byteToFloat(b, mantissaBits=5, zeroExponent=2) */
	static float byte52ToFloat(byte b) 
	{
		// on Java1.5 & 1.6 JVMs, prebuilding a decoding array and doing a lookup
		// is only a little bit faster (anywhere from 0% to 7%)
		if (b == 0) return 0.0f;
		int bits = (b&0xff) << (24-5);
		bits += (63-2) << 24;
		return intBitsToFloat(bits);
	}
};

FX_NS_END

#endif
