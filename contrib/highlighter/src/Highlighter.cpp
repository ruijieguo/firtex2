#include "firtex/contrib/highlighter/Highlighter.h"
#include "firtex/utility/Exception.h"
#include "firtex/contrib/highlighter/SimpleHTMLFormatter.h"
#include "firtex/contrib/highlighter/DefaultEncoder.h"
#include "firtex/contrib/highlighter/SimpleFragmenter.h"
#include "firtex/contrib/highlighter/Scorer.h"
#include "firtex/analyzer/Analyzer.h"
#include "firtex/analyzer/TokenSource.h"
#include "firtex/analyzer/Token.h"

#include <vector>
#include <string>
#include <memory>

using namespace std;

FX_NS_USE(analyzer);
FX_NS_USE(index);
FX_NS_USE(utility);

FX_NS_DEF(highlight);

Highlighter::Highlighter(Scorer* pFragmentScorer)
    : m_pFragmentScorer (pFragmentScorer)
    , m_pTextFragmenter(new SimpleFragmenter)
    , m_pEncoder(new DefaultEncoder)
    , m_pFormatter(new SimpleHTMLFormatter)
    , m_nMaxDocBytesToAnalyze(DEFAULT_MAX_DOC_BYTES_TO_ANALYZE)
{				
}


Highlighter::Highlighter(Formatter* pFormatter, Scorer* pFragmentScorer)
    : m_pFragmentScorer (pFragmentScorer)
    , m_pTextFragmenter(new SimpleFragmenter)
    , m_pEncoder(new DefaultEncoder)
    , m_pFormatter(pFormatter)
    , m_nMaxDocBytesToAnalyze(DEFAULT_MAX_DOC_BYTES_TO_ANALYZE)
{
	
}


Highlighter::Highlighter(Formatter* pFormatter, Encoder* pEncoer,
                         Scorer* pFragmentScorer)
    : m_pFragmentScorer (pFragmentScorer)
    , m_pTextFragmenter(new SimpleFragmenter)
    , m_pEncoder(pEncoer)
    , m_pFormatter(pFormatter)
    , m_nMaxDocBytesToAnalyze(DEFAULT_MAX_DOC_BYTES_TO_ANALYZE)
{				
}

Highlighter::~Highlighter()
{
    if (m_pFormatter)
    {
        delete m_pFormatter;
        m_pFormatter = NULL;
    }
    if (m_pEncoder)
    {
        delete m_pEncoder;
        m_pEncoder = NULL;
    }
    if (m_pTextFragmenter)
    {
        delete m_pTextFragmenter;
        m_pTextFragmenter = NULL;
    }
    if (m_pFragmentScorer)
    {
        delete m_pFragmentScorer;
        m_pFragmentScorer = NULL;
    }
}

void Highlighter::getBestFragments(string& sMarkedText,
                                   const Analyzer* pAnalyzer,
                                   const char* szText,
                                   size_t nTextLen,
                                   size_t nMaxFrags,
                                   const string& sSeparator)
{
    size_t _nMaxFrags = MAX(1, nMaxFrags); //sanity check

    TextFragmentArray frags;
    getBestTextFragments(pAnalyzer, szText, nTextLen,
                         (int32_t)_nMaxFrags, true, frags);

    TextFragment* pFrag;
    size_t nNumFrags = frags.getNumTextFragment();
    for (size_t i = 0; i < nNumFrags; i++)
    {
        pFrag = frags.getTextFragment(i);
        if ((pFrag != NULL) && (pFrag->getScore() > 0))
        {
            pFrag->markString(sMarkedText, szText, nTextLen,
                    m_pFormatter, m_pEncoder);
        }
        if (nNumFrags > 1 || nTextLen > sMarkedText.size())
        {
            sMarkedText += sSeparator;
        }
    }
}

