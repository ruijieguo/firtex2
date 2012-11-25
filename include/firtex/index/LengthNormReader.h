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
// Created	: 2006/1/4
//
#ifndef __FX_LENGTHNORMREADER_H
#define __FX_LENGTHNORMREADER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/store/InputStream.h"
#include "firtex/store/FileSystem.h"
#include "firtex/document/Field.h"
#include "firtex/document/DocumentSchema.h"
#include "firtex/index/LengthNormIterator.h"
#include <sstream>
#include <vector>

FX_NS_DEF(index);

class LengthNormReader
{
public:
    typedef PrimitiveTypeForwardIndexReaderMMapImpl<int32_t> LengthNorm;
    typedef FX_NS(common)::SharedPtr<LengthNorm> LengthNormPtr;

    typedef std::vector<LengthNormPtr> NormVector;
    typedef std::vector<LengthNormIteratorPtr> NormItVector;

public:
    LengthNormReader();
    LengthNormReader(const FX_NS(store)::FileSystemPtr& pFileSys);
    virtual ~LengthNormReader(void);

public:
    /// Load and initialize length norm from file system
    void init(const std::string& sSuffix,
              const FX_NS(document)::DocumentSchema* pDocSchema);

    /// Add length norm iterator
    void addNormIterator(fieldid_t fieldId, const LengthNormIteratorPtr& pNormIt);

public:
    /**
     * Return length norm iterator by specified field
     */
    LengthNormIteratorPtr lengthNorm(const std::string& sField) const;

    /**
     * Return length norm iterator by specified fieldId
     */
    LengthNormIteratorPtr lengthNorm(fieldid_t fieldId) const;

    /**
     * Return length norm by field
     */
    const LengthNorm* getLengthNorm(const std::string& sField) const;

protected:
    FX_NS(store)::FileSystemPtr m_pFileSys;
    const FX_NS(document)::DocumentSchema* m_pDocSchema;

    NormVector m_lengthNorms;
    NormItVector m_lengthNormIts;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(LengthNormReader);

FX_NS_END


#endif
