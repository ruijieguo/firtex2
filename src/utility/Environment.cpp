#include "firtex/common/StdHeader.h"
#include "firtex/utility/Environment.h"
#include <cstdlib>

#if defined(FX_WINDOWS)
#include "firtex/utility/Environment_WIN32.h"
#else
#include "firtex/utility/Environment_LINUX.h"
#endif

FX_NS_DEF(utility)

std::string Environment::get(const std::string& name)
{
    return EnvironmentImpl::getImpl(name);
}

std::string Environment::get(const std::string& name, const std::string& defaultValue)
{
    if (has(name))
        return get(name);
    else
        return defaultValue;
}
	
bool Environment::has(const std::string& name)
{
    return EnvironmentImpl::hasImpl(name);
}
	
void Environment::set(const std::string& name, const std::string& value)
{
    EnvironmentImpl::setImpl(name, value);
}

std::string Environment::osName()
{
    return EnvironmentImpl::osNameImpl();
}
	
std::string Environment::osVersion()
{
    return EnvironmentImpl::osVersionImpl();
}

std::string Environment::osArchitecture()
{
    return EnvironmentImpl::osArchitectureImpl();
}

std::string Environment::nodeName()
{
    return EnvironmentImpl::nodeNameImpl();
}

FX_NS_END
