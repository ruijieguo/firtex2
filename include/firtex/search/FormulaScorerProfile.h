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
// Created : 2013-01-01 15:34:30

#ifndef __FX_FORMULASCORERPROFILE_H
#define __FX_FORMULASCORERPROFILE_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/ScorerProfile.h"

FX_NS_DEF(search);

class FormulaScorerProfile : public ScorerProfile
{
public:
    FormulaScorerProfile();
    ~FormulaScorerProfile();

public:
    /**
     * Return the name of the profile name
     */
    virtual std::string getProfileName() const;

    /**
     * Create scorer for specified query
     */
    virtual ScorerPtr createScorer() const;

    /**
     * Set parameters
     * @param params parameters
     */
    virtual void setParameters(const KeyValueMap& params);

private:
    std::string m_sParams;

private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_FORMULASCORERPROFILE_H
