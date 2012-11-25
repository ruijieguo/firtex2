#include "firtex/contrib/highlighter/GradientFormatter.h"
#include "firtex/utility/Exception.h"
#include "firtex/utility/String.h"
#include "firtex/utility/StringUtils.h"

#ifdef FX_WINDOWS
#pragma warning(push)
#pragma warning(disable:4996)
#endif

FX_NS_USE(utility);

FX_NS_DEF(highlight)

static char HEX_DIGITS[] = { '0', '1', '2', '3', '4', '5', '6', '7',
                             '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

GradientFormatter::GradientFormatter(float fMaxScore, const tchar* minForegroundColor,
                                     const tchar* maxForegroundColor, const tchar* minBackgroundColor,const tchar* maxBackgroundColor)
{
    m_bHighlightForeground = (minForegroundColor != NULL)
                             && (maxForegroundColor != NULL);
    if (m_bHighlightForeground)
    {
        if (strLength(minForegroundColor) != 7)
        {
            FIRTEX_THROW(IllegalArgumentException, _T("minForegroundColor is not 7 bytes long eg a hex RGB value such as #FFFFFF"));
        }
        if (strLength(maxForegroundColor) != 7)
        {
            FIRTEX_THROW(IllegalArgumentException, _T("minForegroundColor is not 7 bytes long eg a hex RGB value such as #FFFFFF"));
        }
        tchar substring[10];
        substring[2] = 0;

        strncpy(substring,minForegroundColor + 1,2);					
        m_fgRMin = hexToInt(substring);

        strncpy(substring,minForegroundColor + 3,2);
        m_fgGMin = hexToInt(substring);
        strncpy(substring,minForegroundColor + 5,2);
        m_fgBMin = hexToInt(substring);

        strncpy(substring,maxForegroundColor + 1,2);
        m_fgRMax = hexToInt(substring);
        strncpy(substring,maxForegroundColor + 5,2);
        m_fgGMax = hexToInt(substring);
        strncpy(substring,maxForegroundColor + 5,2);
        m_fgBMax = hexToInt(substring);
    }

    m_bHighlightBackground = (minBackgroundColor != NULL)
                             && (maxBackgroundColor != NULL);
    if (m_bHighlightBackground)
    {
        if (strLength(minBackgroundColor) != 7)
        {
            FIRTEX_THROW(IllegalArgumentException, _T("minBackgroundColor is not 7 bytes long eg a hex RGB value such as #FFFFFF"));
        }
        if (strLength(maxBackgroundColor) != 7)
        {
            FIRTEX_THROW(IllegalArgumentException, _T("minBackgroundColor is not 7 bytes long eg a hex RGB value such as #FFFFFF"));
        }

        tchar substring[10];
        substring[2] = 0;

        strncpy(substring,minBackgroundColor + 1,2);
        m_bgRMin = hexToInt(substring);

        strncpy(substring,minBackgroundColor + 3,2);
        m_bgGMin = hexToInt(substring);

        strncpy(substring,minBackgroundColor + 5,2);
        m_bgBMin = hexToInt(substring);

        strncpy(substring,maxBackgroundColor + 1,2);
        m_bgRMax = hexToInt(substring);

        strncpy(substring,maxBackgroundColor + 3,2);
        m_bgGMax = hexToInt(substring);

        strncpy(substring,maxBackgroundColor + 5,2);
        m_bgBMax = hexToInt(substring);
    }
    //        this.corpusReader = corpusReader;
    m_fMaxScore = fMaxScore;
    //        totalNumDocs = corpusReader.getDocCount();
}

char* GradientFormatter::highlightTerm(const char* originalText, TokenGroup* pTokenGroup)
{
    if (pTokenGroup->getTotalScore() == 0)
        return strdup(originalText);
    float score = pTokenGroup->getTotalScore();
    if (score == 0)
    {
        return strdup(originalText);
    }
    FX_NS(utility)::AString sb;
    sb.append("<font ",6);
    if (m_bHighlightForeground)
    {
        sb.append("color=\"",7);
        string str = getForegroundColorString(score);
        sb.append(str.c_str(),str.length());
        sb.append("\" ",2);
    }
    if (m_bHighlightBackground)
    {
        sb.append("bgcolor=\"",9);
        string str = getBackgroundColorString(score);
        sb.append(str.c_str(),str.length());
        sb.append("\" ",2);
    }
    sb.append(">",1);
    sb.append(originalText,strlen(originalText));
    sb.append("</font>",7);
    return sb.toString();
}

string GradientFormatter::getForegroundColorString(float fScore)
{
    int32_t rVal = getColorVal(m_fgRMin, m_fgRMax, fScore);
    int32_t gVal = getColorVal(m_fgGMin, m_fgGMax, fScore);
    int32_t bVal = getColorVal(m_fgBMin, m_fgBMax, fScore);
    FX_NS(utility)::AString sb;
    sb.append("#",1);
    string str = intToHex(rVal);
    sb.append(str);
    str = intToHex(gVal);
    sb.append(str);
    str = intToHex(bVal);
    sb.append(str);
    return sb.toString();
}

string GradientFormatter::getBackgroundColorString(float fScore)
{
    int32_t rVal = getColorVal(m_bgRMin, m_bgRMax, fScore);
    int32_t gVal = getColorVal(m_bgGMin, m_bgGMax, fScore);
    int32_t bVal = getColorVal(m_bgBMin, m_bgBMax, fScore);
    FX_NS(utility)::AString sb;
    sb.append("#",1);
    string str = intToHex(rVal);;
    sb.append(str);
    str = intToHex(gVal);
    sb.append(str);
    str = intToHex(bVal);
    sb.append(str);
    return sb.toString();
}

int32_t GradientFormatter::getColorVal(int32_t colorMin, int32_t colorMax, float score)
{
    if (colorMin == colorMax)
    {
        return colorMin;
    }
    float scale = (float)abs(colorMin - colorMax);
    float relScorePercent = min(m_fMaxScore, score) / m_fMaxScore;
    float colScore = scale * relScorePercent;
    return min(colorMin, colorMax) + (int32_t) colScore;
}

string GradientFormatter::intToHex(int32_t i)
{
    string str;
    str += HEX_DIGITS[(i & 0xF0) >> 4];
    str += HEX_DIGITS[i & 0x0F];
    return str;
}

int32_t GradientFormatter::hexToInt(const tchar* hex)
{
    int32_t len = (int32_t)strLength(hex);
    if (len > 16)
        FIRTEX_THROW(NumberFormatException, "lenth greater than 16");

    int32_t l = 0;
    for (int32_t i = 0; i < len; i++)
    {
        l <<= 4;
        int32_t c;
        switch(hex[i])
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            c = hex[i] - '0';
        break;
        case 'A':
            c = 10;
            break;
        case 'B':
            c = 11;
            break;
        case 'C':
            c = 12;
            break;
        case 'D':
            c = 13;
            break;
        case 'E':
            c = 14;
            break;
        case 'F':
            c = 15;
            break;
        default:
            FIRTEX_THROW(NumberFormatException, _T("hex=[%s]"), hex);
            break;
        }
        l |= c;
    }
    return l;
}

FX_NS_END

#ifdef FX_WINDOWS
#pragma warning(pop)
#endif
