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
// Created	: 2007/4/19
//
#ifndef __CHARSET_H
#define __CHARSET_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/StringUtils.h"
#include <string>

FX_NS_DEF(utility)

class Charset
{
public:
	///Charset Types						
	const static charset_t UNKNOWN      = 0;
	const static charset_t ISO_8859_1   = 1;
	const static charset_t UTF_8        = 2;
	const static charset_t EUC_KR       = 3;
	const static charset_t ISO_8859_15  = 4;
	const static charset_t WINDOWS_1252 = 5;
	const static charset_t UTF_16       = 6;
	const static charset_t EUC_JP       = 7;
	const static charset_t KOI8_R       = 8;
	const static charset_t KOI8_U       = 9;
	const static charset_t ISO_8859_2   = 10;
	const static charset_t ISO_8859_3   = 11;
	const static charset_t ISO_8859_4   = 12;
	const static charset_t ISO_8859_5   = 13;
	const static charset_t ISO_8859_6   = 14;
	const static charset_t ISO_8859_6_I = 15;
	const static charset_t ISO_8859_6_E = 16;
	const static charset_t ISO_8859_7   = 17;
	const static charset_t ISO_8859_8   = 18;
	const static charset_t ISO_8859_8_I = 19;
	const static charset_t ISO_8859_8_E = 20;
	const static charset_t ISO_8859_9   = 21;
	const static charset_t ISO_8859_10  = 22;
	const static charset_t ISO_8859_11  = 23;
	const static charset_t ISO_8859_13  = 24;
	const static charset_t ISO_8859_14  = 25;
	const static charset_t ISO_8859_16  = 26;
	const static charset_t ISO_IR_111   = 27;
	const static charset_t ISO_2022_CN  = 29;
	const static charset_t ISO_2022_KR  = 30;
	const static charset_t ISO_2022_JP  = 31;
	const static charset_t US_ASCII     = 32;
	const static charset_t UTF_32BE     = 33;
	const static charset_t UTF_32LE     = 34;
	const static charset_t UTF_16BE     = 35;
	const static charset_t UTF_16LE     = 36;
	const static charset_t WINDOWS_1250 = 37;
	const static charset_t WINDOWS_1251 = 38;
	const static charset_t WINDOWS_1253 = 39;
	const static charset_t WINDOWS_1254 = 40;
	const static charset_t WINDOWS_1255 = 41;
	const static charset_t WINDOWS_1256 = 42;
	const static charset_t WINDOWS_1257 = 43;
	const static charset_t WINDOWS_1258 = 44;
	const static charset_t IBM866       = 45;
	const static charset_t IBM850       = 46;
	const static charset_t IBM852       = 47;
	const static charset_t IBM855       = 48;
	const static charset_t IBM857       = 49;
	const static charset_t IBM862       = 50;
	const static charset_t IBM864       = 51;
	const static charset_t IBM864I      = 52;
	const static charset_t UTF_7        = 53;
	const static charset_t SHIFT_JIS    = 54;
	const static charset_t BIG5         = 55;
	const static charset_t GB2312       = 56;
	const static charset_t GB18030      = 57;
	const static charset_t VISCII       = 58;
	const static charset_t TIS_620      = 59;
	const static charset_t HZ_GB_2312   = 61;
	const static charset_t BIG5_HKSCS   = 62;
	const static charset_t X_GBK        = 63;
	const static charset_t X_EUC_TW     = 64;
	

