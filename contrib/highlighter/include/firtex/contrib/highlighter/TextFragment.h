#ifndef _TEXTFRAGMENT_H
#define _TEXTFRAGMENT_H

#include <string>
#include <vector>
#include <map>
#include "firtex/common/StdHeader.h"
#include "firtex/utility/Exception.h"

FX_NS_DEF(highlight)

class Formatter;
class Encoder;

/**
 * Low-level class used to record information about a section of a document 
 * with a score.
 * @author MAHarwood
 */
class TextFragment
{
public:
    typedef std::pair<int32_t, int32_t> OffsetPair;
    typedef std::vector<OffsetPair> OffsetVector;

public:
    TextFragment(int32_t nTextStartPos, int32_t nFragNum);
    ~TextFragment();

public:
    void setScore(float fScore);

    float getScore() const;
	
    /**
     * @param frag2 Fragment to be merged into this one
     */
    void merge(const TextFragment* pFrag2);

    /**
     * @param fragment 
     * @return true if this fragment follows the one passed
     */
    bool follows(const TextFragment* pFragment);

    /**
     * @return the fragment sequence number
     */
    int32_t getFragNum() const;

    /* 
     * Returns the marked-up text for this text fragment 
     */
    void markString(std::string& sMarkedText, const char* szText,
                    size_t nTextLen, Formatter* pFormatter, Encoder* pEncoder);
    
    void setTextStartPos(int32_t nStartPos)
    {
        m_nTextStartPos = nStartPos;
    }

    int32_t getTextStartPos() const
    {
        return m_nTextStartPos;
    }

    void setTextEndPos(int32_t nEndPos)
    {
        m_nTextEndPos = nEndPos;
    }

    int32_t getTextEndPos() const
    {
        return m_nTextEndPos;
    }

    void addMatchedToken(int32_t nStartOffset, int32_t nEndOffset)
    {
        m_matchedOffsets.push_back(std::make_pair(nStartOffset, nEndOffset));
    }

protected:
    int32_t m_nTextStartPos;
    int32_t m_nTextEndPos;
    int32_t m_nFragNum;
    float m_fScore;
    OffsetVector m_matchedOffsets;
    friend class Highlighter;
};

class TextFragmentArray
{
protected:
    const static int32_t INIT_SIZE = 10;

public:
    TextFragmentArray()
        : m_nNumFragment(0)
        , m_nMaxNumFragment(INIT_SIZE)
    {
        m_ppTextFragment = new TextFragment*[INIT_SIZE];
        memset(m_ppTextFragment,0,INIT_SIZE*sizeof(TextFragment*));
    }
    TextFragmentArray(size_t nInitSize)
        : m_nNumFragment(0)
        , m_nMaxNumFragment(nInitSize)
    {
        m_ppTextFragment = new TextFragment*[nInitSize];
        memset(m_ppTextFragment,0,nInitSize*sizeof(TextFragment*));
    }

    ~TextFragmentArray()
    {
        clear(true);
        delete[] m_ppTextFragment;
    }
public:
    /**
     * add text fragment
     * @param pTextFragment text fragment
     */
    void add(TextFragment* pTextFragment)
    {
        if(m_nNumFragment >= m_nMaxNumFragment)
            grow();
        m_ppTextFragment[m_nNumFragment++] = pTextFragment;
    }

    /**
     * get text fragment
     * @param _off offset of text fragment array
     * @return text fragment
     */
    TextFragment* getTextFragment(size_t _off)
    {
        FIRTEX_ASSERT2(_off < m_nNumFragment);
        return m_ppTextFragment[_off];
    }

    /**
     * set text fragment
     * @param _off offset of text fragment array
     * @param pTextFragment text fragment
     */
    void setTextFragment(size_t _off,TextFragment* pTextFragment,bool bDelete)
    {
        FIRTEX_ASSERT2(_off < m_nNumFragment);
        if(m_ppTextFragment[_off] && bDelete)
            delete m_ppTextFragment[_off];
        m_ppTextFragment[_off] = pTextFragment;
    }

    /**
     * get number of text fragment
     * @return number of text fragment
     */
    size_t getNumTextFragment() const {return m_nNumFragment;}

    /**
     * clear text fragments
     * @param bDelete delete text fragments or not
     */
    void clear(bool bDelete)
    {
        if(bDelete)
        {
            for (size_t i = 0;i < m_nNumFragment;i++)
            {
                if(m_ppTextFragment[i])
                {
                    delete m_ppTextFragment[i];
                    m_ppTextFragment[i] = NULL;
                }
            }							
        }
        else 
        {
            memset(m_ppTextFragment, 0, m_nNumFragment * sizeof(TextFragment*));
        }
        m_nNumFragment = 0;
    }

protected:
    void grow()
    {
        TextFragment** pTmp = new TextFragment*[m_nMaxNumFragment + INIT_SIZE];
        memcpy(pTmp, m_ppTextFragment, m_nMaxNumFragment* sizeof(TextFragment*));
        memset(&pTmp[m_nMaxNumFragment], 0, INIT_SIZE * sizeof(TextFragment*));
        delete[] m_ppTextFragment;
        m_ppTextFragment = pTmp;
        m_nMaxNumFragment += INIT_SIZE;
    }

protected:
    TextFragment** m_ppTextFragment;
    size_t m_nNumFragment;
    size_t m_nMaxNumFragment;		
};		

FX_NS_END

#endif
