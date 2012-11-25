#ifndef _QUERYTERMEXTRACTOR_H
#define _QUERYTERMEXTRACTOR_H

#include <string>
#include "WeightedTerm.h"

FX_NS_DEF(search);
class Query;
class BooleanQuery;
FX_NS_END

FX_NS_DEF(index);
class IndexReader;
FX_NS_END

FX_NS_DEF(highlight);
class QueryTermExtractor
{
public:
    QueryTermExtractor(void);
    ~QueryTermExtractor(void);

public:
    /**
     * Extracts all terms texts of a given Query into an array of WeightedTerms
     *
     * @param pQuery Query to extract term from
     * @return an array of the terms used in a query, plus their weights.
     */
    static WeightedTermsPtr getTerms(const FX_NS(search)::Query* pQuery);

    /**
     * Extracts all terms texts of a given Query into an array of WeightedTerms
     *
     * @param pQuery      Query to extract term texts from
     * @param pReader used to compute IDF which can be used to a) score selected fragments better 
     * b) use graded highlights eg chaning intensity of font color
     * @param fieldName the field on which Inverse Document Frequency (IDF) calculations are based
     * @return an array of the terms used in a query, plus their weights.
     */
    static WeightedTermsPtr getIdfWeightedTerms(const FX_NS(search)::Query* pQuery,
            FX_NS(index)::IndexReader* pRreader,
            const std::string& fieldName);

    /**
     * Extracts all terms texts of a given Query into an array of WeightedTerms
     *
     * @param pQuery      Query to extract term texts from
     * @param prohibited <code>true</code> to extract "prohibited" terms, too
     * @param fieldName  The fieldName used to filter query terms
     * @return an array of the terms used in a query, plus their weights.
     */
    static WeightedTermsPtr getTerms(const FX_NS(search)::Query* pQuery, bool prohibited,
                                   const std::string& fieldName);

    /**
     * Extracts all terms texts of a given Query into an array of WeightedTerms
     *
     * @param pQuery      Query to extract term texts from
     * @param prohibited <code>true</code> to extract "prohibited" terms, too
     * @return an array of the terms used in a query, plus their weights.
     */
    static WeightedTermsPtr getTerms(const FX_NS(search)::Query* pQuery, bool prohibited);

protected:
    static void getTerms(const FX_NS(search)::Query* pQuery, WeightedTermsPtr& pWTs,
                         bool prohibited, const std::string& fieldName);

    /**
     * extractTerms is currently the only query-independent means of introspecting queries but it only reveals
     * a list of terms for that query - not the boosts each individual term in that query may or may not have.
     * "Container" queries such as BooleanQuery should be unwrapped to get at the boost info held
     * in each child element. 
     * Some discussion around this topic here:
     * http://www.gossamer-threads.com/lists/lucene/java-dev/34208?search_string=introspection;#34208
     * Unfortunately there seemed to be limited interest in requiring all Query objects to implement
     * something common which would allow access to child queries so what follows here are query-specific
     * implementations for accessing embedded query elements. 
     */
    static void getTermsFromBooleanQuery(const FX_NS(search)::BooleanQuery* pQuery,
            WeightedTermsPtr& pWTs, bool prohibited, const std::string& fieldName);			
};

FX_NS_END

#endif