	static charset_t nameToType(const tchar* strCharset)
	{
		size_t len;
		if (strCharset == NULL) 
		{
			return UNKNOWN;
		}

		len = strLength(strCharset);

		if (len >= 10 && !strCompareNoCase(strCharset, "ISO-8859-1", 10)) 
		{
			return ISO_8859_1;
		}
		else if (len >= 5 && !strCompareNoCase(strCharset, "UTF-8", 5)) 
		{
			return UTF_8;
		}
		else if (len >= 6 && (!strCompareNoCase(strCharset, "EUC-KR", 6) || !strCompareNoCase(strCharset, "EUC_KR", 6))) 
		{
			return EUC_KR;
		}
		else if (len >= 5 && !strCompareNoCase(strCharset, "EUCKR", 5)) 
		{
			return EUC_KR;
		}
		else if (len >= 11 && !strCompareNoCase(strCharset, "ISO-8859-15", 11)) 
		{
			return ISO_8859_15;
		}
		else if (len >= 12 && !strCompareNoCase(strCharset, "windows-1252", 12)) 
		{
			return WINDOWS_1252;
		}
		else if (len >= 6 && !strCompareNoCase(strCharset, "UTF-16", 6)) 
		{
			return UTF_16;
		}
		else if (len >= 6 && (!strCompareNoCase(strCharset, "EUC_JP", 6) || !strCompareNoCase(strCharset, "EUC-JP", 6))) 
		{
			return EUC_JP;
		}
		else if (len >= 5 && !strCompareNoCase(strCharset, "EUCJP", 5)) 
		{
			return EUC_JP;
		}
		else if (len >= 6 && !strCompareNoCase(strCharset, "KOI8-R", 6)) 
		{
			return KOI8_R;
		}
		else if (len >= 6 && !strCompareNoCase(strCharset, "KOI8-U", 6)) 
		{
			return KOI8_U;
		}
		else if (len >= 10 && !strCompareNoCase(strCharset, "ISO-8859-2", 10)) 
		{
			return ISO_8859_2;
		}
		else if (len >= 10 && !strCompareNoCase(strCharset, "ISO-8859-3", 10)) 
		{
			return ISO_8859_3;
		}
		else if (len >= 10 && !strCompareNoCase(strCharset, "ISO-8859-4", 10)) 
		{
			return ISO_8859_4;
		}
		else if (len >= 10 && !strCompareNoCase(strCharset, "ISO-8859-5", 10)) 
		{
			return ISO_8859_5;
		}
		else if (len >= 10 && !strCompareNoCase(strCharset, "ISO-8859-6", 10)) 
		{
			return ISO_8859_6;
		}
		else if (len >= 12 && !strCompareNoCase(strCharset, "ISO-8859-6-I", 12)) 
		{
			return ISO_8859_6_I;
		}
		else if (len >= 12 && !strCompareNoCase(strCharset, "ISO-8859-6-E", 12)) 
		{
			return ISO_8859_6_E;
		}
		else if (len >= 10 && !strCompareNoCase(strCharset, "ISO-8859-7", 10)) 
		{
			return ISO_8859_7;
		}
		else if (len >= 10 && !strCompareNoCase(strCharset, "ISO-8859-8", 10)) 
		{
			return ISO_8859_8;
		}
		else if (len >= 12 && !strCompareNoCase(strCharset, "ISO-8859-8-I", 12)) 
		{
			return ISO_8859_8_I;
		}
		else if (len >= 12 && !strCompareNoCase(strCharset, "ISO-8859-8-E", 12)) 
		{
			return ISO_8859_8_E;
		}
		else if (len >= 10 && !strCompareNoCase(strCharset, "ISO-8859-9", 10))
		{
			return ISO_8859_9;
		}
		else if (len >= 11 && !strCompareNoCase(strCharset, "ISO-8859-10", 11))
		{
			return ISO_8859_10;
		}
		else if (len >= 11 && !strCompareNoCase(strCharset, "ISO-8859-11", 11)) 
		{
			return ISO_8859_11;
		}
		else if (len >= 11 && !strCompareNoCase(strCharset, "ISO-8859-13", 11))
		{
			return ISO_8859_13;
		}
		else if (len >= 11 && !strCompareNoCase(strCharset, "ISO-8859-14", 11))
		{
			return ISO_8859_14;
		}
		else if (len >= 11 && !strCompareNoCase(strCharset, "ISO-8859-16", 11)) 
		{
			return ISO_8859_16;
		}
		else if (len >= 10 && !strCompareNoCase(strCharset, "ISO-IR-111", 10)) 
		{
			return ISO_IR_111;
		}
		else if (len >= 11 && !strCompareNoCase(strCharset, "ISO-2022-CN", 11))
		{
			return ISO_2022_CN;
		}
		else if (len >= 11 && !strCompareNoCase(strCharset, "ISO-2022-CN", 11)) 
		{
			return ISO_2022_CN;
		}
		else if (len >= 11 && !strCompareNoCase(strCharset, "ISO-2022-KR", 11))
		{
			return ISO_2022_KR;
		}
		else if (len >= 11 && !strCompareNoCase(strCharset, "ISO-2022-JP", 11))
		{
			return ISO_2022_JP;
		}
		else if (len >= 8 && !strCompareNoCase(strCharset, "us-ascii", 8)) 
		{
			return US_ASCII;
		}
		else if (len >= 8 && !strCompareNoCase(strCharset, "UTF-32BE", 8)) 
		{
			return UTF_32BE;
		}
		else if (len >= 8 && !strCompareNoCase(strCharset, "UTF-32LE", 8)) 
		{
			return UTF_32LE;
		}
		else if (len >= 8 && !strCompareNoCase(strCharset, "UTF-16BE", 8)) 
		{
			return UTF_16BE;
		}
		else if (len >= 8 && !strCompareNoCase(strCharset, "UTF-16LE", 8)) 
		{
			return UTF_16LE;
		}
		else if (len >= 12 && !strCompareNoCase(strCharset, "windows-1250", 12))
		{
			return WINDOWS_1250;
		}
		else if (len >= 12 && !strCompareNoCase(strCharset, "windows-1251", 12))
		{
			return WINDOWS_1251;
		}
		else if (len >= 12 && !strCompareNoCase(strCharset, "windows-1253", 12)) 
		{
			return WINDOWS_1253;
		}
		else if (len >= 12 && !strCompareNoCase(strCharset, "windows-1254", 12))
		{
			return WINDOWS_1254;
		}
		else if (len >= 12 && !strCompareNoCase(strCharset, "windows-1255", 12))
		{
			return WINDOWS_1255;
		}
		else if (len >= 12 && !strCompareNoCase(strCharset, "windows-1256", 12)) 
		{
			return WINDOWS_1256;
		}
		else if (len >= 12 && !strCompareNoCase(strCharset, "windows-1257", 12))
		{
			return WINDOWS_1257;
		}
		else if (len >= 12 && !strCompareNoCase(strCharset, "windows-1258", 12)) 
		{
			return WINDOWS_1258;
		}
		else if (len >= 6 && !strCompareNoCase(strCharset, "IBM866", 6)) 
		{
			return IBM866;
		}
		else if (len >= 6 && !strCompareNoCase(strCharset, "IBM850", 6)) 
		{
			return IBM850;
		}
		else if (len >= 6 && !strCompareNoCase(strCharset, "IBM852", 6)) 
		{
			return IBM852;
		}
		else if (len >= 6 && !strCompareNoCase(strCharset, "IBM855", 6))
		{
			return IBM855;
		}
		else if (len >= 6 && !strCompareNoCase(strCharset, "IBM857", 6)) 
		{
			return IBM857;
		}
		else if (len >= 6 && !strCompareNoCase(strCharset, "IBM862", 6))
		{
			return IBM862;
		}
		else if (len >= 6 && !strCompareNoCase(strCharset, "IBM864", 6)) 
		{
			return IBM864;
		}
		else if (len >= 7 && !strCompareNoCase(strCharset, "IBM864i", 7)) 
		{
			return IBM864I;
		}
		else if (len >= 5 && !strCompareNoCase(strCharset, "UTF-7", 5)) 
		{
			return UTF_7;
		}
		else if (len >= 9 && !strCompareNoCase(strCharset, "Shift_JIS", 9))
		{
			return SHIFT_JIS;
		}
		else if (len >= 4 && !strCompareNoCase(strCharset, "Big5", 4)) 
		{
			return BIG5;
		}
		else if (len >= 6 && !strCompareNoCase(strCharset, "GB2312", 6)) 
		{
			return GB2312;
		}
		else if (len >= 7 && !strCompareNoCase(strCharset, "gb18030", 7)) 
		{
			return GB18030;
		}
		else if (len >= 6 && !strCompareNoCase(strCharset, "VISCII", 6)) 
		{
			return VISCII;
		}
		else if (len >= 7 && !strCompareNoCase(strCharset, "TIS-620", 7))
		{
			return TIS_620;
		}
		else if (len >= 10 && !strCompareNoCase(strCharset, "HZ-GB-2312", 10))
		{
			return HZ_GB_2312;
		}
		else if (len >= 10 && !strCompareNoCase(strCharset, "Big5-HKSCS", 10)) 
		{
			return BIG5_HKSCS;
		}
		else if (len >= 5 && !strCompareNoCase(strCharset, "x-gbk", 5)) 
		{
			return X_GBK;
		}
		else if (len >= 8 && !strCompareNoCase(strCharset, "x-euc-tw", 8)) 
		{
			return X_EUC_TW;
		}
		else 
		{					
			return UNKNOWN;
		}
	}

