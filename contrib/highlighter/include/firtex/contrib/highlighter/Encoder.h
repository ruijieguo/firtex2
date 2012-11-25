//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: original author,written in JAVA, ported by ¹ùÈð½Ü(GuoRuijie)
// Email	: ruijieguo@software.ict.ac.cn,ruijieguo@gmail.com
// Created	: 2006/7/5
//

#ifndef _ENCODER_H
#define _ENCODER_H


/**
 * Encodes original text. The Encoder works with the Formatter to generate the output.
 *
 * @author Nicko Cadell
 */
FX_NS_DEF(highlight);

class Encoder
{
public:
    Encoder(void){}
    virtual ~Encoder(void){}
public:
    virtual void encodeText(const char* szOrgText,
                            size_t length, 
                            std::string& sEncodedText) const = 0;
};

FX_NS_END

#endif
