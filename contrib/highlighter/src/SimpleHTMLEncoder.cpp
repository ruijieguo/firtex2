
#include "firtex/common/StdHeader.h"
#include "firtex/contrib/highlighter/SimpleHTMLEncoder.h"
#include "firtex/utility/String.h"

FX_NS_DEF(highlight);

SimpleHTMLEncoder::SimpleHTMLEncoder()
{
}

SimpleHTMLEncoder::~SimpleHTMLEncoder(void)
{
}

void SimpleHTMLEncoder::encodeText(std::string& sEncodedText,
                                   const char* originalText,
                                   size_t length)
{
    SimpleHTMLEncoder::htmlEncode(sEncodedText, originalText, length);
}

void SimpleHTMLEncoder::htmlEncode(std::string& sEncodedText,
                                   const char* plainText,
                                   size_t length)
{
    if (plainText == NULL || length == 0)
    {
        return ;
    }

    for (size_t index = 0; index < length; index++) 
    {
        int ch = plainText[index];

        switch (ch) 
        {
        case '"':
            sEncodedText.append("&quot;",6);
            break;

        case '&':
            sEncodedText.append("&amp;",5);
            break;

        case '<':
            sEncodedText.append("&lt;",4);
            break;

        case '>':
            sEncodedText.append("&gt;",4);
            break;

        default:
            if (ch < 128) 
            {
                sEncodedText += ch;
            } 
            else 
            {
                sEncodedText.append("&#",2);
                sEncodedText += ch;
                sEncodedText.append(";",1);
            }
        }
    }
}

FX_NS_END
