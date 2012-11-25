//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author       : ¹ùÈð½Ü(GuoRuijie)
// Email        : ruijieguo@gmail.com
// Created      : 2005/10/20
//
#ifndef __FX_TYPEDEFS_H
#define __FX_TYPEDEFS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/common/Common.h"
#include "firtex/common/IntTypes.h"
#include <cstddef>

FX_ROOTNS_DEF();

///document's field id 
typedef int32_t	fieldid_t;
const static fieldid_t INVALID_FIELDID = (fieldid_t)(-1);

///term id
typedef int32_t	termid_t;

/// term position
typedef int32_t	loc_t;
const static loc_t INVALID_POS = (loc_t)-1;

/// internal document id encoding.
typedef int32_t	docid_t;
const static docid_t INVALID_DOCID = (docid_t)-1;
#define MAX_DOCID MAX_INT32

/// in-document term frequency
typedef int32_t	tf_t;

/// document frequency
typedef int32_t	df_t;

/// collection term frequency
typedef	int64_t	ctf_t;

/// floating point frequencies and/or scores.
typedef float score_t;
typedef float weight_t;

/// normalization encoding
typedef	int32_t	norm_t;	///norm byte

///field flag
typedef uint32_t fieldflag_t;
////field bits
typedef uint16_t fieldbits_t;
///field type
typedef uint8_t field_t;

///store type
typedef unsigned int store_t;
///index type
typedef unsigned int index_t;
///forward index type
typedef unsigned int fdindex_t;

///term type
typedef uint8_t term_t;

///token hint id type
typedef int64_t hintid_t;
const static int64_t INVALID_HINTID = -1;

///charset
typedef uint32_t charset_t;

typedef int32_t commitid_t;

#define INVALID_COMMIT (commitid_t)-1

typedef int32_t shardid_t;
const static shardid_t INVALID_SHARDID = -1;

/// string 
template<typename CharT>
struct _str 
{
    CharT*      str;
    std::size_t length;
};

typedef _str<char>  str_t;
typedef _str<wchar_t> wstr_t;

FX_ROOTNS_END();

#endif
