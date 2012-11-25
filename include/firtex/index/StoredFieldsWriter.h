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
// Created	: 2005/12/9
//
#ifndef __STOREDFIELDSWRITER_H
#define __STOREDFIELDSWRITER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/document/AnalyzedDocument.h"
#include "firtex/store/FileSystem.h"
#include "firtex/store/OutputStream.h"
#include <string>

FX_NS_DEF(index);

class StoredFieldsReader;
class StoredFieldsWriter
{
public:
    StoredFieldsWriter(const FX_NS(store)::FileSystemPtr& pFileSys,
                       const std::string& sCacheDir);
    ~StoredFieldsWriter(void);

public:

    /**
     * Seal the barrel . After sealing, modification is forbidden
     */
    void seal();

    /**
     * Commit all data of current barrel to filesystem
     * @param sPrefix prefix of file path
     */
    void commitBarrel(const std::string& sPrefix);

    /**
     * add a document
     * @param pDoc document object
     */
    void addDocument(const FX_NS(document)::AnalyzedDocument* pDoc);

    /// Set cache directory
    void setCacheDir(const std::string& sCacheDir);

    /// Return cache directory
    const std::string& getCacheDir() const;

protected:
    /**
     * Create output streams for adding documents
     */
    void createStreams();

protected:
    FX_NS(store)::OutputStreamPtr m_pValueOutStream;
    FX_NS(store)::OutputStreamPtr m_pIdxOutStream;

    FX_NS(store)::FileSystemPtr m_pFileSys;
    std::string m_sCacheDir;

    bool m_bDirty;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(StoredFieldsWriter);

///////////////////////////////////////////////
inline void StoredFieldsWriter::setCacheDir(const std::string& sCacheDir)
{
    m_sCacheDir = sCacheDir;
}

inline const std::string& StoredFieldsWriter::getCacheDir() const
{
    return m_sCacheDir;
}

FX_NS_END

#endif
