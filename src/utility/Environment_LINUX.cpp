#include "firtex/utility/Environment_LINUX.h"
#include "firtex/utility/Exception.h"
#include "firtex/thread/FastMutex.h"
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <cstring>

FX_NS_USE(thread);

FX_NS_DEF(utility)

EnvironmentImpl::StringMap EnvironmentImpl::sm_map;
FastMutex EnvironmentImpl::sm_mutex;

std::string EnvironmentImpl::getImpl(const std::string& name)
{
    FastMutex::Guard lock(sm_mutex);
	
    const char* val = getenv(name.c_str());
    if (val)
        return std::string(val);
    else
        return "";
}

bool EnvironmentImpl::hasImpl(const std::string& name)
{
    FastMutex::Guard lock(sm_mutex);

    return getenv(name.c_str()) != 0;
}

void EnvironmentImpl::setImpl(const std::string& name, const std::string& value)
{
    FastMutex::Guard lock(sm_mutex);
	
    std::string var = name;
    var.append("=");
    var.append(value);
    sm_map[name] = var;
    if (putenv((char*) sm_map[name].c_str()))
    {
        std::string msg = "cannot set environment variable: ";
        msg.append(name);
        FIRTEX_THROW(SystemException, "%s", msg.c_str());
    }
}

std::string EnvironmentImpl::osNameImpl()
{
    struct utsname uts;
    uname(&uts);
    return uts.sysname;
}

std::string EnvironmentImpl::osVersionImpl()
{
    struct utsname uts;
    uname(&uts);
    return uts.release;
}

std::string EnvironmentImpl::osArchitectureImpl()
{
    struct utsname uts;
    uname(&uts);
    return uts.machine;
}

std::string EnvironmentImpl::nodeNameImpl()
{
    struct utsname uts;
    uname(&uts);
    return uts.nodename;
}

FX_NS_END
