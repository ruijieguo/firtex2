#ifndef _DEFAULT_ENCODER_H
#define _DEFAULT_ENCODER_H

#include "Encoder.h"

FX_NS_DEF(highlight)

class DefaultEncoder :	public Encoder
{
public:

    DefaultEncoder(void)
    {
    }

    ~DefaultEncoder(void)
    {
    }

public:
    void encodeText(const char* szOrgText,
                    size_t length, 
                    std::string& sEncodedText) const
    {
        sEncodedText.append(szOrgText, length);
    }
};

FX_NS_END

#endif
