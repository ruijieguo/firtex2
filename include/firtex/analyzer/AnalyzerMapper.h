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
// Email	: ruijieguo@gmail.com
// Created	: 2008/12/28
//
#ifndef __FX_ANALYZERMAPPER_H
#define __FX_ANALYZERMAPPER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/document/DocumentSchema.h"
#include "firtex/analyzer/Analyzer.h"
#include <map>

FX_NS_DEF(analyzer);

class TokenFilter;
class AnalyzerMapper
{
    typedef std::map<tstring, AnalyzerPtr> AnalyzerMap;
    typedef std::map<fieldid_t, AnalyzerPtr> AnalyzerIdMap;

public:
    AnalyzerMapper();
    AnalyzerMapper(const AnalyzerMapper& src);
    virtual ~AnalyzerMapper();		

public:
    /**
     * Initialize
     */
    void init(const FX_NS(document)::DocumentSchema* pDocSchema);

    /**
     * map a field id to a component producer
     * @param pDocSchema document schema	 
     */
    void map(const FX_NS(document)::DocumentSchema* pDocSchema);

    /**
     * map a field id to an analyzer for indexer and searcher
     * @param fid field id
     * @param szFieldName field name
     * @param pAnalyzer analyzer
     */
    void map(fieldid_t fid, const tstring& sFieldName,
             const AnalyzerPtr& pAnalyzer);

    /**
     * map a field id to an analyzer for searcher
     * @param fid field id
     * @param szFieldName field name
     * @param pAnalyzer analyzer
     */
    void mapForSearcher(fieldid_t fid, const tstring& sFieldName,
                        const AnalyzerPtr& pAnalyzer);

public:
    /**
     * Return analyzer by field id
     * @param fid field id
     * @return internal analyzer object
     */
    const Analyzer* getAnalyzer(fieldid_t fid) const;

    /**
     * Return analyzer by producer identifier
     * @param sFieldName field name
     * @return internal analyzer object
     */
    const Analyzer* getAnalyzer(const tstring& sFieldName) const;

    /**
     * Return analyzer for searcher by field id
     * @param fid field id
     * @return internal analyzer object
     */
    const Analyzer* getAnalyzerForSearcher(fieldid_t fid) const;

    /**
     * Return analyzer by producer identifier
     * @param sFieldName field name
     * @return internal analyzer object
     */
    const Analyzer* getAnalyzerForSearcher(const tstring& sFieldName) const;

    /**
     * Return analyzer by field id
     * @param fid field id
     * @return internal analyzer object
     */
    AnalyzerPtr getAnalyzer(fieldid_t fid);

    /**
     * Return analyzer for searcher by field id
     * @param fid field id
     * @return internal analyzer object
     */
    AnalyzerPtr getAnalyzerForSearcher(fieldid_t fid);

    /**
     * Return true if the specified field is an analyzed field
     */
    bool isAnalyzed(const tstring& sField) const;

    /**
     * Return true if the specified field is an indexed field
     */
    bool isIndexed(const tstring& sField) const;

    /**
     * clear analyzers
     */
    void clear();

    /**
     * Clone a new object
     */
    AnalyzerMapper* clone() const;

protected:
    /**
     * Create pipelined token filters
     */
    TokenFilter* createFilters(const tstring& sFilterStr);

    Analyzer* createAnalyzer(const FX_NS(document)::FieldType::Analyzer& analyzer);
    Analyzer* createAnalyzer(const std::string& sIdent,
                             const std::string& sParam);

    /**
     * Map a field id to an analyzer
     * @param fid field id
     * @param szFieldName field name
     * @param pAnalyzer analyzer
     * @param nameMap field name to analyzer map
     * @param idMap field id to analyzer map
     */
    void map(fieldid_t fid, const tstring& sFieldName, const AnalyzerPtr& pAnalyzer,
             AnalyzerMap& nameMap, AnalyzerIdMap& idMap);

private:			
    AnalyzerMap m_analyzerForIndex;
    AnalyzerIdMap m_analyzerForIndexById;

    AnalyzerMap m_analyzerForSearch;
    AnalyzerIdMap m_analyzerForSearchById;

    const FX_NS(document)::DocumentSchema* m_pDocSchema;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(AnalyzerMapper);

/////////////////////////////////////////////////
//
inline bool AnalyzerMapper::isAnalyzed(const tstring& sField) const
{
    const FX_NS(document)::FieldSchema* pSchema = m_pDocSchema->getSchema(sField);
    return pSchema ? pSchema->isAnalyzed() : false;
}

inline bool AnalyzerMapper::isIndexed(const tstring& sField) const
{
    const FX_NS(document)::FieldSchema* pSchema = m_pDocSchema->getSchema(sField);
    return pSchema ? pSchema->isIndexed() : false;
}

FX_NS_END

#endif
