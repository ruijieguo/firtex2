#include "LoggerTestCase.h"
#include "firtex/common/Logger.h"
#include "firtex/common/FileAppender.h"
#include "firtex/config/Configurator.h"
#include "firtex/config/XMLConfigurator.h"
#include <memory>
#include <sstream>
#include "TestHelper.h"

using namespace std;

FX_NS_USE(common);
FX_NS_USE(config);
FX_NS_USE(utility);

FX_NS_DEF(common);

CPPUNIT_TEST_SUITE_REGISTRATION(LoggerTestCase);

class FakeAppender : public Appender
{
public:
    FakeAppender()
    {
    }

    virtual void append(const LoggingEvent& event)
    {
        m_sLogText = event.message;
    }

    void flush()
    {
        m_sLogText.clear();
    }

    void configure(Configurator& conf) {}

    string getIdentifier() const { return "apender.fake";}

    bool hasLogged() const {return !m_sLogText.empty();}
    bool hasFlushed() const {return m_sLogText.empty();}
    string getLogText() const {return m_sLogText;}
private:
    string m_sLogText;
};

LoggerTestCase::LoggerTestCase(void)
{
}

LoggerTestCase::~LoggerTestCase(void)
{
}

void LoggerTestCase::setUp()
{
}

void LoggerTestCase::tearDown()
{
}

string toXML(const Configurator::ConfMap& confMap)
{
    stringstream ss;
    for (Configurator::ConfMap::const_iterator it = confMap.begin();
         it != confMap.end(); ++it)
    {
        ss << "<" << it->first << ">";
        if (it->second.getType() == typeid(string))
        {
            ss << AnyCast<string>(it->second);
        }
        else if (it->second.getType() == typeid(Configurator::ConfMap))
        {
            Configurator::ConfMap confMap2 = AnyCast<Configurator::ConfMap>(it->second);
            ss << toXML(confMap2);
        }
        // else if (it->second.getType() == typeid(multimap<string, Any>))
        // {
        //     multimap<string, Any> confMap2 = AnyCast<multimap<string, Any> >(it->second);
        //     ss << toXML(confMap2);
        // }
        ss << "</" << it->first << ">";
    }
    return ss.str();
}

void LoggerTestCase::testGetLogger()
{
    Logger& gLog = LoggerBuilder::instance()->getLogger("child.grandchild");
    CPPUNIT_ASSERT(&gLog != NULL);
    LoggerPtr rootLog = LoggerBuilder::instance()->hasLogger("/");
    CPPUNIT_ASSERT(rootLog != NULL);
    Logger& rootLog2 = LoggerBuilder::instance()->parentLogger("child");
    CPPUNIT_ASSERT(rootLog == &rootLog2);

    Logger& cLog = LoggerBuilder::instance()->getLogger("child");
    Logger& rootLog4 = LoggerBuilder::instance()->parentLogger("child.grandchild");
    CPPUNIT_ASSERT(&cLog == &rootLog4);
}

void LoggerTestCase::testLog()
{
    Logger& log = LoggerBuilder::instance()->getLogger("testlogger");
    FakeAppender* fakeAppender = new FakeAppender;
    log.setAppender(fakeAppender);

    CPPUNIT_ASSERT(!fakeAppender->hasLogged());
    log.log(LoggingLevel::LEVEL_INFO, "test log");
    CPPUNIT_ASSERT(fakeAppender->hasLogged());

    CPPUNIT_ASSERT(!fakeAppender->hasFlushed());
    log.flush();
    CPPUNIT_ASSERT(fakeAppender->hasFlushed());
}

void LoggerTestCase::testConfigureLog()
{
    LoggerBuilder logBuilder;
    Logger& gLog1 = logBuilder.getLogger("child.grandchild1");
    CPPUNIT_ASSERT(&gLog1 != NULL);

    Logger& gLog2 = logBuilder.getLogger("child2.grandchild2");
    CPPUNIT_ASSERT(&gLog2 != NULL);
    gLog2.setLevel(LoggingLevel::LEVEL_TRACE);
    FileAppenderPtr pApp = new FileAppender();
    logBuilder.setAppender("child2.grandchild2", pApp);

    Configurator conf;
    logBuilder.configure(conf);
    string str = toXML(conf.getMap());
    string strExp = "<loggers><logger><name>/</name><appender>appender.console</appender><flush>false</flush>"
                    "<level>INFO</level></logger><logger><name>child.grandchild1</name>"
                    "<appender>appender.console</appender><flush>false</flush><level>INFO</level></logger>"
                    "<logger><name>child2.grandchild2</name><appender>appender.file</appender>"
                    "<path></path><secondary_path></secondary_path><rotation></rotation>"
                    "<flush>false</flush><level>TRACE</level></logger></loggers>";
    CPPUNIT_ASSERT_EQUAL(strExp, str);
    string sTestConf = TestHelper::getTestDataPath(_T("conf/logger_conf_test.xml"), false);
    XMLConfigurator xmlConf;
    xmlConf.load(sTestConf);

    LoggerBuilder logBuilder2;
    try
    {
        logBuilder2.configure(xmlConf);
    }
    catch(const FirteXException& e)
    {
        cout << "===ERROR: " << e.what() << endl;
        CPPUNIT_ASSERT(false);
    }
    LoggerPtr rootLog = logBuilder2.hasLogger("/");
    CPPUNIT_ASSERT(rootLog != NULL);
    CPPUNIT_ASSERT(rootLog->getLevel() == LoggingLevel::LEVEL_INFO);

    LoggerPtr log1 = logBuilder2.hasLogger("child.grandchild1");
    CPPUNIT_ASSERT(log1 != NULL);    
    CPPUNIT_ASSERT(log1->getLevel() == LoggingLevel::LEVEL_INFO);
    CPPUNIT_ASSERT_EQUAL(string("appender.console"), log1->getAppender()->getIdentifier());

    LoggerPtr log2 = logBuilder2.hasLogger("child2.grandchild2");
    CPPUNIT_ASSERT(log2 != NULL);
    CPPUNIT_ASSERT(log2->getLevel() == LoggingLevel::LEVEL_TRACE);
    CPPUNIT_ASSERT_EQUAL(string("appender.file"), log2->getAppender()->getIdentifier());

    LoggerPtr log3 = logBuilder2.hasLogger("child3");
    CPPUNIT_ASSERT(log3 != NULL);    
    CPPUNIT_ASSERT(log3->getLevel() == LoggingLevel::LEVEL_TRACE);

    AppenderPtr pApp3 = log3->getAppender();
    CPPUNIT_ASSERT(pApp3);
    
    FileAppenderPtr pFileApp3 = pApp3.cast<FileAppender>();
    CPPUNIT_ASSERT(pFileApp3);
    CPPUNIT_ASSERT_EQUAL(string("myAppenderPath"), pFileApp3->getPath());

    LoggerPtr log4 = logBuilder2.hasLogger("child3.grandchild3");
    CPPUNIT_ASSERT(log4 != NULL);    
    CPPUNIT_ASSERT(log4->getLevel() == LoggingLevel::LEVEL_DEBUG);

    AppenderPtr pApp4 = log4->getAppender();
    CPPUNIT_ASSERT(pApp4);
    CPPUNIT_ASSERT(pApp4 == pApp3);
}


FX_NS_END
