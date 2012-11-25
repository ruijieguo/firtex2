#ifndef __ENVIRONMENT_LINUX_IMPL_H
#define __ENVIRONMENT_LINUX_IMPL_H

#include "firtex/common/StdHeader.h"
#include "firtex/thread/FastMutex.h"
#include <map>

FX_NS_DEF(utility)

class EnvironmentImpl
{
public:
    static std::string getImpl(const std::string& name);	
    static bool hasImpl(const std::string& name);	
    static void setImpl(const std::string& name, const std::string& value);
    static std::string osNameImpl();
    static std::string osVersionImpl();
    static std::string osArchitectureImpl();
    static std::string nodeNameImpl();
private:
    typedef std::map<std::string, std::string> StringMap;
	
    static StringMap sm_map;
    static FX_NS(thread)::FastMutex sm_mutex;
};

FX_NS_END

#endif
