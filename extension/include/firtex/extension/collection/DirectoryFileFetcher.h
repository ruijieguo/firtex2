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
// Created : 2011-06-19 16:10:53

#ifndef __FX_DIRECTORYFILEFETCHER_H
#define __FX_DIRECTORYFILEFETCHER_H

#include <string>
#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/DirectoryIterator.h"
#include "firtex/extension/collection/FileFetcher.h"
#include "firtex/extension/collection/FileFilter.h"

FX_NS_DEF(collection);

class DirectoryFileFetcher : public FileFetcher
{
public:
    DirectoryFileFetcher();
    ~DirectoryFileFetcher();

public:
    /**
     * Initialize the fetcher
     * 
     * @param sDirectory directory path
     * @param pFileFilter used to filter out the unneeded files
     */
    void init(const std::string& sDirectory, const FileFilterPtr& pFileFilter);

    /**
     * Retrieval one raw document and move to next
     *
     * @param pRawDoc store extracted raw document
     *
     * @return true if there is one document, otherwise return false
     */
    virtual bool fetchNext(FX_NS(processor)::RawDocumentPtr& pRawDoc);

private:
    FX_NS(utility)::DirectoryIteratorPtr m_pDirIterator;
    FileFilterPtr m_pFileFilter;
    
private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_DIRECTORYFILEFETCHER_H
