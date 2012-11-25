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
// Created	: 2011-02-17 23:37:21

#ifndef __FX_MERGEPROPOSAL_H
#define __FX_MERGEPROPOSAL_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/BarrelsInfo.h"

FX_NS_DEF(index);

typedef std::vector<BarrelsInfoPtr> MergeProposal;

DEFINE_TYPED_PTR(MergeProposal);

FX_NS_END

#endif //__FX_MERGEPROPOSAL_H
