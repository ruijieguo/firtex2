#
# $Id: Makefile.am,v 1.44 2005/06/14 21:28:46 blep Exp $
#
cppunit_srcdir = $(top_srcdir)/test/cppunit/src/cppunit

EXTRA_DIST += \
	$(cppunit_srcdir)/cppunit.dsp\
	$(cppunit_srcdir)/cppunit_dll.dsp\
	$(cppunit_srcdir)/DllMain.cpp


libcppunit_la_SOURCES += \
	$(cppunit_srcdir)/AdditionalMessage.cpp\
	$(cppunit_srcdir)/Asserter.cpp\
	$(cppunit_srcdir)/BeOsDynamicLibraryManager.cpp\
	$(cppunit_srcdir)/BriefTestProgressListener.cpp\
	$(cppunit_srcdir)/CompilerOutputter.cpp\
	$(cppunit_srcdir)/DefaultProtector.h\
	$(cppunit_srcdir)/DefaultProtector.cpp\
	$(cppunit_srcdir)/DynamicLibraryManager.cpp\
	$(cppunit_srcdir)/DynamicLibraryManagerException.cpp\
	$(cppunit_srcdir)/Exception.cpp\
	$(cppunit_srcdir)/Message.cpp\
	$(cppunit_srcdir)/RepeatedTest.cpp\
	$(cppunit_srcdir)/PlugInManager.cpp\
	$(cppunit_srcdir)/PlugInParameters.cpp\
	$(cppunit_srcdir)/Protector.cpp\
	$(cppunit_srcdir)/ProtectorChain.h\
	$(cppunit_srcdir)/ProtectorContext.h\
	$(cppunit_srcdir)/ProtectorChain.cpp\
	$(cppunit_srcdir)/SourceLine.cpp\
	$(cppunit_srcdir)/StringTools.cpp\
	$(cppunit_srcdir)/SynchronizedObject.cpp\
	$(cppunit_srcdir)/Test.cpp\
	$(cppunit_srcdir)/TestAssert.cpp\
	$(cppunit_srcdir)/TestCase.cpp\
	$(cppunit_srcdir)/TestCaseDecorator.cpp\
	$(cppunit_srcdir)/TestComposite.cpp\
	$(cppunit_srcdir)/TestDecorator.cpp\
	$(cppunit_srcdir)/TestFactoryRegistry.cpp\
	$(cppunit_srcdir)/TestFailure.cpp\
	$(cppunit_srcdir)/TestLeaf.cpp\
	$(cppunit_srcdir)/TestNamer.cpp\
	$(cppunit_srcdir)/TestPath.cpp\
	$(cppunit_srcdir)/TestPlugInDefaultImpl.cpp\
	$(cppunit_srcdir)/TestResult.cpp\
	$(cppunit_srcdir)/TestResultCollector.cpp\
	$(cppunit_srcdir)/TestRunner.cpp\
	$(cppunit_srcdir)/TestSetUp.cpp\
	$(cppunit_srcdir)/TestSuccessListener.cpp\
	$(cppunit_srcdir)/TestSuite.cpp\
	$(cppunit_srcdir)/TestSuiteBuilderContext.cpp\
	$(cppunit_srcdir)/TextOutputter.cpp\
	$(cppunit_srcdir)/TextTestProgressListener.cpp\
	$(cppunit_srcdir)/TextTestResult.cpp\
	$(cppunit_srcdir)/TextTestRunner.cpp\
	$(cppunit_srcdir)/TypeInfoHelper.cpp\
	$(cppunit_srcdir)/UnixDynamicLibraryManager.cpp\
	$(cppunit_srcdir)/ShlDynamicLibraryManager.cpp\
	$(cppunit_srcdir)/XmlDocument.cpp\
	$(cppunit_srcdir)/XmlElement.cpp\
	$(cppunit_srcdir)/XmlOutputter.cpp\
	$(cppunit_srcdir)/XmlOutputterHook.cpp\
	$(cppunit_srcdir)/Win32DynamicLibraryManager.cpp

#libcppunit_la_LDFLAGS= 
# -no-undefined -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) 
# -release $(LT_RELEASE)

