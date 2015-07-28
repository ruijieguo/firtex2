#include "firtex/common/Logger.h"
#include "firtex/common/ConsoleAppender.h"
#include "firtex/common/FileAppender.h"
#include "firtex/utility/Exception.h"
#include "firtex/utility/NumberFormatter.h"
#include "firtex/config/XMLConfigurator.h"
#include "firtex/thread/ScopedUnlock.h"
#include "firtex/common/Appender.h"
#include "firtex/common/AppenderFactory.h"
#include "firtex/common/LoggingLayoutFactory.h"
#include <string>
#include <assert.h>

using namespace std;
FX_NS_USE(utility);
FX_NS_USE(config);
FX_NS_USE(thread);
FX_NS_DEF(common);

LoggerBase::LoggerBase(const string& name,
                       const AppenderPtr& pAppender, LevelType level)
    : m_sName(name)
    , m_pAppender(pAppender)
    , m_level(level)
{
}

LoggerBase::LoggerBase()
    : m_level(LoggingLevel::LEVEL_INFO)
{
}

LoggerBase::~LoggerBase()
{
}

void LoggerBase::setLevel(const string& level)
{
    setLevel(LoggingLevel::strToLevel(level));
}

void LoggerBase::log(const FirteXException& exc)
{
    error(exc.what());
}

void LoggerBase::flush()
{
    if (!m_pAppender)
    {
        m_pAppender->flush();
    }
}

void Logger::configure(Configurator& conf)
{
    string sName = getName();
    conf.configure("name", sName);

    if (conf.getMode() == Configurator::TO_CONF)
    {
        if (m_pAppender)
        {
            string sApp = m_pAppender->getIdentifier();
            conf.configure("appender", sApp);
            m_pAppender->configure(conf);
        }
    
        string str = LoggingLevel::levelToStr(getLevel());
        conf.configure("level", str);
    }
    else
    {
        m_sName = sName;
        string sApp;
        conf.configure("appender", sApp, "");
        if (!sApp.empty())
        {
            AppenderPtr pAppender = m_loggerBuilder.findAppenderUnsafe(sApp);
            m_loggerBuilder.setAppenderUnsafe(getName(), pAppender);
        }

        string str;
        conf.configure("level", str);
        m_loggerBuilder.setLevelUnsafe(getName(), str);
        setLevel(str);
    }
}

////////////////////////////////////////////////////////////
///LoggerBuilder

LoggerBuilder::LoggerBuilder()
{
}

LoggerBuilder::~LoggerBuilder()
{
    shutdown();
}

void LoggerBuilder::configure(const std::string& sConfFile)
{
    XMLConfigurator configurator;
    configurator.load(sConfFile);
    configure(configurator);
}

void LoggerBuilder::configure(Configurator& conf)
{
    FastMutex::Guard lock(m_mapMtx);

    if (conf.getMode() == Configurator::TO_CONF)
    {
        Configurator loggersConf;
        for (LoggerMap::iterator it = m_loggerMap.begin();
             it != m_loggerMap.end(); ++it)
        {
            Configurator loggerConf;
            it->second->configure(loggerConf);
            loggersConf.configure("logger", loggerConf.getMap());
        }        
        conf.configure("loggers", loggersConf.getMap());
    }
    else
    {
        Configurator::Iterator appIt = conf.findConf("appenders");
        if (appIt.hasNext())
        {
            Configurator::KeyValuePair kv = appIt.next();
            Configurator::ConfMap appConfMap 
                = AnyCast<Configurator::ConfMap>(kv.second);
            Configurator appConf(appConfMap);
            configureAppender(appConf);
        }

        Configurator::Iterator loggerIt = conf.findConf("loggers");
        if (loggerIt.hasNext())
        {
            Configurator::KeyValuePair kv = loggerIt.next();
            Configurator::ConfMap logConfMap 
                = AnyCast<Configurator::ConfMap>(kv.second);
            Configurator logConf(logConfMap);
            configureLogger(logConf);
        }
                                    
    }
}

