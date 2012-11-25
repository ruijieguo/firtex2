#!/usr/bin/env python

import sys
import os
import string

def getNamespace(path):
        path = path.replace('\\', '/')
        if 'src/' in path:
                LLL = path[path.find('src/') + len('src/') : ].split("/")
                if not LLL or not LLL[0]: 
                        return "NONE"
                return LLL[0]
        else:
                return "NONE"
		

def createTestClassH(path, classname):
	ns = getNamespace(path) 
	if not ns:
                return False

	context = ""
	context = context + "#ifndef __FX_" + classname.upper() + "_H" + "\n"
	context = context + "#define __FX_" + classname.upper() + "_H" + "\n"
	context = context + "" + "\n"
	context = context + "#include \"firtex/common/StdHeader.h\"" + "\n"
	context = context + "#include \"firtex/common/Logger.h\"" + "\n"
	context = context + "#include \"firtex/common/SharedPtr.h\"" + "\n"
	context = context + "#include \"cppunit/extensions/HelperMacros.h\"" + "\n"
	context = context + "" + "\n"
	context = context + "FX_NS_DEF(" + ns + ");" + "\n"
	context = context + "" + "\n"
	context = context + "class " + classname + " : public CPPUNIT_NS::TestFixture\n"
	context = context + "{" + "\n"
	context = context + "    " + "CPPUNIT_TEST_SUITE(" + classname + ");" + "\n"
	context = context + "    " + "CPPUNIT_TEST(" + "testXXX" + ");" + "\n"
	context = context + "    " + "CPPUNIT_TEST_SUITE_END();" + "\n"
	context = context + "public:" + "\n"
	context = context + "    " + classname + "();" + "\n"
	context = context + "    ~" + classname + "();" + "\n\n"
	context = context + "    " + "void setUp();" + "\n"
	context = context + "    " + "void tearDown();" + "\n"
	context = context + "" + "\n"
	context = context + "protected:" + "\n"
	context = context + "    " + "void testXXX();" + "\n\n"
	context = context + "private:" + "\n"
	context = context + "    " + "DECLARE_LOGGER();" + "\n"
	context = context + "};" + "\n"
	context = context + "" + "\n"
	context = context + "FX_NS_END" + "\n"
	context = context + "" + "\n"
	context = context + "#endif //__FX_" + classname.upper() + "_H" + "\n"
	headerFile = path + "/" + classname + ".h";
	if os.path.exists(headerFile):
		print (headerFile + " exists!") 
		return False
	else:
		f = open(headerFile, "w")
		f.write(context)
		f.close()
		print ("Create " + headerFile + " success!") 
		return True

def createTestClassCPP(path, classname):
	ns = getNamespace(path)
	if not ns:
		return False
	context = ""
	context = context + "#include \"" + ns + "/" + classname + ".h\"" + "\n"
	context = context + "" + "\n"
	context = context + "using namespace std;" + "\n\n"
	context = context + "FX_NS_DEF(" + ns + ");" + "\n\n"
	context = context + "SETUP_LOGGER(" + ns + ", " + classname + ");" + "\n"
	context = context + "CPPUNIT_TEST_SUITE_REGISTRATION(" + classname + ");" + "\n"
	context = context + "" + "\n"
	context = context + classname + "::" + classname + "() " + "\n"
	context = context + "{" + "\n"
	context = context + "}" + "\n"
	context = context + "" + "\n"
	context = context + classname + "::~" + classname + "() " + "\n"
	context = context + "{" + "\n"
	context = context + "}" + "\n"
	context = context + "" + "\n"
	context = context + "void " + classname + "::" + "setUp()" + "\n"
	context = context + "{" + "\n"
	context = context + "}" + "\n"
	context = context + "" + "\n"
	context = context + "void " + classname + "::" + "tearDown()" + "\n"
	context = context + "{" + "\n"
	context = context + "}" + "\n"
	context = context + "" + "\n"
	context = context + "void " + classname + "::" + "testXXX()" + "\n"
	context = context + "{" + "\n"
	context = context + "}" + "\n"
	context = context + "" + "\n"
	context = context + "FX_NS_END" + "\n"
	context = context + "" + "\n"
	srcFile = path + "/" + classname + ".cpp"
	if os.path.exists(srcFile):
		print (srcFile + " exists!") 
		return False
	else:
		f = open(srcFile, "w")
		f.write(context)
		f.close()
		print ("Create " + srcFile + " success!") 
		return True

def createTestClass(path, classname):
	ret = 0;
	if createTestClassH(path, classname):
		ret = ret + 1
	if createTestClassCPP(path, classname):
		ret = ret + 1
	return ret

if __name__ == "__main__":
	if len(sys.argv) != 2:
		print >> sys.stderr, "Usage:", sys.argv[0], "<classname>"
		sys.exit(1)
	path = os.getcwd()
	if createTestClass(path, sys.argv[1]) == 2:
		pass