	static tstring typeToName(charset_t _charset)
	{
		return ( (_charset==ISO_8859_1) ? _T("ISO-8859-1") :
					(_charset==UTF_8) ? _T("UTF-8") :
					(_charset==EUC_KR) ? _T("EUC-KR") :
					(_charset==ISO_8859_15) ? _T("ISO-8859-15") :
					(_charset==WINDOWS_1252) ? _T("windows-1252") :
					(_charset==UTF_16) ? _T("UTF-16") :
					(_charset==EUC_JP) ? _T("EUC-JP") :
					(_charset==KOI8_R) ? _T("KOI8-R") :
					(_charset==KOI8_U) ? _T("KOI8-U") :
					(_charset==ISO_8859_2) ? _T("ISO-8859-2") :
					(_charset==ISO_8859_3) ? _T("ISO-8859-3") :
					(_charset==ISO_8859_4) ? _T("ISO-8859-4") :
					(_charset==ISO_8859_5) ? _T("ISO-8859-5") :
                    (_charset==ISO_8859_6) ? _T("ISO-8859-6") :
					(_charset==ISO_8859_6_I) ? _T("ISO-8859-6-I") :
					(_charset==ISO_8859_6_E) ? _T("ISO-8859-6-E") :
                    (_charset==ISO_8859_7) ? _T("ISO-8859-7") :
					(_charset==ISO_8859_8) ? _T("ISO-8859-8") :
					(_charset==ISO_8859_8_I) ? _T("ISO-8859-8-I") :
					(_charset==ISO_8859_8_E) ? _T("ISO-8859-8-E") :
					(_charset==ISO_8859_9) ? _T("ISO-8859-9") :
                    (_charset==ISO_8859_10) ? _T("ISO-8859-10") :
					(_charset==ISO_8859_11) ? _T("ISO-8859-11") :
                    (_charset==ISO_8859_13) ? _T("ISO-8859-13") :
					(_charset==ISO_8859_14) ? _T("ISO-8859-14") :
					(_charset==ISO_8859_16) ? _T("ISO-8859-16") :
                    (_charset==ISO_IR_111) ? _T("ISO-IR-111") :
					(_charset==ISO_2022_CN) ? _T("ISO-2022-CN") :
					(_charset==ISO_2022_CN) ? _T("ISO-2022-CN") :
                    (_charset==ISO_2022_KR) ? _T("ISO-2022-KR") :
					(_charset==ISO_2022_JP) ? _T("ISO-2022-JP") :
					(_charset==US_ASCII) ? _T("us-ascii") :
					(_charset==UTF_32BE) ? _T("UTF-32BE") :
                    (_charset==UTF_32LE) ? _T("UTF-32LE") :
					(_charset==UTF_16BE) ? _T("UTF-16BE") :
					(_charset==UTF_16LE) ? _T("UTF-16LE") :
                    (_charset==WINDOWS_1250) ? _T("windows-1250") :
					(_charset==WINDOWS_1251) ? _T("windows-1251") :
					(_charset==WINDOWS_1253) ? _T("windows-1253") :
					(_charset==WINDOWS_1254) ? _T("windows-1254") :
					(_charset==WINDOWS_1255) ? _T("windows-1255") :
                    (_charset==WINDOWS_1256) ? _T("windows-1256") :
					(_charset==WINDOWS_1257) ? _T("windows-1257") :
                    (_charset==WINDOWS_1258) ? _T("windows-1258") :
                    (_charset==IBM866) ? _T("IBM866") :
                    (_charset==IBM850) ? _T("IBM850") :
					(_charset==IBM852) ? _T("IBM852") :
					(_charset==IBM855) ? _T("IBM855") :
					(_charset==IBM857) ? _T("IBM857") :
					(_charset==IBM862) ? _T("IBM862") :
					(_charset==IBM864) ? _T("IBM864") :
					(_charset==IBM864I) ? _T("IBM864i") :
                    (_charset==UTF_7) ? _T("UTF-7") :
					(_charset==SHIFT_JIS) ? _T("Shift_JIS") :
					(_charset==BIG5) ? _T("Big5") :
                    (_charset==GB2312) ? _T("GB2312") :
					(_charset==GB18030) ? _T("gb18030") :
					(_charset==VISCII) ? _T("VISCII") :
					(_charset==TIS_620) ? _T("TIS-620") :
					(_charset==HZ_GB_2312) ? _T("HZ-GB-2312") :
					(_charset==BIG5_HKSCS) ? _T("Big5-HKSCS") :
					(_charset==X_GBK) ? _T("_charset-gbk") :
					(_charset==X_EUC_TW) ? _T("_charset-euc-tw") :
					_T("unknown"));
	}
};

FX_NS_END

#endif