void LoggerBuilder::configureAppender(const Configurator& conf)
{
    Configurator::Iterator it2 = conf.iterator();
    while (it2.hasNext())
    {
        Configurator::KeyValuePair kv = it2.next();
        if (!strCompareNoCase(kv.first.c_str(), "appender"))
        {
            Configurator::ConfMap appConfMap 
                = AnyCast<Configurator::ConfMap>(kv.second);
            Configurator appConf(appConfMap);
                    
            string sAppName;
            Configurator::Iterator it3 = appConf.findConf("name");
            if (it3.hasNext())
            {
                sAppName = AnyCast<string>(it3.next().second);
                if (m_appenderMap.find(sAppName) != m_appenderMap.end())
                {
                    FIRTEX_THROW(BadParameterException, "Appender duplicated: [%s].",
                            sAppName.c_str());
                }

                Configurator::Iterator it4 = appConf.findConf("base");
                if (!it4.hasNext())
                {
                    FIRTEX_THROW(BadParameterException, "Base appender not defined: [%s].",
                            sAppName.c_str());
                }
                string sAppBase = AnyCast<string>(it4.next().second);
                AppenderPtr pApp;
                pApp.reset(AppenderFactory::instance()->createAppender(sAppBase));
                if (!pApp)
                {
                    FIRTEX_THROW(BadParameterException, "Invalid base appender name: [%s].",
                            sAppBase.c_str());
                }
                
                Configurator::Iterator it5 = appConf.findConf("layout");
                if (it5.hasNext())
                {
                    LayoutAppenderPtr pLayoutApp = std::dynamic_pointer_cast<LayoutAppender>(pApp);
                    if (!pLayoutApp)
                    {
                        FIRTEX_THROW(BadParameterException, "[%s] is not a layout appender",
                                sAppBase.c_str());
                    }
                    string sLayoutName = AnyCast<string>(it5.next().second);
                    LoggingLayout* pLayout = LoggingLayoutFactory::instance()->createLoggingLayout(sLayoutName);
                    if (!pLayout)
                    {
                        FIRTEX_THROW(BadParameterException, "Invalid layout name: [%s].",
                                sLayoutName.c_str());
                    }
                    
                    pLayout->configure(appConf);
                    pLayoutApp->setLayout(pLayout);
                }

                pApp->configure(appConf);
                m_appenderMap.insert(make_pair(sAppName, pApp));
            }
        }
    }
}

void LoggerBuilder::configureLogger(const Configurator& conf)
{
    Configurator::Iterator it2 = conf.iterator();
    while (it2.hasNext())
    {
        Configurator::KeyValuePair kv = it2.next();
        if (!strCompareNoCase(kv.first.c_str(), "logger"))
        {
            Configurator::ConfMap loggerConfMap 
                = AnyCast<Configurator::ConfMap>(kv.second);
            Configurator loggerConf(loggerConfMap);
                    
            Configurator::Iterator it3 = loggerConf.findConf("name");
            if (it3.hasNext())
            {
                string sLoggerName = AnyCast<string>(it3.next().second);
                Logger& logger = getLoggerUnsafe(sLoggerName);
                logger.configure(loggerConf);
            }
        }
    }
}

AppenderPtr LoggerBuilder::findAppender(const string& sAppName)
{
    FastMutex::Guard lock(m_mapMtx);
    return findAppenderUnsafe(sAppName);
}

AppenderPtr LoggerBuilder::findAppenderUnsafe(const string& sAppName)
{
    AppenderMap::const_iterator it = m_appenderMap.find(sAppName);
    if (it != m_appenderMap.end())
    {
        return it->second;
    }
    
    AppenderPtr pApp;
    pApp.reset(AppenderFactory::instance()->createAppender(sAppName));
    if (!pApp)
    {
        FIRTEX_THROW(BadParameterException, "Invalid appender name: [%s]", 
                     sAppName.c_str());
    }
    
    m_appenderMap.insert(make_pair(sAppName, pApp));
    return pApp;
}

void LoggerBuilder::setLevel(const string& name, LoggingLevel::LevelType level)
{
    FastMutex::Guard lock(m_mapMtx);

    string::size_type len = name.length();
    for (LoggerMap::iterator it = m_loggerMap.begin(); it != m_loggerMap.end(); ++it)
    {
        if (len == 0 || name == ROOT_LOGGER ||
            (it->first.compare(0, len, name) == 0 && 
             (it->first.length() == len || it->first[len] == '.')))
        {
            it->second->setLevel(level);
        }
    }
}

void LoggerBuilder::setLevel(const string& name, const string& sLevel)
{
    FastMutex::Guard lock(m_mapMtx);
    setLevelUnsafe(name, sLevel);
}

void LoggerBuilder::setLevelUnsafe(const string& name, const string& sLevel)
{
    string::size_type len = name.length();
    for (LoggerMap::iterator it = m_loggerMap.begin(); it != m_loggerMap.end(); ++it)
    {
        if (len == 0 || name == ROOT_LOGGER ||
            (it->first.compare(0, len, name) == 0 && 
             (it->first.length() == len || it->first[len] == '.')))
        {
            it->second->setLevel(sLevel);
        }
    }
}

void LoggerBuilder::setAppender(const string& name, const AppenderPtr& pAppender)
{
    FastMutex::Guard lock(m_mapMtx);
    setAppenderUnsafe(name, pAppender);
}

