#include "SearchResource.h"

FX_NS_USE(index);

FX_NS_DEF(app);

SearchResource::SearchResource()
{
}

SearchResource::~SearchResource()
{
    m_pIndexReader.reset();
}

void SearchResource::init(const SearchServerConf& conf)
{
    m_conf = conf;

    m_index.open(conf.resourceConf.index_data, Index::READ, NULL);
    m_pIndexReader = m_index.acquireReader();
}

void SearchResource::reopenIndex()
{
    m_pIndexReader = m_index.acquireReader(true);
}
    
FX_NS_END
