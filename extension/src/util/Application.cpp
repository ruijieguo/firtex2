#include <string>
#include "firtex/extension/util/Application.h"
#include "firtex/extension/util/OptionException.h"
#include "firtex/extension/util/SharedOptionCallback.h"
#include "firtex/extension/util/HelpDisplayAppRunner.h"

using namespace std;

FX_NS_DEF(util);

Application::Application(const string& sProgName, const string& sDesc)
    : m_sProgName(sProgName)
{
    m_pOptionParser = new OptionParser();
    string sUsage = "Usage: ";
    sUsage += sProgName;
    sUsage += " [OPTION]...";
    m_pOptionParser->addDescription(sUsage);
    m_pOptionParser->addDescription(sDesc);

    addAppRunner(new HelpDisplayAppRunner);
}

Application::~Application()
{
    for (RunnerVect::iterator it = m_runners.begin(); 
         it != m_runners.end(); it++)
    {
        delete *it;
    }
    m_runners.clear();

    delete m_pOptionParser;
    m_pOptionParser = NULL;
}

Application::Status Application::run()
{
    Status s = AppRunner::S_EXIT;
    bool run = false;
    for (RunnerVect::const_iterator it = m_runners.begin(); 
         it != m_runners.end(); it++)
    {
        s = (*it)->run();
        if (s != AppRunner::S_NONE)
        {
            run = true;
            break;
        }
    }
    if (!run)
    {
        m_pOptionParser->displayHelp();
    }
    return s;
}

void Application::addAppRunner(AppRunner* pRunner)
{
    try
    {
        pRunner->registerOption(this);
    }
    catch(const OptionException& oe)
    {
        cout << "Register option FAILED:" << oe.what() << endl;
    }
    m_runners.push_back(pRunner);
}

bool Application::parse(int argc, const char* argv[])
{
    if (argc <= 1)
    {
        m_pOptionParser->displayHelp();
        return false;
    }
    try
    {
        m_pOptionParser->parse(argc, argv);
        return true;
    }
    catch(const OptionException& oe)
    {
        cout << oe.what() << endl;
    }
    return false;
}

void Application::addOption(const FX_NS(util)::Option& opt) 
{
    try 
    {
        const Option& opt2 = m_pOptionParser->getOption(string("--") + opt.fullName());
        AbstractOptionCallback* pCallback = opt2.callback();
        SharedOptionCallback* pSharedCallback = dynamic_cast<SharedOptionCallback*>(pCallback);
        pSharedCallback->addCallback(opt.callback());
    }
    catch (const UnknownOptionException& )
    {
        m_pOptionParser->addOption(opt);
    }
}

void Application::addDescription(const string& sDesc)
{
    m_pOptionParser->addDescription(sDesc);
}

void Application::stop()
{
    for (RunnerVect::const_iterator it = m_runners.begin(); 
         it != m_runners.end(); it++)
    {
        (*it)->stop();
    }
}

void Application::join()
{
    for (RunnerVect::const_iterator it = m_runners.begin(); 
         it != m_runners.end(); it++)
    {
        (*it)->waitStop();
    }
}


FX_NS_END