void Highlighter::getBestTextFragments(const Analyzer* pAnalyzer,
                                       const char* szText,
                                       size_t nTextLen,
                                       size_t nMaxFrags,
                                       bool bMergeContiguousFragments,
                                       TextFragmentArray& fragArray)
{
    TextFragment* pCurrentFrag = new TextFragment(0,
            (int32_t)fragArray.getNumTextFragment());
    fragArray.add(pCurrentFrag);
    
    m_pFragmentScorer->startFragment(pCurrentFrag);

    try
    {
        m_pTextFragmenter->start(szText, nTextLen);

        TokenGroupPtr pTokenGroup(new TokenGroup);
        TokenSourcePtr tokenSource(new FX_NS(analyzer)::TokenSource);
        TokenViewPtr pOrgTokenView(new TokenView());
        pOrgTokenView->addToken(szText, nTextLen);
        tokenSource->setOriginalView(pOrgTokenView);

        TokenViewPtr pTokenView = pAnalyzer->tokenize(tokenSource);

        int32_t lastEndOffset = 0;
        float lastScore = 0.0f;
        TokenView::Iterator iter = pTokenView->iterator();
        while (iter.hasNext())
        {
            const Token& token = iter.next();
            if (token.getStartOffset() < (int32_t)m_nMaxDocBytesToAnalyze)
            {
                if ((pTokenGroup->getNumTokenView() > 0) 
                    && (pTokenGroup->isDistinct(&token)))
                {
                    if (pTokenGroup->getTotalScore() > 0)
                    {
                        pCurrentFrag->addMatchedToken(pTokenGroup->getMatchedStartOffset(),
                                pTokenGroup->getMatchedEndOffset());
                    }

                    pTokenGroup->clear();

                    //check if current token marks the start of a new fragment
                    if (m_pTextFragmenter->isNewFragment(&token))
                    {
                        pCurrentFrag->setScore(m_pFragmentScorer->getFragmentScore());
                        //record stats for a new fragment
                        pCurrentFrag->setTextEndPos(token.getEndOffset());
                        pCurrentFrag = new TextFragment((int32_t)token.getStartOffset(),
                                (int32_t)fragArray.getNumTextFragment());
                        fragArray.add(pCurrentFrag);
                        m_pFragmentScorer->startFragment(pCurrentFrag);
                    }
                }

                TermPtr pTerm(new Term(m_pFragmentScorer->getFieldName(), token.getTextValue()));
                lastScore = m_pFragmentScorer->getTokenScore(pTerm.get());
                pTokenGroup->addToken(&token, lastScore);
            }
            lastEndOffset = token.getEndOffset();
        }//end while

        if ((lastScore > 0.0f) 
            && (pTokenGroup->getNumTokenView() > 0) 
            && (pTokenGroup->getTotalScore() > 0))
        {
            pCurrentFrag->addMatchedToken(pTokenGroup->getMatchedStartOffset(),
                    pTokenGroup->getMatchedEndOffset());
            pTokenGroup->clear();
        }

        pCurrentFrag->setScore(m_pFragmentScorer->getFragmentScore());
        pCurrentFrag->setTextEndPos(lastEndOffset);

        FragmentQueue fragQueue(nMaxFrags);

        //sort the most relevant sections of the text
        for (size_t i = 0; i < fragArray.getNumTextFragment(); i++)
        {
            TextFragment* pFrag = fragArray.getTextFragment(i);
            if (!fragQueue.insert(pFrag))
            {
                delete pFrag;
            }
        }
        fragArray.clear(false);
		
        //return the most relevant fragments
        while (fragQueue.size() > 0)
        {
            fragArray.add(fragQueue.pop());
        }
		
        //merge any contiguous fragments to improve readability
        if (bMergeContiguousFragments)
        {
            TextFragment* pTextFragment;
            TextFragmentArray tmpFragArray;
            for (size_t i = 0; i < fragArray.getNumTextFragment(); i++)
            {
                pTextFragment = fragArray.getTextFragment(i);
                tmpFragArray.add(pTextFragment);
            }
            fragArray.clear(false);

            mergeContiguousFragments(tmpFragArray);

            for (size_t i = 0; i < tmpFragArray.getNumTextFragment(); i++)
            {
                pTextFragment = tmpFragArray.getTextFragment(i);
                if ( pTextFragment != NULL)
                {
                    if (pTextFragment->getScore() > 0)
                    {
                        fragArray.add(pTextFragment);
                    }
                    else
                    {
                        delete pTextFragment;
                    }
                }
            }
            tmpFragArray.clear(false);
        }
    }
    catch (const FirteXException& e)
    {
        FIRTEX_RETHROW(e);
    }
}

