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
// Created : 2012-01-24 21:42:04

#ifndef __FX_LOADBALLANCEPOLICY_H
#define __FX_LOADBALLANCEPOLICY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/Singleton.h"
#include "SearchReplicaBase.h"
#include <vector>

FX_NS_DEF(app);

#define DECLARE_LOAD_BALLANCE_CREATOR(PolicyClass, PolicyName)          \
    class Creator : public LoadBallancePolicy::Creator                  \
                  , public FX_NS(utility)::Singleton<PolicyClass::Creator> \
    {                                                                   \
    protected:                                                          \
        Creator()                                                       \
        {                                                               \
            m_identifier += ".";                                        \
            m_identifier += PolicyName;                                 \
        }                                                               \
        virtual ~Creator(){}                                            \
    public:                                                             \
        virtual LoadBallancePolicy* create() const {return new PolicyClass;} \
        DECLARE_LAZY_SINGLETON(PolicyClass::Creator);                   \
    };

class LoadBallancePolicy
{
public:
    class Creator
    {
    protected:
        Creator() : m_identifier("load_ballance")
        {}
        virtual ~Creator(){}

    public:
        /**
         * Create an LoadBallancePolicy object
         * @return create a LoadBallancePolicy
         */
        virtual LoadBallancePolicy* create() const = 0;

        /** 
         * Return the identifier of collection
         */
        virtual const std::string getIdentifier() const {return m_identifier;}

    protected:
        std::string m_identifier;
    };

    typedef std::vector<SearchReplicaBasePtr> ReplicaVector;

public:
    virtual ~LoadBallancePolicy() {}

public:
    /**
     * Initialize
     */
    virtual void init(const ReplicaVector& replicas) = 0;

    /**
     * Select one replica
     * @return selected replica
     */
    virtual SearchReplicaBasePtr select() = 0;

    /// Mark one replica available
    virtual void markAvailable(const SearchReplicaBasePtr& pReplica) = 0;

    /// Mark one replica unavailable
    virtual void markUnavailable(const SearchReplicaBasePtr& pReplica) = 0;
};

DEFINE_TYPED_PTR(LoadBallancePolicy);

FX_NS_END

#endif //__FX_LOADBALLANCEPOLICY_H
