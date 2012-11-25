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
// Created : 2012-01-24 21:50:58

#ifndef __FX_ROUNDROBINPOLICY_H
#define __FX_ROUNDROBINPOLICY_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/thread/RWLock.h"
#include "LoadBallancePolicy.h"

FX_NS_DEF(app);

class RoundRobinPolicy : public LoadBallancePolicy
{
public:
    DECLARE_LOAD_BALLANCE_CREATOR(RoundRobinPolicy, "round_robin");

public:
    using LoadBallancePolicy::ReplicaVector;

public:
    RoundRobinPolicy();
    ~RoundRobinPolicy();

public:
    /**
     * Initialize
     */
    virtual void init(const ReplicaVector& replicas);

    /**
     * Select one replica
     * @return selected replica
     */
    virtual SearchReplicaBasePtr select();

    /// Mark one replica available
    virtual void markAvailable(const SearchReplicaBasePtr& pReplica);

    /// Mark one replica unavailable
    virtual void markUnavailable(const SearchReplicaBasePtr& pReplica);

private:
    enum State
    {
        AVAILABLE,
        UNAVAILABLE,
    };

private:
    void markState(const SearchReplicaBasePtr& pReplica, State s);

private:
    typedef std::pair<SearchReplicaBasePtr, State> RepPair;
    typedef std::vector<RepPair> RepPairVector;

    FX_NS(thread)::RWLock m_rwlock;
    RepPairVector m_replicas;
    size_t m_nActive;

private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_ROUNDROBINPOLICY_H
