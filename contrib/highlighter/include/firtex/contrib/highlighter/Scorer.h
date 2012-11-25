
#ifndef _HIGHLIGHT_SCORER_H
#define _HIGHLIGHT_SCORER_H

#include "TextFragment.h"

FX_NS_DEF(index)
class Term;
FX_NS_END

FX_NS_DEF(highlight);

/**
 * Adds to the score for a fragment based on its tokens
 * @author mark@searcharea.co.uk
 */
class Scorer
{
public:
    Scorer(){}
    virtual ~Scorer(){};
public:
    /**
     * called when a new fragment is started for consideration
     * @param pNewFragment
     */
    virtual void startFragment(TextFragment* pNewFragment) = 0;

    /**
     * Called for each token in the current fragment
     * @param term The term to be scored
     * @return a score which is passed to the Highlighter 
     *  class to influence the mark-up of the text
     * (this return value is NOT used to score the fragment)
     */
    virtual float getTokenScore(const FX_NS(index)::Term* pTerm) = 0;

    /**
     * Called when the highlighter has no more tokens for the current fragment - the scorer returns
     * the weighting it has derived for the most recent fragment, typically based on the tokens
     * passed to getTokenScore().			 
     */	
    virtual float getFragmentScore() const = 0;

    virtual const tstring& getFieldName() const = 0;
};

FX_NS_END

#endif
