#ifndef __FX_SEARCH_RESOURCE_H
#define __FX_SEARCH_RESOURCE_H

#include "SearchServerConf.h"
#include "firtex/index/Index.h"

FX_NS_DEF(app);

class SearchResource
{
public:
    SearchResource();
    ~SearchResource();

public:
    void init(const SearchServerConf& conf);
    void reopenIndex();

    FX_NS(index)::IndexReaderPtr getIndexReader() const;
    std::string getDefaultField() const;

private:
    SearchServerConf m_conf;
    FX_NS(index)::Index m_index;
    FX_NS(index)::IndexReaderPtr m_pIndexReader;
};

/////////////////////////////////////
//
inline FX_NS(index)::IndexReaderPtr SearchResource::getIndexReader() const
{
    return m_pIndexReader;
}

inline std::string SearchResource::getDefaultField() const
{
    return m_conf.resourceConf.default_field;
}

FX_NS_END

#endif//__FX_SEARCH_RESOURCE_H