void Highlighter::mergeContiguousFragments(TextFragmentArray& fragArray)
{
    bool bMergingStillBeingDone;
    if (fragArray.getNumTextFragment() > 1)
    {
        TextFragment* pTextFrag;
        TextFragment* pTextFrag2;
        do
        {
            bMergingStillBeingDone = false; //initialize loop control flag
            //for each fragment, scan other frags looking for contiguous blocks
            for (size_t i = 0; i < fragArray.getNumTextFragment(); i++)
            {
                pTextFrag = fragArray.getTextFragment(i);

                if (pTextFrag == NULL)
                {
                    continue;
                }

                //merge any contiguous blocks 
                for (size_t x = 0; x < fragArray.getNumTextFragment(); x++)
                {
                    pTextFrag2 = fragArray.getTextFragment(x);
                    if (pTextFrag2 == NULL)
                    {
                        continue;
                    }
                    if (pTextFrag == NULL)
                    {
                        break;
                    }
                    TextFragment* pFrag1 = NULL;
                    TextFragment* pFrag2 = NULL;
                    size_t frag1Num = 0;
                    size_t frag2Num = 0;
                    size_t bestScoringFragNum;
                    size_t worstScoringFragNum;
                    //if blocks are contiguous....
                    if (pTextFrag->follows(pTextFrag2))
                    {
                        pFrag1 = pTextFrag2;
                        frag1Num = x;
                        pFrag2 = pTextFrag;
                        frag2Num = i;
                    }
                    else if (pTextFrag2->follows(pTextFrag))
                    {
                        pFrag1 = pTextFrag;
                        frag1Num = i;
                        pFrag2 = pTextFrag2;
                        frag2Num = x;
                    }
					
                    //merging required..
                    if (pFrag1 != NULL)
                    {
                        if (pFrag1->getScore() > pFrag2->getScore())
                        {
                            bestScoringFragNum = frag1Num;
                            worstScoringFragNum = frag2Num;
                        }
                        else
                        {
                            bestScoringFragNum = frag2Num;
                            worstScoringFragNum = frag1Num;
                        }
                        pFrag1->merge(pFrag2);
                        fragArray.setTextFragment(worstScoringFragNum, NULL,
                                worstScoringFragNum != frag1Num);
                        bMergingStillBeingDone = true;
                        fragArray.setTextFragment(bestScoringFragNum, pFrag1,
                                bestScoringFragNum != frag1Num);
                    }
                }
            }
        }while (bMergingStillBeingDone);
    }
}

size_t Highlighter::getMaxDocBytesToAnalyze() const
{
    return m_nMaxDocBytesToAnalyze;
}

void Highlighter::setMaxDocBytesToAnalyze(size_t nByteCount)
{
    m_nMaxDocBytesToAnalyze = nByteCount;
}

Fragmenter* Highlighter::getTextFragmenter()
{
    return m_pTextFragmenter;
}

void Highlighter::setTextFragmenter(Fragmenter* pFragmenter)
{
    if (m_pTextFragmenter && m_pTextFragmenter != pFragmenter)
        delete m_pTextFragmenter;
    m_pTextFragmenter = pFragmenter;
}

Scorer* Highlighter::getFragmentScorer()
{
    return m_pFragmentScorer;
}

void Highlighter::setFragmentScorer(Scorer* pScorer)
{
    if (m_pFragmentScorer)
        delete m_pFragmentScorer;
    m_pFragmentScorer = pScorer;
}

void Highlighter::setEncoder(Encoder* pEncoder)
{
    if (m_pEncoder && m_pEncoder != pEncoder)
        delete m_pEncoder;
    m_pEncoder = pEncoder;
}

void Highlighter::setFormatter(Formatter* pFormatter)
{
    if (m_pFormatter && m_pFormatter != pFormatter)
        delete m_pFormatter;
    m_pFormatter = pFormatter;
}

FX_NS_END
