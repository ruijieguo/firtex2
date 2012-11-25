#include "firtex/utility/Environment_WIN32.h"
#include "firtex/utility/Exception.h"
#include <sstream>
#include <windows.h>
#include <iphlpapi.h>

FX_NS_DEF(utility);

std::string EnvironmentImpl::getImpl(const std::string& name)
{
    DWORD len = GetEnvironmentVariable(name.c_str(), 0, 0);
    if (len == 0) return "";
    char* buffer = new char[len];
    GetEnvironmentVariable(name.c_str(), buffer, len);
    std::string result(buffer);
    delete [] buffer;
    return result;
}

bool EnvironmentImpl::hasImpl(const std::string& name)
{
    DWORD len = GetEnvironmentVariable(name.c_str(), 0, 0);
    return len > 0;
}

void EnvironmentImpl::setImpl(const std::string& name, const std::string& value)
{
    if (SetEnvironmentVariable(name.c_str(), value.c_str()) == 0)
    {
        std::string msg = "cannot set environment variable: ";
        msg.append(name);
        FIRTEX_THROW(SystemException, msg.c_str());
    }
}

std::string EnvironmentImpl::osNameImpl()
{
    OSVERSIONINFO vi;
    vi.dwOSVersionInfoSize = sizeof(vi);
    if (GetVersionEx(&vi) == 0) 
    {
        FIRTEX_THROW(SystemException, "Cannot get OS version information");
    }
    switch (vi.dwPlatformId)
    {
    case VER_PLATFORM_WIN32s:
        return "Windows 3.x";
    case VER_PLATFORM_WIN32_WINDOWS:
        return vi.dwMinorVersion == 0 ? "Windows 95" : "Windows 98";
    case VER_PLATFORM_WIN32_NT:
        return "Windows NT";
    default:
        return "Unknown";
    }
}

std::string EnvironmentImpl::osVersionImpl()
{
    OSVERSIONINFO vi;
    vi.dwOSVersionInfoSize = sizeof(vi);
    if (GetVersionEx(&vi) == 0) 
    {
        FIRTEX_THROW(SystemException, "Cannot get OS version information");
    }
    std::ostringstream str;
    str << vi.dwMajorVersion << "." << vi.dwMinorVersion 
        << " (Build " << (vi.dwBuildNumber & 0xFFFF);
    if (vi.szCSDVersion[0]) str << ": " << vi.szCSDVersion;
    str << ")";
    return str.str();
}

std::string EnvironmentImpl::osArchitectureImpl()
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    switch (si.wProcessorArchitecture)
    {
    case PROCESSOR_ARCHITECTURE_INTEL:
        return "IA32";
    case PROCESSOR_ARCHITECTURE_MIPS:
        return "MIPS";
    case PROCESSOR_ARCHITECTURE_ALPHA:
        return "ALPHA";
    case PROCESSOR_ARCHITECTURE_PPC:
        return "PPC";
    case PROCESSOR_ARCHITECTURE_IA64:
        return "IA64";
#ifdef PROCESSOR_ARCHITECTURE_IA32_ON_WIN64
    case PROCESSOR_ARCHITECTURE_IA32_ON_WIN64:
        return "IA64/32";
#endif
#ifdef PROCESSOR_ARCHITECTURE_AMD64
    case PROCESSOR_ARCHITECTURE_AMD64:
        return "AMD64";
#endif
    default:
        return "Unknown";
    }
}

std::string EnvironmentImpl::nodeNameImpl()
{
    char name[256];
    DWORD size = sizeof(name);
    if (GetComputerNameA(name, &size) == 0) 
        FIRTEX_THROW(SystemException, "Cannot get computer name");
    return std::string(name);
}

FX_NS_END
