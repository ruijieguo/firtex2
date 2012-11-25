#ifndef _GRADIENTFORMATTER_H
#define _GRADIENTFORMATTER_H

#include <string>
#include "Formatter.h"

using namespace std;

FX_NS_DEF(highlight)

class GradientFormatter : public Formatter
{
	/**
	 * Sets the color range for the IDF scores
	 * 
	 * @param maxScore
	 *            The score (and above) displayed as maxColor (See QueryScorer.getMaxWeight 
	 * 			  which can be used to callibrate scoring scale)
	 * @param minForegroundColor
	 *            The hex color used for representing IDF scores of zero eg
	 *            #FFFFFF (white) or null if no foreground color required
	 * @param maxForegroundColor
	 *            The largest hex color used for representing IDF scores eg
	 *            #000000 (black) or null if no foreground color required
	 * @param minBackgroundColor
	 *            The hex color used for representing IDF scores of zero eg
	 *            #FFFFFF (white) or null if no background color required
	 * @param maxBackgroundColor
	 *            The largest hex color used for representing IDF scores eg
	 *            #000000 (black) or null if no background color required
	 */
	GradientFormatter(float fMaxScore, const tchar* minForegroundColor,
		const tchar* maxForegroundColor, const tchar* minBackgroundColor,
		const tchar* maxBackgroundColor);
public:
	char* highlightTerm(const char* originalText, TokenGroup* pTokenGroup);

protected:
	string getForegroundColorString(float fScore);

	string getBackgroundColorString(float fScore);

	int32_t getColorVal(int32_t colorMin, int32_t colorMax, float fScore);			

	static string intToHex(int32_t i);

	/**
	 * Converts a hex string into an int. Integer.parseInt(hex, 16) assumes the
	 * input is nonnegative unless there is a preceding minus sign. This method
	 * reads the input as twos complement instead, so if the input is 8 bytes
	 * long, it will correctly restore a negative int produced by
	 * Integer.toHexString() but not neccesarily one produced by
	 * Integer.toString(x,16) since that method will produce a string like '-FF'
	 * for negative integer values.
	 * 
	 * @param hex
	 *            A string in capital or lower case hex, of no more then 16
	 *            characters.
	 * @throws NumberFormatException
	 *             if the string is more than 16 characters long, or if any
	 *             character is not in the set [0-9a-fA-f]
	 */
	static int32_t hexToInt(const tchar* hex);
protected:
	float	m_fMaxScore;

	int32_t m_fgRMin;
	int32_t m_fgGMin;
	int32_t m_fgBMin;

	int32_t m_fgRMax;
	int32_t m_fgGMax;
	int32_t m_fgBMax;

	bool	m_bHighlightForeground;
	bool	m_bHighlightBackground;

	int32_t m_bgRMin;
	int32_t m_bgGMin;
	int32_t m_bgBMin;

	int32_t m_bgRMax;
	int32_t m_bgGMax;
	int32_t m_bgBMax;			
};

FX_NS_END

#endif
