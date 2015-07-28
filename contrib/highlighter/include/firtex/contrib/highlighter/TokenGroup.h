#ifndef _TOKENGROUP_H
#define _TOKENGROUP_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"

FX_NS_DEF(analyzer)
class Token;
FX_NS_END

FX_NS_DEF(highlight);

/**
 * One, or several overlapping tokens, along with the score(s) and the
 * scope of the original text
 * @author MAHarwood
 */
class TokenGroup
{
protected:
    const static int32_t MAX_NUM_TOKENS_PER_GROUP = 50;

public:
    TokenGroup();
    ~TokenGroup();

public:
    void addToken(const FX_NS(analyzer)::Token* pToken, float score);

    bool isDistinct(const FX_NS(analyzer)::Token* pToken) const;

    void clear();

    /**
     * 
     * @param index a value between 0 and getNumTokenView -1
     * @return the "n"th token
     */
    // 	FX_NS(analyzer)::Token* getToken(int32_t nIndex) const
    // 	{
    // 		return m_ppTokenView[nIndex];
    // 	}

    /**
     * 
     * @param index a value between 0 and getNumTokenView -1
     * @return the "n"th score
     */
    // 	inline float getScore(int32_t nIndex) const
    // 	{
    // 		return m_pScores[nIndex];
    // 	}

    /**
     * @return the end position in the original text
     */
    inline int32_t getEndOffset() const
    {
        return m_nEndOffset;
    }

    /**
     * @return the start position in the original text
     */
    inline int32_t getStartOffset() const
    {
        return m_nStartOffset;
    }

    /**
     * @return start offset of matched term
     */
    inline int32_t getMatchedStartOffset() const
    {
        return m_nMatchStartOffset;
    }

    /**
     * @return start offset of matched term
     */
    inline int32_t getMatchedEndOffset() const
    {
        return m_nMatchEndOffset;
    }

    /**
     * @return the number of tokens in this group
     */
    inline int32_t getNumTokenView() const
    {
        return m_nNumTokenView;
    }

    /**
     * @return all tokens' scores summed up
     */
    inline float getTotalScore() const
    {
        return m_fTot;
    }

protected:
    int32_t	m_nNumTokenView;
    int32_t	m_nStartOffset;
    int32_t	m_nEndOffset;
    float m_fTot;

    int32_t	m_nMatchStartOffset;
    int32_t	m_nMatchEndOffset;

    friend class Highlighter;
};

DEFINE_TYPED_PTR(TokenGroup);

FX_NS_END

#endif

