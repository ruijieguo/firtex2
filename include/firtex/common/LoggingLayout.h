//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2011-07-02 19:25:12

#ifndef __FX_LOGGINGLAYOUT_H
#define __FX_LOGGINGLAYOUT_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/config/Configurable.h"
#include "firtex/common/LoggingEvent.h"
#include <sstream>

#define DECLARE_LAYOUT_CREATOR(LayoutClass, LayoutName)           \
    class Creator : public LoggingLayout::Creator                            \
                  , public FX_NS(utility)::Singleton<LayoutClass::Creator> \
    {                                                                   \
    protected:                                                          \
        Creator()                                                       \
        {                                                               \
            m_identifier += _T(".");                                    \
            m_identifier += LayoutName;                               \
        }                                                               \
        virtual ~Creator(){}                                            \
    public:                                                             \
        virtual LoggingLayout* create() const {return new LayoutClass;}    \
        DECLARE_LAZY_SINGLETON(LayoutClass::Creator);                 \
    };                                                                  \
public:                                                                 \
std::string getIdentifier() const {return LayoutClass::Creator::instance()->getIdentifier();}


FX_NS_DEF(common);

class LoggingLayout : public FX_NS(config)::Configurable
{
public:
    class Creator
    {
    protected:
        Creator() : m_identifier(_T("layout")) {}
        virtual ~Creator(){}

    public:
        virtual LoggingLayout* create() const = 0;		
        virtual const std::string& getIdentifier() const {return m_identifier;}

    protected:
        std::string m_identifier;
    };

public:
    virtual ~LoggingLayout() {}

public:
    /**
     * Format the logging data to a string
     * Implement this method to create your own layout format.
     *
     * @param message appendable string 
     * @param event The LoggingEvent.
     */
    virtual void format(std::ostringstream& message, const LoggingEvent& event) = 0;

};

FX_NS_END

#endif //__FX_LOGGINGLAYOUT_H
