//
// Copyright(C) 2005--2010 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2010-07-05 23:28:14

#ifndef __FX_ANYQUERYEXECUTOR_H
#define __FX_ANYQUERYEXECUTOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/search/QueryExecutor.h"
#include "firtex/search/AnyQuery.h"

FX_NS_DEF(search);

class AnyQueryExecutor : public QueryExecutor
{
public:
    AnyQueryExecutor(FX_NS(index)::IndexReaderPtr& pIndexReader, 
                      FeatureProviderPtr& provider,
                      FX_NS(utility)::PoolPtr& pPool);
    ~AnyQueryExecutor();

public:
    /**
     * Move to next matched documents
     * @param scoreDocs matched documents stored here
     * @return number of matched documents
     */   
    size_t advance(MatchedDocSet& matchedDocs);
    
    /** 
     * Skip to document whose id is equal to or greater than <i>docId</i>.
     * @param docId docid to skip to
     * @return an docid whoese value is not less than <i>docId</i>
     */ 
    docid_t skipTo(docid_t docId);

    /**
     * Get the latest matched document meta
     */
    void currentMatchedDoc(MatchedDoc& matchedDoc);

    /**
     * Extract query feature
     */
    void extractFeature(QueryFeature& queryFeature) const;

    /**
     * Number of atomic terms in this query
     */
    size_t size() const { return 1;}

protected:
    FX_NS(index)::IndexReaderPtr m_pIndexReader;

    docid_t m_lastDocId;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__FX_TERMQUERYEXECUTOR_H
