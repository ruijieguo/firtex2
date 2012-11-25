#include "firtex/index/PostingCodingFactory.h"
#include "firtex/index/OptPFDPostingCoding.h"

FX_NS_DEF(index);

PostingCodingFactory::PostingCodingFactory() 
    : m_pDefaultCoding(NULL)
{
    init();
}

PostingCodingFactory::~PostingCodingFactory() 
{
    delete m_pDefaultCoding;
}

void PostingCodingFactory::init()
{
    m_pDefaultCoding = new OptPFDPostingCoding();
}

const PostingCoding* PostingCodingFactory::getDocCoding() const
{
    return m_pDefaultCoding;
}

const PostingCoding* PostingCodingFactory::getTfCoding() const
{
    return m_pDefaultCoding;
}

const PostingCoding* PostingCodingFactory::getPosCoding() const
{
    return m_pDefaultCoding;
}

const PostingCoding* PostingCodingFactory::getSkipListCoding() const
{
    return m_pDefaultCoding;    
}

FX_NS_END

