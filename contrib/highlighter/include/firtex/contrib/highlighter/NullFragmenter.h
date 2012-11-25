#ifndef _NULLFRAGMENTER_H
#define _NULLFRAGMENTER_H

#include "Fragmenter.h"

FX_NS_DEF(highlight)

/**
 * {@link Fragmenter} implementation which does not fragment the text.
 * This is useful for highlighting the entire content of a document or field.
 */
class NullFragmenter : public Fragmenter 
{
public:
	NullFragmenter(){}
	virtual ~NullFragmenter(){}

	/**
	 * Initializes the Fragmenter
	 * @param originalText
	 */
	void start(const char* s){}

	/**
	 * Test to see if this token from the stream should be held in a new TextFragment
	 * @param nextToken
	 */
	bool isNewFragment(Token* pToken) {return false;}
}

FX_NS_END

#endif
