#include "firtex/utility/Path_WIN32.h"
#include "firtex/utility/Environment_WIN32.h"
#include "firtex/utility/Exception.h"
#include <windows.h>

FX_NS_DEF(utility)

std::string PathImpl::currentImpl()
{
    tchar buffer[_MAX_PATH];
    DWORD n = GetCurrentDirectory(sizeof(buffer), buffer);
    if (n > 0 && n < sizeof(buffer))
    {
        std::string result(buffer, n);
        if (result[n - 1] != '\\')
            result.append("\\");
        return result;
    }
    else FIRTEX_THROW(SystemException, "Cannot get current directory");
}


std::string PathImpl::homeImpl()
{
    std::string result = EnvironmentImpl::getImpl("HOMEDRIVE");
    result.append(EnvironmentImpl::getImpl("HOMEPATH"));
    std::string::size_type n = result.size();
    if (n > 0 && result[n - 1] != '\\')
        result.append("\\");
    return result;
}


std::string PathImpl::tempImpl()
{
    tchar buffer[_MAX_PATH];
    DWORD n = GetTempPath(sizeof(buffer), buffer);
    if (n > 0 && n < sizeof(buffer))
    {
        std::string result(buffer, n);
        if (result[n - 1] != '\\')
            result.append("\\");
        return result;
    }
    else throw SystemException("Cannot get current directory");
}

std::string PathImpl::expandImpl(const std::string& path)
{
    tchar buffer[_MAX_PATH];
    DWORD n = ExpandEnvironmentStrings(path.c_str(), buffer, sizeof(buffer));
    if (n > 0 && n < sizeof(buffer))
        return std::string(buffer, n - 1);
    else
        return path;
}

FX_NS_END
