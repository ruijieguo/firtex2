#ifndef __FXAPPRUNNER_H
#define __FXAPPRUNNER_H

#include "firtex/extension/util/Option.h"

FX_NS_DEF(util);

class Application;
class AppRunner
{
public:
    enum Status
    {
        S_NONE = 0,
        S_INVALID,
        S_RUNNING, 
        S_ERROR,
        S_EXIT,
    };

public:
    AppRunner() {}
    virtual ~AppRunner() {}

public:
    virtual Status run() = 0;
    virtual void registerOption(Application* pApp) = 0;
    virtual void optionCallback(const FX_NS(util)::Option& option, 
                                const std::string& sValue) = 0;
    virtual void stop() {};
    virtual void waitStop() {};
};

FX_NS_END

#endif
