//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2012-03-13 22:00:01

#ifndef __FX_PROCESS_WIN32_H
#define __FX_PROCESS_WIN32_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/common/SharedPtr.h"
#include <windows.h>
#include <vector>
#include <map>

FX_NS_DEF(util);

enum ProcessStatus
{
    PS_UNKNOWN = 0,
    PS_RUNNING,
    PS_EXIT_NORMAL,
    PS_EXIT_WITH_ERROR,
    PS_STOPPING
};

class ProcessHandle
{
public:
    typedef uint32_t PIDType;

public:
    ProcessHandle(HANDLE hProc, PIDType pid) 
        : m_hProcess(hProc), m_pid(pid) {}

    ProcessHandle(PIDType pid) 
        : m_hProcess(NULL), m_pid(pid) 
    {
        m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)pid);
        if (!m_hProcess)
        {
            FX_LOG(INFO, "Open process for: [%d] FAILED, error code: [%d]", 
                   pid, GetLastError());
        }
    }

    ~ProcessHandle() 
    {
        CloseHandle(m_hProcess);
        m_hProcess = NULL;
    }
	
    PIDType id() const { return m_pid;}

    HANDLE process() const
    {
        return m_hProcess;
    }

    int wait() const 
    {
	DWORD rc = WaitForSingleObject(m_hProcess, INFINITE);
	if (rc != WAIT_OBJECT_0)
        {
            FIRTEX_THROW(SystemException, "Wait failed for process: [%lu]", m_pid);
        }

	DWORD exitCode;
	if (GetExitCodeProcess(m_hProcess, &exitCode) == 0)
        {
            FIRTEX_THROW(SystemException, "Cannot get exit code for process", m_pid);
        }
	return exitCode;
    }
	
private:
    HANDLE m_hProcess;
    PIDType m_pid;
    
private:
    DECLARE_LOGGER();
};
DEFINE_TYPED_PTR(ProcessHandle);

class Process
{
public:
    typedef std::map<std::string, std::string> EnvMap;
    typedef ProcessHandle::PIDType PIDType;
    typedef std::vector<std::string> Args;

    static std::string STDOUT_FILE_NAME;
    static std::string STDERR_FILE_NAME;
 
public:
    Process();
    ~Process();

public:
    /// Return the process ID of the current process.
    static PIDType id();

    /**
     * Return the number of seconds spent by the
     * current process in user and kernel mode.
     */
    static void times(long& userTime, long& kernelTime);

    /**
     * Create a new process for the given command 
     * @param command the command to execute
     * @param args the arguments passed to the command on the command line.
     *
     * @return a ProcessHandle of the new process.
     * @throw SystemException if failed
     */ 
    static ProcessHandlePtr launch(const std::string& command, const Args& args);

    /**
     * Create a new process for the given command 
     * @param command the command to execute
     * @param args the arguments passed to the command on the command line.
     * @param envVariables environment variables
     * @param sWorkDir work directory of the process
     *
     * @return a ProcessHandle of the new process.
     * @throw SystemException if failed
     */ 
    static ProcessHandlePtr launch(const std::string& command, const Args& args,
                                   const EnvMap& envVariables, 
                                   const std::string& sWorkDir);

    /**
     * Wait for the process to terminate
     * @return the exit code of the process.
     */
    static int wait(const ProcessHandle& handle);

    /**
     * Kill the process specified by handle.
     * This method is preferable on Windows where process IDs
     * may be reused.
     */
    static void kill(const ProcessHandle& handle);

    /// Kill the process with the given pid.
    static void kill(PIDType pid);

    /**
     * Return process status
     */
    static ProcessStatus getProcessStatus(const ProcessHandlePtr& pProcHandle);

private:
    DECLARE_LOGGER();
};

FX_NS_END

#endif //__FX_PROCESS_WIN32_H
