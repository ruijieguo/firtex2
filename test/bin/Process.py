#!/bin/env python
import os, tempfile, sys

class Process(object):
    def run(self, cmd):
        outFile = tempfile.NamedTemporaryFile()
        errFile = tempfile.NamedTemporaryFile()
        cmd = '%(cmd)s 1>>%(out)s 2>>%(err)s' \
            %{'cmd': cmd, 'out': outFile.name, 'err': errFile.name}
        retCode = os.system(cmd)
        stdout = outFile.read()
        stderr = errFile.read()
        outFile.close()
        errFile.close()
        return stdout, stderr, retCode

    def runInBackground(self, cmd):
        outFile = tempfile.NamedTemporaryFile()
        errFile = tempfile.NamedTemporaryFile()
        cmd = '%(cmd)s 1>%(out)s 2>%(err)s &' \
            % {'cmd': cmd, 'out': outFile, 'err': errFild}
        retCode = os.system(cmd)
        stdout = outFile.read()
        stderr = errFile.read()
        outFile.close()
        errFile.close()
        return stdout, stderr, retCode


if __name__ == '__main__':
    process = Process()
    print process.run(sys.argv[1])
