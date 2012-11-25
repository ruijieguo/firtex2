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
// Created : 2011-06-19 17:44:30

#ifndef __FX_DEFAULTFILEFILTER_H
#define __FX_DEFAULTFILEFILTER_H

#include <vector>
#include <string>
#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/extension/collection/FileFilter.h"

FX_NS_DEF(collection);

class DefaultFileFilter : public FileFilter
{
public:
    /**
     * Constructor
     *
     * @param sBasePath base directory path. 
     */
    DefaultFileFilter(const std::string& sBasePath);
    ~DefaultFileFilter();

public:
    /**
     * Initialize the filter with filter string
     * @param sFilter filter string. 
     *        format: filter=value1, value2, ....;file_extension=ext1,ext2...
     *        Any file whose path contains any value in <i>filter</i> and 
     *        any file whose extension matches the value in <i>file_extension</i> will be filtered.
     * @return true if initialize is success
     */
    bool init(const std::string& sFilter);

    /**
     * Determine one file filtered or not
     *
     * @return true if filtered
     */
    virtual bool isFiltered(const FX_NS(utility)::Path& filePath);

private:
    typedef std::vector<std::string> StringVector;

    StringVector m_filters;
    StringVector m_fileTypes;
    size_t m_nBasePathDepth;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_DEFAULTFILEFILTER_H
