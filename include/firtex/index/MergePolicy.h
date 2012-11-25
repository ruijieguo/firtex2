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
// Created	: 2011-02-17 23:15:28

#ifndef __FX_MERGEPOLICY_H
#define __FX_MERGEPOLICY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/index/MergeProposal.h"
#include "firtex/utility/Singleton.h"

FX_NS_DEF(index);


#define DECLARE_MERGEPOLICY_CREATOR(policyClass, policyName)            \
    class Creator : public MergePolicy::Creator                         \
                  , public FX_NS(utility)::Singleton<policyClass::Creator> \
    {                                                                   \
    protected:                                                          \
        Creator()                                                       \
        {                                                               \
            m_identifier += _T(".");                                    \
            m_identifier += policyName;                                 \
        }                                                               \
        virtual ~Creator(){}                                            \
    public:                                                             \
        virtual MergePolicy* create() const {return new policyClass;}   \
        DECLARE_LAZY_SINGLETON(policyClass::Creator);                   \
    };


class MergePolicy
{
public:
    class Creator
    {
    protected:
        Creator() : m_identifier(_T("merge_policy"))
        {}
        virtual ~Creator(){}

    public:
        virtual MergePolicy* create() const = 0;
        virtual const tstring& getIdentifier() const {return m_identifier;}

    protected:
        tstring m_identifier;
    };

public:
    MergePolicy() {}
    virtual ~MergePolicy() {}

public:
    /**
     * Create merge proposal
     * @param barrelsInfo the barrels need to merge
     * @return merge proposals
     */
    virtual MergeProposalPtr createProposals(
            const BarrelsInfo& barrelsInfo)  = 0;
};

DEFINE_TYPED_PTR(MergePolicy);

FX_NS_END

#endif //__FX_MERGEPOLICY_H
