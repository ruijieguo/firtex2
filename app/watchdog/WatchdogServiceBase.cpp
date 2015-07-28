#include "WatchdogServiceBase.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/NumberFormatter.h"
#include "firtex/utility/File.h"
#include "firtex/utility/NumberParser.h"
#include "firtex/utility/Hash.h"

using namespace std;
FX_NS_USE(thread);
FX_NS_USE(util);
FX_NS_USE(utility);


FX_NS_DEF(app);

#define IF_NULL_THEN_RETURN(node, tag)                                  \
    if (!node)                                                          \
    {                                                                   \
        FX_LOG(ERROR, "Missing " tag " tag in result string.");           \
        return false;                                                   \
    }


SETUP_LOGGER(app, WatchdogServiceBase);

WatchdogServiceBase::WatchdogServiceBase() 
    : m_bStop(false)
{
}

WatchdogServiceBase::~WatchdogServiceBase() 
{
    m_bStop = true;
    m_pMonitorThread->join();
}

void WatchdogServiceBase::init(const std::string& sWorkDir)
{
    Path p(Path::expand(sWorkDir));
    p.makeDirectory();
    m_sWorkDir = p.toString();
    p.pushDirectory("processes");
    m_sDefaultProcessWorkDir = p.toString();
    p.popDirectory();
    p.setFileName("process.status");
    m_sStatusFile = p.toString();

    if (!deserializeStatusFromFile())
    {
        FX_LOG(INFO, "Recovered from status file FAILED");
    }
    else
    {
        runOnce();
    }
    
    m_pMonitorThread.reset(new Thread());
    m_pMonitorThread->start(*this);
}

WatchdogServiceBase::PIDType WatchdogServiceBase::startProcess(
        ErrorInfo& errorInfo, const ExecutionTask& task)
{
    string sEncodedProc = encodeProcessName(task.command.processName,
            task.command.parameters);
    {
        FastMutex::Guard g(m_mutex);
        if (m_processes.find(sEncodedProc) != m_processes.end())
        {
            errorInfo.errorCode = EC_PROCESS_EXIST;
            errorInfo.errorMsg = "Process exists";
            FX_LOG(ERROR, "Start process: [%s] FAILED, reason: [%s]", 
                   task.command.processName.c_str(),
                   errorInfo.errorMsg.c_str());
            return -1;
        }
    }
    
    try
    {
        string sWorkDir;
        if (task.workDirectory.empty())
        {
            Path p(m_sDefaultProcessWorkDir);
            p.makeDirectory();
            p.pushDirectory(sEncodedProc);
            sWorkDir = p.toString();
        }
        else
        {
            Path p(task.workDirectory);
            p.makeDirectory();
            sWorkDir = Path::expand(p.toString());
        }

        ProcessHandlePtr pProcHandle;
        try
        {
            pProcHandle = launchProcess(task.command.processName, 
                    task.command.parameters, task.envVariables, sWorkDir);
        }
        catch(const SystemException& e)
        {
            FX_LOG(ERROR, "Start process: [%s] FAILED, reason: [%s]", 
                   task.command.processName.c_str(), e.what().c_str());
            errorInfo.errorCode = EC_SYSTEM_ERROR;
            errorInfo.errorMsg = e.what();
        }
        
        ProcessInfoPtr pInfo(new ProcessInfo(task.command.processName,
                        task.command.parameters, task.envVariables,
                        task.processInfoKeepTime, sWorkDir, 
                        pProcHandle, task.restartAfterCrash));
        {
            FastMutex::Guard g(m_mutex);
            if (m_processes.find(sEncodedProc) != m_processes.end())
            {
                killProcess(*pProcHandle);
                errorInfo.errorCode = EC_PROCESS_EXIST;
                errorInfo.errorMsg = "Process exists";
                
                FX_LOG(ERROR, "Start process: [%s] FAILED, reason: [%s]", 
                       task.command.processName.c_str(),
                       errorInfo.errorMsg.c_str());

                return -1;
            }
            pInfo->status = PS_RUNNING;
            m_processes.insert(make_pair(sEncodedProc, pInfo));

            if (!serializeStatusToFile())
            {
                killProcess(*pProcHandle);
                m_processes.erase(sEncodedProc);

                errorInfo.errorCode = EC_SERIALIZE_FAIL;
                errorInfo.errorMsg = "Serialize status to disk failed";

                FX_LOG(ERROR, "Start process: [%s] FAILED, reason: [%s]", 
                       task.command.processName.c_str(),
                       errorInfo.errorMsg.c_str());

                return -1;
            }
        }
        errorInfo.errorCode = EC_OK;
        return pProcHandle->id();
    }
    catch (const SystemException& e)
    {
        errorInfo.errorCode = EC_SYSTEM_ERROR;
		errorInfo.errorMsg = "Launch process failed: " + e.what();

        FX_LOG(ERROR, "Start process: [%s] FAILED, reason: [%s]", 
               task.command.processName.c_str(), errorInfo.errorMsg.c_str());
        return -1;
    }
}

