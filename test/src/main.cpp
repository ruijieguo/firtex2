#include "cppunit/BriefTestProgressListener.h"
#include "cppunit/CompilerOutputter.h"
#include "cppunit/extensions/TestFactoryRegistry.h"
#include "cppunit/TestResult.h"
#include "cppunit/TestResultCollector.h"
#include "cppunit/TestRunner.h"
#include "firtex/common/Logger.h"
#include "firtex/config/GlobalConfig.h"
#include "firtex/config/XMLConfigurator.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/File.h"
#include "TestHelper.h"
#include <ostream>

FX_NS_USE(utility);
FX_NS_USE(common);
FX_NS_USE(config);

int main(int argc, char* argv[])
{
    File file(std::string("logger_conf.xml"));
    if (file.exists())
    {
        XMLConfigurator configurator;
        configurator.load(file.getPath());
        LoggerBuilder::instance()->configure(configurator);
    }

    Path confPath = TestHelper::getTestDataPath();
    confPath.makeDirectory();
    confPath.pushDirectory("conf");
#ifdef FX_WINDOWS
    confPath.setFileName("test_config.xml");
#else
    confPath.setFileName("test_config_posix.xml");
#endif

    XMLConfigurator configurator;
    configurator.load(confPath.toString());
    tstring str = GLOBAL_CONF().General.path;
    GLOBAL_CONF().configure(configurator);
    GLOBAL_CONF().General.path = str;
    
    // Create the event manager and test controller
    CPPUNIT_NS::TestResult controller;

    // Add a listener that colllects test result
    CPPUNIT_NS::TestResultCollector result;
    controller.addListener( &result );        

    // Add a listener that print dots as test run.
    CPPUNIT_NS::BriefTestProgressListener progress;
    controller.addListener( &progress );      

    // Add the top suite to the test runner
    CPPUNIT_NS::TestRunner runner;
    runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest() );
    runner.run( controller );

    // Print test in a compiler compatible format.
    // CPPUNIT_NS::CompilerOutputter outputter( &result, CPPUNIT_NS::stdCOut() );
    CPPUNIT_NS::CompilerOutputter outputter( &result, std::cerr);
    outputter.write(); 

    LoggerBuilder::instance()->shutdown();

    //    getchar();
    return result.wasSuccessful() ? 0 : 1;
}
