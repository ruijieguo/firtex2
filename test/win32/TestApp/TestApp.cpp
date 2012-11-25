// TestApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TestApp.h"
#include "TestAppDlg.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/File.h"
#include "firtex/common/Logger.h"
#include "TestHelper.h"
#include "firtex/config/XMLConfigurator.h"
#include "firtex/config/GlobalConfig.h"
#include <cppunit/ui/mfc/TestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h>

FX_NS_USE(common);
FX_NS_USE(utility);
FX_NS_USE(config);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TestApp

BEGIN_MESSAGE_MAP(TestApp, CWinApp)
	//{{AFX_MSG_MAP(TestApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TestApp construction

TestApp::TestApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only TestApp object

TestApp theApp;

/////////////////////////////////////////////////////////////////////////////
// TestApp initialization

BOOL 
TestApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
# if _MSC_VER < 1300   // vc6
	Enable3dControls();			// Call this when using MFC in a shared DLL
# endif
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

  SetRegistryKey(_T("Local AppWizard-Generated Applications"));

  RunTests();        

  
  // Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}


void 
TestApp::RunTests()
{
    File file(tstring("logger_conf.xml"));
    if (file.exists())
    {
        XMLConfigurator configurator;
        configurator.load(file.getPath());
        LoggerBuilder::instance()->configure(configurator);
    }

    Path confPath = TestHelper::getTestDataPath();
    confPath.makeDirectory();
    confPath.pushDirectory(_T("conf"));
    confPath.setFileName(_T("test_config.xml"));

    tstring selfPath = GLOBAL_CONF().General.path;

    XMLConfigurator configurator;
    configurator.load(confPath.toString());
    GLOBAL_CONF().configure(configurator);

    GLOBAL_CONF().General.path = selfPath;

    CPPUNIT_NS::MfcUi::TestRunner runner;

    runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest() );
    
    runner.run();
}
