#include "firtex/utility/IconvWrapper.h"

#ifdef FX_WINDOWS

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4018)
#pragma warning(disable:4267)
#pragma warning(disable:4244)
#pragma warning(disable:4273)
#pragma warning(disable:431)

#endif

#include "../external/iconv/iconv.h"

FX_NS_DEF(utility);

SETUP_STREAM_LOGGER(utility, IconvWrapper);

IconvWrapper::IconvWrapper() 
    : m_handle((iconv_t)-1)
{
}

IconvWrapper::~IconvWrapper() 
{
    close();
}

void IconvWrapper::open(const std::string& sToCode,
                        const std::string& sFromCode)
{
    m_handle = iconv_open(sToCode.c_str(), sFromCode.c_str());
    if (m_handle == (iconv_t)-1)
    {
        FIRTEX_THROW(BadParameterException, "Open iconv for convertion "
                     "from [%s] to [%s] FAILED.", 
                     sFromCode.c_str(), sToCode.c_str());
    }
}

bool IconvWrapper::convert(std::string& sOutStr, const std::string& sInStr) const
{
    FIRTEX_ASSERT2(m_handle != (iconv_t)-1);

    char* pInBuf = (char*)sInStr.c_str();
    size_t nInBytes = sInStr.length();

    sOutStr.reserve(sInStr.length() << 1);

    char outbuf[1024];
    while (nInBytes > 0)
    {
        char* pOutBuf = (char*)outbuf;
        size_t nOutBytes = 1024;
        size_t nRetLen = iconv(m_handle, &pInBuf, &nInBytes, &pOutBuf, &nOutBytes);
        if (nRetLen != (size_t)-1)
        {
            sOutStr.append(outbuf, 1024 - nOutBytes);
        }
        else 
        {
            return false;
        }
    }
    return true;
}

size_t IconvWrapper::convert(char* szOutBuf, size_t nOutBufLen, 
                             const char* szInBuf, size_t nInBufLen) const
{
    FIRTEX_ASSERT2(m_handle != (iconv_t)-1);

    char* pInBuf = (char*)szInBuf;
    size_t nInBytes = nInBufLen;
    
    char* pOutBuf = szOutBuf;
    size_t nOutBytes = nOutBufLen;
    size_t ret = iconv(m_handle, &pInBuf, &nInBytes, &pOutBuf, &nOutBytes);
    if (ret != (size_t) -1)
    {
        return nOutBufLen - nOutBytes;
    }
    return 0;
}

void IconvWrapper::close()
{
    if (m_handle != (iconv_t)-1)
    {
        iconv_close(m_handle);
        m_handle = (iconv_t)-1;
    }
}




FX_NS_END

#ifdef FX_WINDOWS
#include "../external/iconv/iconv.c"
#include "../external/iconv/localcharset.c"
#include "../external/iconv/relocatable.c"

#pragma warning(pop)

#endif
