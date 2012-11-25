#!/bin/env python
import os, tempfile, sys
import shutil
import httplib
from Process import *

class SmokeTest(object):
    def setUp(self):
        self.copyFile("../../app/fbuilder/fbuilder", "./fbuilder")
        self.copyFile("../../app/fxist4c/fxist4c", "./fxist4c")
        self.copyFile("../../app/fproxy/fproxy", "./fproxy")
        self.copyFile("../../app/fsearchd/fsearchd", "./fsearchd")
        
    def tearDown(self):
        self.rmFile("./fbuilder")
        self.rmFile("./fxist4c")
        self.rmFile("./fproxy")
        self.rmFile("./fsearchd")

    def run(self):
        self.setUp()
        process = Process()
        ret = process.run("./fbuilder -c ../../conf/fbuilder.xml")
        if ret[2]:
            print("fbuilder FAILED.")
            return False

        ret = process.run("./fxist4c -s ../../examples/data/plain_utf8_index"
                          " -e query=BODY:2 -l ../../sysdata")
        open("output.txt", "a").write(ret[0])
        if ret[2]:
            print("fxist4c FAILED.")
            return False

        self.tearDown()
        return True
    
    def copyFile(self, src, dst):
        if os.path.exists(dst):
            os.remove(dst)
        shutil.copy(src, dst);

    def rmFile(self, f):
        if os.path.exists(f):
            os.remove(f)

if __name__ == '__main__':
    print "Running smoke test."

    smokeTest = SmokeTest()
    ret = smokeTest.run();
    if ret:
        print "Smoke test PASSED."
    else :
        print "Smoke test FAILED."
