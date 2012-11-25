//
// Copyright(C) 2005--2010 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2010-08-11 23:36:35

#ifndef __FX_POSTINGCODINGFACTORY_H
#define __FX_POSTINGCODINGFACTORY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/Singleton.h"
#include "firtex/index/PostingCoding.h"

FX_NS_DEF(index);

class PostingCodingFactory : public FX_NS(utility)::Singleton<PostingCodingFactory>
{
public:
    PostingCodingFactory();
    ~PostingCodingFactory();

public:
    const PostingCoding* getDocCoding() const;
    const PostingCoding* getTfCoding() const;
    const PostingCoding* getPosCoding() const;
    const PostingCoding* getSkipListCoding() const;

protected:
    void init();

private:
    PostingCoding* m_pDefaultCoding;
};

FX_NS_END

#endif //__FX_POSTINGCODINGFACTORY_H
