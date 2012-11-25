//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2011-02-24 22:31:39

#ifndef __FX_ANALYZEDDOCUMENT_H
#define __FX_ANALYZEDDOCUMENT_H

#include <vector>
#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/document/AnalyzedField.h"
#include "firtex/document/TypedDocument.h"

FX_NS_DEF(document);

typedef TypedDocument<AnalyzedField> AnalyzedDocument;

DEFINE_TYPED_PTR(AnalyzedDocument);

FX_NS_END

#endif //__FX_ANALYZEDDOCUMENT_H
