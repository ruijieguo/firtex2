#!/usr/bin/env python

import sys
import os
import string
import time

def getNamespace(path):
        path = path.replace('\\', '/')
        if 'src/extension' in path:
                LLL = path[path.find('src/extension/') + len('src/extension/') : ].split("/")
                if not LLL or not LLL[0]: 
                        return "NONE"
                return LLL[0]
        if 'src' in path:
                LLL = path[path.find('src/') + len('src/') : ].split("/")
                if not LLL or not LLL[0]: 
                        return "NONE"
                return LLL[0]
        if 'include/firtex/extension' in path:
                LLL = path[path.find('include/firtex/extension/') + len('include/firtex/extension/') : ].split("/")
                if not LLL or not LLL[0]: 
                        return "NONE"
                return LLL[0]                
        if 'include/firtex' in path:
                LLL = path[path.find('include/firtex/') + len('include/firtex/') : ].split("/")
                if not LLL or not LLL[0]: 
                        return "NONE"
                return LLL[0]                
        if 'app/common' in path:
                return "common"
        if 'app' in path:
                return "app"
        else:
                return "NONE"

def getHeaderPath(path):
        if 'include/firtex/extension/' in path:
                return path[:path.find('include/firtex/extension/') + len('include/firtex/extension/')] + getNamespace(path) + "/"
        if 'include/firtex/' in path:
                return path[:path.find('include/firtex/') + len('include/firtex/')] + getNamespace(path) + "/"
        if 'extension/src/' in path:
                return path[:path.find('extension/src/')] + "extension/include/firtex/extension/" + getNamespace(path) + "/"
        if 'src/' in path:
                return path[:path.find('src/')] + "include/firtex/" + getNamespace(path) + "/"
        if 'app/common' in path:
                return path + '/'
        if 'app/' in path:
                return path + '/'

def getSrcPath(path):
        if 'include/firtex/extension/' in path:
                return path[:path.find('include/firtex/extension/')] + "src/" + getNamespace(path) + "/"
        if 'include/firtex/' in path:
                return path[:path.find('include/firtex/')] + "src/" + getNamespace(path) + "/"
        if 'src/extension/' in path:
                return path[:path.find('src/extension/') + len('src/extension/')] + getNamespace(path) + "/"
        if 'src/' in path:
                return path[:path.find('src/') + len('src/')] + getNamespace(path) + "/"
        if 'app/common' in path:
                return path + '/'
        if 'app/' in path:
                return path + '/'

def createClassH(path, classname):
	ns = getNamespace(path) 
	if not ns:
                print ("Error: get namespace FAILED.")

	context = ""
        context = context + "//" + "\n"
        context = context + "// Copyright(C) 2005--2011 FirteX Development Team. " + "\n"
        context = context + "// All rights reserved." + "\n"
        context = context + "// This file is part of FirteX (http://sourceforge.net/projects/firtex)" + "\n"
        context = context + "//" + "\n"
        context = context + "// Use of the FirteX is subject to the terms of the software license set forth in " + "\n"
        context = context + "// the LICENSE file included with this software.\n"
        context = context + "//" + "\n"
        context = context + "// Author  : Ruijie Guo" + "\n"
        context = context + "// Email   : ruijieguo@gmail.com" + "\n"
        context = context + "// Created : "
        context = context + time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()) + "\n\n"

	context = context + "#ifndef __FX_" + classname.upper() + "_H" + "\n"
	context = context + "#define __FX_" + classname.upper() + "_H" + "\n"
	context = context + "" + "\n"
	context = context + "#include \"firtex/common/StdHeader.h\"" + "\n"
	context = context + "#include \"firtex/common/Logger.h\"" + "\n"
	context = context + "#include \"firtex/common/SharedPtr.h\"" + "\n"
	context = context + "" + "\n"
	context = context + "FX_NS_DEF(" + ns + ");" + "\n"
	context = context + "" + "\n"
	context = context + "class " + classname + "\n"
	context = context + "{" + "\n"
	context = context + "public:" + "\n"
	context = context + "    " + classname + "();" + "\n"
	context = context + "    ~" + classname + "();" + "\n\n"
	context = context + "public:" + "\n\n"
	context = context + "protected:" + "\n\n"
	context = context + "private:" + "\n"
	context = context + "    " + "DECLARE_LOGGER();" + "\n"
	context = context + "};" + "\n"
	context = context + "" + "\n"
	context = context + "FX_NS_END" + "\n"
	context = context + "" + "\n"
	context = context + "#endif //__FX_" + classname.upper() + "_H" + "\n"
        headerFile = getHeaderPath(path) + classname + ".h"

	if os.path.exists(headerFile):
		print (headerFile + " exists!") 
	else:
		f = open(headerFile, "w")
		f.write(context)
		f.close()
		print ("create header file " + headerFile + " succeed.")

def createClassCPP(path, classname):
	ns = getNamespace(path)
	if not ns:
                print ("Error: get namespace FAILED.")

	context = ""
	context = context + "#include \"firtex/" + ns + "/" + classname + ".h\"" + "\n"
	context = context + "" + "\n"
	context = context + "FX_NS_DEF(" + ns + ");" + "\n\n"
	context = context + "SETUP_LOGGER(" + ns + ", " + classname + ");" + "\n"
	context = context + "" + "\n"
	context = context + classname + "::" + classname + "() " + "\n"
	context = context + "{" + "\n"
	context = context + "}" + "\n"
	context = context + "" + "\n"
	context = context + classname + "::~" + classname + "() " + "\n"
	context = context + "{" + "\n"
	context = context + "}" + "\n"
	context = context + "" + "\n"
	context = context + "FX_NS_END" + "\n"
	context = context + "" + "\n"
        srcFile = getSrcPath(path) + classname + ".cpp" 

	if os.path.exists(srcFile):
		print (srcFile + " exists!") 
	else:
		f = open(srcFile, "w")
		f.write(context)
		f.close()
		print ("create source file " + srcFile + " succeed.")


def createClass(path, classname, hasCpp):
	createClassH(path, classname)
        if hasCpp == 1:
		createClassCPP(path, classname)

if __name__ == "__main__":
	if len(sys.argv) < 2:
		print >> sys.stderr, "Usage:", sys.argv[0], "<classname>", "[--without-cpp]"
		sys.exit(1)
	path = os.getcwd()
        hasCpp = 1
        if len(sys.argv) > 2:
                if sys.argv[2] == "--without-cpp":
                        hasCpp = 0
	createClass(path, sys.argv[1], hasCpp)
