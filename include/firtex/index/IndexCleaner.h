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
// Created : 2011-07-10 23:01:26

#ifndef __FX_INDEXCLEANER_H
#define __FX_INDEXCLEANER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/FileSystem.h"
#include <string>

FX_NS_DEF(index);

#define DECLARE_INDEXCLEANER_CREATOR(cleanerClass, policyName)          \
    class Creator : public IndexCleaner::Creator                        \
    {                                                                   \
    public:                                                             \
        Creator()                                                       \
        {                                                               \
            m_identifier += ".";                                        \
            m_identifier += policyName;                                 \
        }                                                               \
        virtual ~Creator(){}                                            \
    public:                                                             \
        virtual IndexCleaner* create() const {return new cleanerClass;} \
    };


class IndexCleaner
{
public:
    class Creator
    {
    public:
        Creator() : m_identifier("cleaner")
        {}
        virtual ~Creator(){}

    public:
        virtual IndexCleaner* create() const = 0;
        virtual const std::string& getIdentifier() const {return m_identifier;}

    protected:
        std::string m_identifier;
    };
    DEFINE_TYPED_PTR(Creator);

public:
    virtual ~IndexCleaner() {}

public:
    /**
     * Initialize parameter 
     *
     * @param pFileSys file system instance
     * @param sParam parameter for cleaner
     */
    virtual void init(const FX_NS(store)::FileSystemPtr& pFileSys,
                      const std::string& sParam) = 0;

    /**
     * Clean up stale index data.
     * Implemented by derived class
     */
    virtual void cleanup() = 0;
};

DEFINE_TYPED_PTR(IndexCleaner);

FX_NS_END

#endif //__FX_INDEXCLEANER_H
