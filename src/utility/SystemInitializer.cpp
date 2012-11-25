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
// Email	: ruijieguo@software.ict.ac.cn
// Created	: 2007/6/09
//

#include "firtex/search/Similarity.h"
#include "firtex/analyzer/AnalyzerFactory.h"
#include "firtex/utility/SystemInitializer.h"
#include <map>

using namespace std;
FX_NS_USE(thread);
FX_NS_USE(search);

FX_NS_DEF(utility)

SystemInitializer::SystemInitializer(void)
: m_bInit(false)
{
    initialize();  
}

SystemInitializer::~SystemInitializer(void)
{
    terminate();
  
    for (InitVect::iterator it = m_initializers.begin();
         it != m_initializers.end(); ++it)
    {
        delete (*it);
    }
    m_initializers.clear();
}

void SystemInitializer::initialize()
{	
    for (InitVect::const_iterator it = m_initializers.begin();
         it != m_initializers.end(); ++it)
    {
        (*it)->initialize();
    }
  
    //Similarity::initNormTable();
    m_bInit = true;	
}

void SystemInitializer::terminate()
{
    // if(Similarity::getDefault())
    //     delete Similarity::getDefault();

    for (InitVect::const_iterator it = m_initializers.begin();
         it != m_initializers.end(); ++it)
    {
        (*it)->terminate();
    }
}

void SystemInitializer::addInitializer(Initializer* pInitializer)
{
    FastMutex::Guard g(m_lock);

    m_initializers.push_back(pInitializer);
    if (m_bInit)
    {
        pInitializer->initialize();
    }
}

FX_NS_END