void WatchdogServiceBase::stopProcess(ErrorInfo& errorInfo, 
                                     const Command& cmd,
                                     const int32_t stopSignal)
{
    string sEncodedProc = encodeProcessName(cmd.processName,
            cmd.parameters);

    FastMutex::Guard g(m_mutex);
    ProcessMap::iterator it = m_processes.find(sEncodedProc);
    if (it == m_processes.end())
    {
        errorInfo.errorCode = EC_PROCESS_NOT_FOUND;
        errorInfo.errorMsg = "Process not found";
        return;
    }
    killProcess(*(it->second->procHandle));
    it->second->status = PS_STOPPING;
    if (!serializeStatusToFile())
    {
        errorInfo.errorCode = EC_SERIALIZE_FAIL;
        errorInfo.errorMsg = "Serialize status to disk failed";
        return;
    }

    errorInfo.errorCode = EC_OK;
}

WatchdogServiceBase::ProcessInfoPtr WatchdogServiceBase::getProcessInfo(
        ErrorInfo& errorInfo, const Command& cmd)
{
    string sEncodedProc = encodeProcessName(cmd.processName,
            cmd.parameters);

    FastMutex::Guard g(m_mutex);
    ProcessMap::const_iterator it = m_processes.find(sEncodedProc);
    if (it == m_processes.end())
    {
        errorInfo.errorCode = EC_PROCESS_NOT_FOUND;
        errorInfo.errorMsg = "Process not found";
        return ProcessInfoPtr();
    }
    
    errorInfo.errorCode = EC_OK;
    return it->second;
}

void WatchdogServiceBase::run()
{
    while (!m_bStop)
    {
        Thread::sleep(DEFAULT_LOOP_INTERVAL);
        runOnce();
    }
}

void WatchdogServiceBase::runOnce()
{
    FastMutex::Guard g(m_mutex);
    vector<string> processToRemove;
    bool bNeedSerialize = false;
    for (ProcessMap::iterator it = m_processes.begin();
         it != m_processes.end(); ++it)
    {
        ProcessInfoPtr& pProcInfo = it->second;
        ProcessStatus lastStatus = pProcInfo->status;
        if (lastStatus == PS_STOPPING)
        {
            ProcessStatus curStatus = this->getProcessStatus(
                    pProcInfo->procHandle);
            if (curStatus == PS_RUNNING)
            {
                // kill again
                FX_LOG(INFO, "Kill process: [%d] again", it->second->procHandle->id());
                killProcess(*(pProcInfo->procHandle));
            }
            else
            {
                pProcInfo->exitTime = time(NULL);
                FX_LOG(INFO, "Process: [%d] stopped", it->second->procHandle->id());
                processToRemove.push_back(it->first);
                bNeedSerialize = true;
            }
        }
        else if (lastStatus == PS_RUNNING)
        {
            ProcessStatus curStatus = this->getProcessStatus(
                    pProcInfo->procHandle);
            if (curStatus == PS_EXIT_WITH_ERROR 
                || curStatus == PS_UNKNOWN)
            {
                pProcInfo->status = curStatus;
                pProcInfo->lastCrashTime = time(NULL);
                if (pProcInfo->restartAfterCrash)
                {
                    restartProcess(pProcInfo);
                }
                bNeedSerialize = true;
                FX_LOG(INFO, "Process: [%d] exit with error", it->second->procHandle->id());
            }
            else if (curStatus == PS_EXIT_NORMAL)
            {
                pProcInfo->exitTime = time(NULL);
                processToRemove.push_back(it->first);
                bNeedSerialize = true;
                FX_LOG(INFO, "Process: [%d] exit normally", it->second->procHandle->id());
            }
        }
    } // end for

    for (vector<string>::const_iterator it = processToRemove.begin();
         it != processToRemove.end(); ++it)
    {
        m_processes.erase(*it);
    }

    if (bNeedSerialize && !serializeStatusToFile())
    {
        FX_LOG(ERROR, "Serialize status to disk FAILED");
    }
}

