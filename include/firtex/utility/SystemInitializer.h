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

#ifndef __SYSTEMINITIALIZER_H
#define __SYSTEMINITIALIZER_H

#include "firtex/utility/Singleton.h"
#include "firtex/thread/FastMutex.h"
#include <vector>

FX_NS_DEF(utility)

class Initializer
{
public:
    Initializer(){}
    virtual ~Initializer(){}
public:
	/**
	 * initialize global variables
	 */
    virtual void initialize() = 0;

	/**
	 * clean global resources
	 */
	virtual	void terminate() = 0;
};


class SystemInitializer : public Initializer, public Singleton<SystemInitializer>
{
public:
	SystemInitializer(void);
	virtual ~SystemInitializer(void);
public:
	/**
	 * initialize global variables
	 */
    void initialize();

	/**
	 * clean global resources
	 */
    void terminate();

public:
	/**
	 * add a initializer
	 * @param pInitializer starterinitializer object
	 */
	void addInitializer(Initializer* pInitializer);

protected:
    typedef std::vector<Initializer*> InitVect;
    
    FX_NS(thread)::FastMutex m_lock;    
	InitVect m_initializers;    
	bool m_bInit;
};

FX_NS_END

#endif
