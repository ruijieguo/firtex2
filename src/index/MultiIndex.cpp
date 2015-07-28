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
// Email	: ruijieguo@software.ict.ac.cn ruijieguo@gmail.com
// Created	: 2007/10/02
//

#include "firtex/index/MultiIndex.h"
#include "firtex/index/MultiIndexReader.h"

using namespace std;
FX_NS_USE(document);
FX_NS_USE(analyzer);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, MultiIndex);

MultiIndex::MultiIndex(void)
{
    m_accessMode = Index::READ;
}

MultiIndex::~MultiIndex(void)
{
    removeAllIndice(true);
}

void MultiIndex::open(const IndexVector& indices)
{
    for (IndexVector::const_iterator it = indices.begin(); it != indices.end(); ++it)
    {
        addIndex(*it); 
    }
    m_pReader.reset(new MultiIndexReader(this));
}

void MultiIndex::open(const vector<tstring>& indexPaths)
{
    for (vector<string>::const_iterator it = indexPaths.begin(); 
         it != indexPaths.end(); ++it)
    {
        addIndex(*it);
    }
    m_pReader.reset(new MultiIndexReader(this));
}

void MultiIndex::addIndex(Index* pIndex)
{
    for (IndexVector::iterator iter = m_indices.begin();
         iter != m_indices.end();iter++)
    {
        if(pIndex == *iter)
            return;
    }
	
    BarrelsInfoPtr pBarrelsInfo(new BarrelsInfo());
    FileSystemPtr pFileSys = pIndex->getFileSystem();
    pBarrelsInfo->read(pFileSys);//read barrels Info

    addBarrels(pBarrelsInfo);
    addSchema(pIndex->getDocSchema());
    m_indices.push_back(pIndex);
    initAnalyzerMapper();
}

void MultiIndex::addIndex(const tstring& sIndexPath)
{
    Index* pIndex = new Index();
    pIndex->open(sIndexPath, Index::READ, NULL);
    addIndex(pIndex);
}

void MultiIndex::addBarrels(const BarrelsInfoPtr& pBarrels)
{
    if (!m_pBarrelsInfo)
    {
        m_pBarrelsInfo.reset(new BarrelsInfo());
    }

    BarrelsInfo::Iterator iter = pBarrels->iterator();
    while (iter.hasNext())
    {
        const BarrelInfo& baInfo = iter.next();
        BarrelInfo& newBarrel = m_pBarrelsInfo->newBarrel();
        newBarrel = baInfo;
    }
}

void MultiIndex::addSchema(const DocumentSchema* pSchema)
{
    if (pSchema)
    {
        if (m_pDocSchema)
        {
            m_pDocSchema->mergeSchema(*pSchema);
        }
        else
        {
            m_pDocSchema.reset(new DocumentSchema(*pSchema));
        }
    }
}

IndexReaderPtr MultiIndex::acquireReader()
{
    return m_pReader;
}

IndexWriterPtr MultiIndex::acquireWriter()
{
    return IndexWriterPtr();
}

void MultiIndex::removeAllIndice(bool bDestroy)
{
    if(bDestroy)
    {
        IndexVector::iterator iter = m_indices.begin();
        while (iter != m_indices.end())
        {
            delete (*iter);
            iter++;
        }
    }
    m_indices.clear();
}

FX_NS_END

