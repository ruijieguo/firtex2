#include "firtex/utility/ZLib.h"
#include "../external/zlib/zlib.h"
#include <string.h>

FX_NS_DEF(utility);

ZLib::ZLib(void)
{
}

ZLib::~ZLib(void)
{

}

byte* ZLib::compress(const void* pInput,size_t nInput, size_t* pnOutput, size_t nSuggest)
{		
    *pnOutput = nSuggest ? nSuggest : (size_t)compressBound( (uLong)nInput ); 
	
    byte* pBuffer = new byte[ *pnOutput ];

    if ( ::compress(pBuffer,(uLongf*)pnOutput,(const byte *)pInput,(uLong)nInput ) )
    {			
        delete [] pBuffer;
        return NULL;
    }

	
    byte* pOutput = new byte[ *pnOutput ]; // Allocate a new buffer exactly big enough to hold the bytes compress wrote
    memcpy( pOutput, pBuffer, *pnOutput ); // Copy the compressed bytes from the old buffer to the new one
    delete [] pBuffer;                     // Delete the old buffer
    return pOutput;                        // Return the new one
}

byte* ZLib::decompress(const void* pInput, size_t nInput, size_t* pnOutput, size_t nSuggest)
{
    *pnOutput = nSuggest ? nSuggest : nInput * 6;			
    byte* pBuffer = new byte[ *pnOutput ];

    if (::uncompress(pBuffer, (uLongf*)pnOutput, (const byte *)pInput, (uLong)nInput))
    {
        delete [] pBuffer;
        return NULL;
    }

    byte* pOutput = new byte[ *pnOutput ];
    memcpy(pOutput,pBuffer, *pnOutput );  
    delete [] pBuffer;                    
    return pOutput;                       
}

FX_NS_END

