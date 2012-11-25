#ifndef __FX_SIMPLEHTMLENCODER_H
#define __FX_SIMPLEHTMLENCODER_H

#include "Encoder.h"

FX_NS_DEF(highlight);

class SimpleHTMLEncoder : public Encoder
{
public:
    SimpleHTMLEncoder(void);
    virtual ~SimpleHTMLEncoder(void);

public:
    void encodeText(std::string& sEncodedText,
                    const char* originalText,
                    size_t length);
protected:
    static void htmlEncode(std::string& sEncodedText,
                           const char* originalText,
                           size_t length);
};

FX_NS_END

#endif
