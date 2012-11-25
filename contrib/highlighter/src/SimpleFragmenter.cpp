#include "firtex/contrib/highlighter/SimpleFragmenter.h"
#include "firtex/analyzer/Token.h"

using namespace std;

FX_NS_USE(analyzer);

FX_NS_DEF(highlight);

SimpleFragmenter::SimpleFragmenter()
    : m_nFragmentSize(DEFAULT_FRAGMENT_SIZE)
{		
}

SimpleFragmenter::SimpleFragmenter(size_t nFragmentSize)
    : m_nFragmentSize(nFragmentSize)
{			
}

SimpleFragmenter::~SimpleFragmenter()
{
}

void SimpleFragmenter::start(const char* szOrgText, size_t length)
{
    m_nCurrentNumFrags = 1;
}

bool SimpleFragmenter::isNewFragment(const Token* pToken)
{
    bool isNewFrag = (pToken->getEndOffset() >=
                      (int32_t)(m_nFragmentSize * m_nCurrentNumFrags));
    if(isNewFrag)
    {
        m_nCurrentNumFrags++;
    }
    return isNewFrag;
}

FX_NS_END
