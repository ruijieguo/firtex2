#ifndef __FX_HIGHLIGHTER_H
#define __FX_HIGHLIGHTER_H

#include <string>
#include "firtex/utility/PriorityQueue.h"
#include "firtex/common/SharedPtr.h"
#include "TextFragment.h"

FX_NS_DEF(analyzer)
class Analyzer;
FX_NS_END

FX_NS_DEF(highlight);

class Scorer;
class Formatter;
class Encoder;
class Fragmenter;

/**
 * Class used to markup highlighted terms found in the best sections of a
 * text, using configurable {@link Fragmenter}, {@link Scorer}, {@link Formatter},
 * {@link Encoder} and tokenizers.
 * @author mark@searcharea.co.uk
 */
class Highlighter
{		
public:
    Highlighter(Scorer* pFragmentScorer);
    Highlighter(Formatter* pFormatter, Scorer* pFragmentScorer);
    Highlighter(Formatter* pFormatter, Encoder* pEncoder,
                Scorer* pFragmentScorer);
    ~Highlighter();

public:
    /**
     * Highlights chosen terms in a text, extracting the most relevant sections.
     * @param pAnalyzer the analyzer that will be used to split <code>szText</code> into chunks
     * @param sText text to highlight terms in
     * @param nMaxFrags  the maximum number of fragments.
     * @param sSeparator separator char for multi fragments
     * @param sMarkedText the highlighted text returned.
     */
    void getBestFragments(std::string& sMarkedText,
                          const FX_NS(analyzer)::Analyzer* pAnalyzer,
                          const char* szText,
                          size_t nTextLen,
                          size_t nMaxFrags,
                          const std::string& sSeparator = "...");

    /**
     * @return the maximum number of bytes to be tokenized per doc 
     */
    size_t getMaxDocBytesToAnalyze() const;

    /**
     * @param byteCount the maximum number of bytes to be tokenized per doc
     * (This can improve performance with large documents)
     */
    void setMaxDocBytesToAnalyze(size_t nByteCount);

    /**
     * get text fragmenter
     * @return internal fragmenter of highlighter
     */
    Fragmenter* getTextFragmenter();

    /**
     * set text fragmenter
     * @param pFragmenter fragmenter, will be deleted by the highlighter 
     */
    void setTextFragmenter(Fragmenter* pFragmenter);

    /**
     * get fragment scorer
     * @return Object used to score each text fragment 
     */
    Scorer* getFragmentScorer();

    /**
     * set fragment scorer
     * @param pScorer scorer, will be deleted by the highlighter
     */
    void setFragmentScorer(Scorer* pScorer);

    /**
     * get encoder of highlighter
     * @return encoder, reference to internal encoder
     */
    Encoder* getEncoder(){return m_pEncoder;}
	
    /**
     * set encoder of highlighter
     * @param pEncoder encoder of highlighter, will be deleted by the highlighter
     */
    void setEncoder(Encoder* pEncoder);

    /**
     * set formatter of highlighter
     * @return reference to internal formatter
     */
    Formatter* getFormatter(){return m_pFormatter;}

    /** 
     * set formatter of highlighter
     * @param pFormatter formatter of highlighter, will be deleted by the highlighter
     */
    void setFormatter(Formatter* pFormatter);

protected:
    /**
     * Low level api to get the most relevant (formatted) sections of the document.
     * @param pAnalyer
     * @param szText
     * @param nTextLen
     * @param nMaxFrags
     * @param bMergeContiguousFragments
     * @param fragArray
     * @throws FileIOException
     */
    void getBestTextFragments(const FX_NS(analyzer)::Analyzer* pAnalyzer,
                              const char* szText,
                              size_t nTextLen,
                              size_t nMaxFrags,
                              bool bMergeContiguousFragments,
                              TextFragmentArray& fragArray);

    /**
     * Improves readability of a score-sorted list of 
     * TextFragmentArray by merging any fragments 
     * that were contiguous in the original text into
     * one larger fragment with the correct order.
     * This will leave a "null" in the array entry
     * for the lesser scored fragment. 
     * @param frag An array of document fragments in descending score
     */
    void mergeContiguousFragments(TextFragmentArray& fragArray);

public:
    const static int32_t DEFAULT_MAX_DOC_BYTES_TO_ANALYZE = 50 * 1024;
	
private:
    Scorer* m_pFragmentScorer;
    Fragmenter* m_pTextFragmenter;
    Encoder* m_pEncoder;
    Formatter* m_pFormatter;
    size_t m_nMaxDocBytesToAnalyze;
};

DEFINE_TYPED_PTR(Highlighter);

struct FragmentComparator
{
    bool operator () (TextFragment* pFragA, TextFragment* pFragB)
    {				
        if (pFragA->getScore() == pFragB->getScore())
        {
            return pFragA->getFragNum() > pFragB->getFragNum();
        }
        else
        {
            return pFragA->getScore() < pFragB->getScore();
        }
    }
};

class FragmentQueue : public FX_NS(utility)::PriorityQueue<TextFragment*, FragmentComparator>
{
public:
    FragmentQueue(size_t size)
        : FX_NS(utility)::PriorityQueue<TextFragment*, FragmentComparator>(size, true)
    {
    }
};

FX_NS_END

#endif
