#ifndef __APPLICATION_H
#define __APPLICATION_H

#include "firtex/extension/util/AppRunner.h"
#include "firtex/extension/util/Option.h"
#include "firtex/extension/util/OptionParser.h"

#include <vector>

FX_NS_DEF(util);

class Application
{
public:
    typedef AppRunner::Status Status;

public:
    Application(const std::string& sProgName, const std::string& sDesc);
    virtual ~Application();

public:
    virtual Status run();

public:
    void stop();
    void join();

public:
    void addAppRunner(AppRunner* pRunner);
    bool parse(int argc, const char* argv[]);
    void addOption(const FX_NS(util)::Option& opt);
    void addDescription(const std::string& sDesc);
    FX_NS(util)::OptionParser* getOptionParser() const {return m_pOptionParser;}
    const std::string& getProgName() const {return m_sProgName;}

private:
    typedef std::vector<AppRunner*> RunnerVect;

    FX_NS(util)::OptionParser* m_pOptionParser;
    RunnerVect m_runners;
    std::string m_sProgName;
};

FX_NS_END

#endif