void WatchdogServiceBase::clear()
{
    FastMutex::Guard g(m_mutex);
    for (ProcessMap::iterator it = m_processes.begin();
         it != m_processes.end(); ++it)
    {
        ProcessInfoPtr& pProcInfo = it->second;
        FX_LOG(INFO, "Kill process: [%d]", pProcInfo->procHandle->id());
        killProcess(*(pProcInfo->procHandle));
        pProcInfo->status = PS_STOPPING;
    }
}

string WatchdogServiceBase::encodeProcessName(const string& sProcName, 
        const vector<string>& params)
{
    ostringstream oss2;
    oss2 << sProcName << "_" 
         << Hash::hashString64(encodeParameters(params).c_str());

    return oss2.str();
}

void WatchdogServiceBase::restartProcess(ProcessInfoPtr& pProcInfo)
{
    try
    {
        FX_LOG(INFO, "Restart process: [%d]", pProcInfo->procHandle->id());

        ProcessHandlePtr pProcHandle = launchProcess(pProcInfo->processName,
                pProcInfo->parameters, pProcInfo->envVariables, 
                pProcInfo->workDirectory);
        pProcInfo->procHandle = pProcHandle;
    }
    catch(const SystemException& e)
    {
        pProcInfo->restartCount++;
        FX_LOG(ERROR, "Restart process: [%d] FAILED, message: [%s]", 
               pProcInfo->procHandle->id(), e.what().c_str());
        return;
    }
    pProcInfo->restartCount++;
    
    FX_LOG(INFO, "Process: [%d] restarted", pProcInfo->procHandle->id());
}

void WatchdogServiceBase::procInfoToXML(XMLNodeWrapperPtr& pProcNode, 
                                        const ProcessInfoPtr& pProcInfo)
{
    pProcNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                          "name", pProcInfo->processName);
    XMLNodeWrapperPtr pArgsNode = pProcNode->appendNode(
            XMLDocumentWrapper::NODE_ELEMENT, "arguments");
    for (Args::const_iterator aIt = pProcInfo->parameters.begin();
         aIt != pProcInfo->parameters.end(); ++aIt)
    {
        XMLNodeWrapperPtr pArgNode = pArgsNode->appendNode(
                XMLDocumentWrapper::NODE_ELEMENT, "argument");
        pArgNode->appendNode(XMLDocumentWrapper::NODE_CDATA, "", *aIt);
    }

    XMLNodeWrapperPtr pEnvNode = pProcNode->appendNode(
            XMLDocumentWrapper::NODE_ELEMENT, "environment");
    for (EnvMap::const_iterator envIt = pProcInfo->envVariables.begin();
         envIt != pProcInfo->envVariables.end(); ++envIt)
    {
        XMLNodeWrapperPtr pValueNode = pEnvNode->appendNode(
                XMLDocumentWrapper::NODE_ELEMENT, "value");
        pValueNode->appendAttribute("name", envIt->first);
        pValueNode->appendNode(XMLDocumentWrapper::NODE_CDATA,
                               "", envIt->second);
    }

    string str;
    NumberFormatter::append(str, pProcInfo->processInfoKeepTime);
    pProcNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                          "process_info_keep_time", 
                          str);
    pProcNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                          "work_directory", 
                          pProcInfo->workDirectory);
    str.clear();
    NumberFormatter::append(str, pProcInfo->procHandle->id());
    pProcNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                          "pid", str);
    str.clear();
    NumberFormatter::append(str, (int32_t)pProcInfo->status);
    pProcNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                          "status", str);
    str.clear();
    NumberFormatter::append(str, pProcInfo->restartCount);
    pProcNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                          "restart_count", str);
    str.clear();
    NumberFormatter::append(str, (int64_t)pProcInfo->launchTime);
    pProcNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                          "launch_time", str);
    str.clear();
    NumberFormatter::append(str, (int64_t)pProcInfo->exitTime);
    pProcNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                          "exit_time", str);
    str.clear();
    NumberFormatter::append(str, (int64_t)pProcInfo->lastCrashTime);
    pProcNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                          "last_crash_time", str);
    str.clear();
    NumberFormatter::append(str, pProcInfo->stopSignal);
    pProcNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                          "stop_signal", str);
    pProcNode->appendNode(XMLDocumentWrapper::NODE_ELEMENT,
                          "restart_after_crash",
                          pProcInfo->restartAfterCrash ? "true" : "false");
}

