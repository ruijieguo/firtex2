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
// Created : 2011-07-02 22:39:50

#ifndef __FX_LAYOUTAPPENDER_H
#define __FX_LAYOUTAPPENDER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/common/Appender.h"

FX_NS_DEF(common);

class LoggingLayout;
class LayoutAppender : public Appender
{
public:
    LayoutAppender();
    LayoutAppender(const std::string& sLayout);
    ~LayoutAppender();

public:
    /// Set layout
    void setLayout(const std::string& sLayout);
    void setLayout(LoggingLayout* pLayout);
    LoggingLayout& getLayout() const;
    
private:
    LoggingLayout* m_pLayout;
};

DEFINE_TYPED_PTR(LayoutAppender);
////////////////////////////////////////////
///
inline LoggingLayout& LayoutAppender::getLayout() const
{
    return (*m_pLayout);
}

FX_NS_END

#endif //__FX_LAYOUTAPPENDER_H
