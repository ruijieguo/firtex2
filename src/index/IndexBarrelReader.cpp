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
// Created	: 2005/12/27
//

#include "firtex/index/IndexBarrelReader.h"

FX_NS_USE(store);
FX_NS_USE(document);

FX_NS_DEF(index);

IndexBarrelReader::IndexBarrelReader()
{
}

IndexBarrelReader::IndexBarrelReader(const FileSystemPtr& pFileSys,
                                     const DocumentSchema* pDocSchema,
                                     const ComponentBuilder* pComponentBuilder)
    : m_pFileSys(pFileSys)
    , m_pDocSchema(pDocSchema)
    , m_pComponentBuilder(pComponentBuilder)
{
    m_pInStreamPool = new InputStreamPool(pFileSys);
}

IndexBarrelReader::IndexBarrelReader(const IndexBarrelReader& src)
    : m_pFileSys(src.m_pFileSys)
    , m_pInStreamPool(src.m_pInStreamPool)
    , m_pDocSchema(src.m_pDocSchema)
    , m_pComponentBuilder(src.m_pComponentBuilder)
{
}

IndexBarrelReader::~IndexBarrelReader(void)
{
    m_pInStreamPool.reset();
    m_pFileSys.reset();
}

FX_NS_END