bool WatchdogServiceBase::serializeStatusToFile()
{
    try
    {
        XMLDocumentWrapper xmlDoc;
        xmlDoc.addDeclarationNode("utf-8");
        XMLNodeWrapperPtr pRootNode = xmlDoc.appendNode(
                XMLDocumentWrapper::NODE_ELEMENT, "processes");
        for (ProcessMap::const_iterator it = m_processes.begin();
             it != m_processes.end(); ++it)
        {
            const ProcessInfoPtr& pProcInfo = it->second;
            XMLNodeWrapperPtr pProcNode = pRootNode->appendNode(
                    XMLDocumentWrapper::NODE_ELEMENT, "process");
            procInfoToXML(pProcNode, pProcInfo);
        }

        string sStatusTmpFile = m_sStatusFile + ".temp";
        xmlDoc.printToFile(sStatusTmpFile);
        string sStatusBackFile = m_sStatusFile + ".backup";
        if (File::exists(sStatusBackFile))
        {
            File f(sStatusBackFile);
            f.remove();
        }
        if (File::exists(m_sStatusFile))
        {
            File f(m_sStatusFile);
            f.moveTo(sStatusBackFile);
        }
        
        File f(sStatusTmpFile);
        f.moveTo(m_sStatusFile);
    }
    catch(const FirteXException& e)
    {
        FX_LOG(ERROR, "Serialize status to file: [%s] FAILED, [%s]", 
               m_sStatusFile.c_str(), e.what().c_str());
        return false;
    }

    return true;
}