void LoggerBuilder::setAppenderUnsafe(const std::string& name,
                                      const AppenderPtr& pAppender)
{
    string::size_type len = name.length();
    for (LoggerMap::iterator it = m_loggerMap.begin(); it != m_loggerMap.end(); ++it)
    {
        if (len == 0 || name == ROOT_LOGGER || 
            (it->first.compare(0, len, name) == 0 &&
             (it->first.length() == len || it->first[len] == '.')))
        {
            it->second->setAppender(pAppender);
        }
    }
}

void LoggerBuilder::setConsoleAppender(const string& name)
{
    AppenderPtr pApp(new ConsoleAppender);
    setAppender(name, pApp);
}

void LoggerBuilder::setFileAppender(const string& name, const string& sFilePath)
{
    FileAppenderPtr pAppender(new FileAppender(sFilePath));
    pAppender->open();
    setAppender(name, pAppender);
}

Logger& LoggerBuilder::getLogger(const string& name)
{
    FastMutex::Guard lock(m_mapMtx);
    return getLoggerUnsafe(name);
}

Logger& LoggerBuilder::getLoggerUnsafe(const string& name)
{
    LoggerPtr pLogger = findLogger(name);
    if (!pLogger)
    {
        if (name == ROOT_LOGGER)
        {
            AppenderPtr pConApp(new ConsoleAppender);
            pLogger.reset(new Logger(*this, name, pConApp,
                            LoggingLevel::LEVEL_INFO));
        }
        else
        {
            Logger& par = parentLoggerUnsafe(name);
            pLogger.reset(new Logger(*this, name, par.getAppender(), par.getLevel()));
        }
        addLogger(pLogger);
    }
    return *pLogger;
}

Logger& LoggerBuilder::createLogger(const string& name, const AppenderPtr& pAppender,
                                    LoggingLevel::LevelType level)
{
    FastMutex::Guard lock(m_mapMtx);

    LoggerPtr pLogger = findLogger(name);
    if (pLogger) 
    {
        FIRTEX_THROW(ExistsException, "Logger: [%s] exists", name.c_str());
    }
    pLogger.reset(new Logger(*this, name, pAppender, level));
    addLogger(pLogger);
    return *pLogger;
}

Logger& LoggerBuilder::rootLogger()
{
    FastMutex::Guard lock(m_mapMtx);

    return getLoggerUnsafe(ROOT_LOGGER);
}

Logger& LoggerBuilder::parentLogger(const string& name)
{
    FastMutex::Guard lock(m_mapMtx);
    return parentLoggerUnsafe(name);
}

LoggerPtr LoggerBuilder::hasLogger(const string& name)
{
    FastMutex::Guard lock(m_mapMtx);
    return findLogger(name);
}

void LoggerBuilder::shutdown()
{
    FastMutex::Guard lock(m_mapMtx);

    m_loggerMap.clear();
}

LoggerPtr LoggerBuilder::findLogger(const string& name)
{
    LoggerMap::iterator it = m_loggerMap.find(name);
    if (it != m_loggerMap.end())
    {
        return it->second;
    }

    return NULL;
}

void LoggerBuilder::addLogger(LoggerPtr& pLogger)
{
    LoggerMap::iterator it = m_loggerMap.find(pLogger->getName());
    if (it != m_loggerMap.end())
    {
        m_loggerMap.erase(it);
    }
    m_loggerMap.insert(LoggerMap::value_type(pLogger->getName(), pLogger));
}

void LoggerBuilder::destroyLogger(const string& name)
{
    FastMutex::Guard lock(m_mapMtx);

    m_loggerMap.clear();
}

//static
void LoggerBuilder::flush(const string& name)
{
    FastMutex::Guard lock(m_mapMtx);

    if (!name.empty()) // flush the specified logger
    {
        LoggerMap::iterator it = m_loggerMap.find(name);
        if (it != m_loggerMap.end())
        {
            it->second->flush();
        }
    }
    else // flush all loggers
    {
        for (LoggerMap::iterator it = m_loggerMap.begin(); 
             it != m_loggerMap.end(); ++it)
        {
            it->second->flush();
        }
    }

}

void LoggerBuilder::listLoggers(std::vector<string>& names)
{
    FastMutex::Guard lock(m_mapMtx);

    names.clear();
    for (LoggerMap::const_iterator it = m_loggerMap.begin();
         it != m_loggerMap.end(); ++it)
    {
        names.push_back(it->first);
    }
}

Logger& LoggerBuilder::parentLoggerUnsafe(const string& name)
{
    string::size_type pos = name.rfind('.');
    if (pos != string::npos)
    {
        string pname = name.substr(0, pos);
        LoggerPtr pParent = findLogger(pname);
        if (pParent)
        {
            return *pParent;
        }
        else
        {
            return parentLoggerUnsafe(pname);
        }
    }
    else 
    {
        return getLoggerUnsafe(ROOT_LOGGER);
    }
}

FX_NS_END
