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
// Created	: 2006/1/4
// Modified : 2006/10/3 change norms from byte to int32_t,no precision lost.
//


#ifndef __FX_LENGTHNORM_WRITER_H
#define __FX_LENGTHNORM_WRITER_H

#include <sstream>
#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/document/AnalyzedField.h"
#include "firtex/store/OutputStream.h"
#include "firtex/store/FileSystem.h"
#include "firtex/document/DocumentSchema.h"
#include "firtex/index/forwardindex/PrimitiveTypeForwardIndexWriter.h"
#include "firtex/index/LengthNormIterator.h"

FX_NS_DEF(index);

class LengthNormWriter
{
public:
    LengthNormWriter(const FX_NS(store)::FileSystemPtr& pFileSys);
    ~LengthNormWriter();

public:
    /**
     * Init 
     */
    void init(const FX_NS(document)::DocumentSchema* pDocSchema);

    /**
     * Add a field
     */
    void addField(docid_t docId, const FX_NS(document)::AnalyzedField* pField);

    /** 
     * Commit data to file system
     * @param sPrefix prefix of file path
     */
    void commitBarrel(const std::string& sPrefix);

    /**
     * Create an iterator of this forward index.
     *     
     * @param fieldId field id
     * @return forward index iterator. it is efficient if you access
     * value sequentially
     */
    LengthNormIteratorPtr iterator(fieldid_t fieldId) const;

protected:
    typedef PrimitiveTypeForwardIndexWriter<int32_t> LengthNorm;
    DEFINE_TYPED_PTR(LengthNorm);
    typedef std::vector<LengthNormPtr> NormVector;

protected:
    FX_NS(store)::FileSystemPtr m_pFileSys;
    NormVector m_lengthNorm;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(LengthNormWriter);

FX_NS_END

#endif