bool WatchdogServiceBase::deserializeStatusFromFile()
{
    try
    {
        XMLDocumentWrapper xmlDoc;
        if (File::exists(m_sStatusFile))
        {
            xmlDoc.parseFile(m_sStatusFile);
        }
        else 
        {
            string sStatusTmpFile = m_sStatusFile + ".temp";
            if (File::exists(sStatusTmpFile))
            {
                xmlDoc.parseFile(sStatusTmpFile);
            }
            else
            {
                string sStatusBackFile = m_sStatusFile + ".backup";
                if (File::exists(sStatusBackFile))
                {
                    xmlDoc.parseFile(sStatusTmpFile);
                }
                else
                {
                    return false;
                }
            }
        }

        XMLNodeWrapperPtr pRootNode = xmlDoc.firstNode("processes");
        if (pRootNode)
        {
            for (XMLNodeWrapperPtr pProcNode = pRootNode->firstNode("process");
                 pProcNode; pProcNode = pProcNode->nextSibling())
            {
                ProcessInfoPtr pProcInfo(new ProcessInfo());

                XMLNodeWrapperPtr pNameNode  = pProcNode->firstNode("name");
                IF_NULL_THEN_RETURN(pNameNode, "<name>");
                pProcInfo->processName = pNameNode->getValue();

                XMLNodeWrapperPtr pArgsNode  = pProcNode->firstNode("arguments");
                IF_NULL_THEN_RETURN(pArgsNode, "<arguments>");
                for (XMLNodeWrapperPtr pArgNode = pArgsNode->firstNode("argument");
                     pArgNode; pArgNode = pArgNode->nextSibling())
                {
                    XMLNodeWrapperPtr pDataNode = pArgNode->firstNode();
                    pProcInfo->parameters.push_back(pDataNode->getValue());
                }

                XMLNodeWrapperPtr pEnvNode = pProcNode->firstNode("environment");
                IF_NULL_THEN_RETURN(pEnvNode, "<environment>");
                
                for (XMLNodeWrapperPtr pValueNode = pEnvNode->firstNode("value");
                     pValueNode; pValueNode = pValueNode->nextSibling())
                {
                    XMLAttributeWrapperPtr pNameAttr = pValueNode->firstAttribute("name");
                    XMLNodeWrapperPtr pDataNode = pValueNode->firstNode();
                    pProcInfo->envVariables.insert(make_pair(pNameAttr->getValue(), 
                                    pDataNode->getValue()));
                }
                XMLNodeWrapperPtr pProcInfoKeepTimeNode = 
                    pProcNode->firstNode("process_info_keep_time");
                IF_NULL_THEN_RETURN(pProcInfoKeepTimeNode, 
                        "<process_info_keep_time>");
                pProcInfo->processInfoKeepTime = NumberParser::parseInt32(
                        pProcInfoKeepTimeNode->getValue());
                
                XMLNodeWrapperPtr pWorkDirNode = pProcNode->firstNode("work_directory");
                IF_NULL_THEN_RETURN(pWorkDirNode, "work_directory");
                pProcInfo->workDirectory = pWorkDirNode->getValue();

                XMLNodeWrapperPtr pPIDNode = pProcNode->firstNode("pid");
                IF_NULL_THEN_RETURN(pPIDNode, "pid");
                PIDType pid = NumberParser::parseInt32(pPIDNode->getValue());
                pProcInfo->procHandle.reset(new ProcessHandle(pid));

                XMLNodeWrapperPtr pStatusNode = pProcNode->firstNode("status");
                IF_NULL_THEN_RETURN(pStatusNode, "<status>");
                pProcInfo->status = (ProcessStatus)NumberParser::parseInt32(
                        pStatusNode->getValue());
                
                XMLNodeWrapperPtr pRestartCountNode = pProcNode->firstNode("restart_count");
                IF_NULL_THEN_RETURN(pRestartCountNode, "<restart_count>");
                pProcInfo->restartCount = NumberParser::parseInt32(
                        pRestartCountNode->getValue());

                XMLNodeWrapperPtr pLaunchTime = pProcNode->firstNode("launch_time");
                IF_NULL_THEN_RETURN(pLaunchTime, "<launch_time>");
                pProcInfo->launchTime = NumberParser::parseInt32(
                        pLaunchTime->getValue());

                XMLNodeWrapperPtr pExitTime = pProcNode->firstNode("exit_time");
                IF_NULL_THEN_RETURN(pExitTime, "<exit_time>");
                pProcInfo->exitTime = NumberParser::parseInt32(
                        pExitTime->getValue());

                XMLNodeWrapperPtr pLastCrashTime = pProcNode->firstNode("last_crash_time");
                IF_NULL_THEN_RETURN(pLastCrashTime, "<last_crash_time>");
                pProcInfo->lastCrashTime = NumberParser::parseInt32(
                        pLastCrashTime->getValue());
                
                XMLNodeWrapperPtr pStopSignalNode = pProcNode->firstNode("stop_signal");
                IF_NULL_THEN_RETURN(pStopSignalNode, "<stop_signal>");
                pProcInfo->stopSignal = NumberParser::parseInt32(
                        pStopSignalNode->getValue());

                XMLNodeWrapperPtr pRestartAfterCrash = pProcNode->firstNode("restart_after_crash");
                IF_NULL_THEN_RETURN(pRestartAfterCrash, "<restart_after_crash>");
                if (!strCompareNoCase(pRestartAfterCrash->getValue(), "true"))
                {
                    pProcInfo->restartAfterCrash = true;
                }
                else
                {
                    pProcInfo->restartAfterCrash = false;
                }

                m_processes.insert(make_pair(encodeProcessName(
                                        pProcInfo->processName, 
                                        pProcInfo->parameters), pProcInfo));
            }
        }
    }
    catch(const FirteXException& e)
    {
        FX_LOG(ERROR, "De-serialize status from file: [%s] FAILED, [%s]", 
               m_sStatusFile.c_str(), e.what().c_str());
        return false;
    }

    return true;
}

ProcessHandlePtr WatchdogServiceBase::launchProcess(
        const std::string& command, const Args& args,
        const EnvMap& envVariables, const std::string& sWorkDir)
{
    FX_LOG(INFO, "Launching process: [%s], arguments: [%s]",
           command.c_str(), encodeParameters(args).c_str());
    return Process::launch(command, args, envVariables, sWorkDir);
}

void WatchdogServiceBase::killProcess(const ProcessHandle& handle)
{
    FX_LOG(INFO, "Kill process: [%d]", handle.id());
    Process::kill(handle);
}

ProcessStatus WatchdogServiceBase::getProcessStatus(const ProcessHandlePtr& pProcHandle)
{
    return Process::getProcessStatus(pProcHandle);
}

#undef IF_NULL_THEN_RETURN

FX_NS_END

