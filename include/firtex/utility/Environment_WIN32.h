#ifndef __FX_ENVIRONMENT_WIN32_IMPL_H
#define __FX_ENVIRONMENT_WIN32_IMPL_H

#include "firtex/common/StdHeader.h"

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
};

FX_NS_END

#endif 
