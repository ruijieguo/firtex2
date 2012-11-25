#include "HTTPWatchdogService.h"
#include "firtex/utility/KeyValueParser.h"
#include "firtex/utility/NumberParser.h"
#include "firtex/utility/StringTokenizer.h"

using namespace std;
FX_NS_USE(network);
FX_NS_USE(utility);

FX_NS_DEF(app);

SETUP_LOGGER(app, HTTPWatchdogService);

HTTPWatchdogService::HTTPWatchdogService() 
{
}

HTTPWatchdogService::~HTTPWatchdogService() 
{
}

std::string HTTPWatchdogService::requestCanHandle() const
{
    return "/watchdog";
}

void HTTPWatchdogService::handleRequest(EvHttpRequestContext* pCtx)
{
    ExecutionTask task;

    switch(pCtx->getMethodType())
    {
    case EVHTTP_REQ_GET:
        if (commandToTask(pCtx, task))
        {
            ErrorInfo errorInfo;
            ProcessInfoPtr pProcInfo = getProcessInfo(errorInfo, task.command);
            if (pProcInfo.isNull())
            {
                sendErrorInfo(pCtx, errorInfo);
            }
            else
            {
                pCtx->setResponseCode(HTTPTypes::RESPONSE_CODE_OK);
                sendProcessInfo(pCtx, pProcInfo);
            }
        }
        break;
    case EVHTTP_REQ_PUT:
    case EVHTTP_REQ_POST:
        // start process
        if (commandToTask(pCtx, task))
        {
            ErrorInfo errorInfo;
            PIDType pid = startProcess(errorInfo, task);
            if (errorInfo.errorCode == EC_OK)
            {
                pCtx->setResponseCode(HTTPTypes::RESPONSE_CODE_CREATED);
                sendPID(pCtx, pid);
            }
            else 
            {
                sendErrorInfo(pCtx, errorInfo);
            }
        }
        break;

    case EVHTTP_REQ_DELETE:
    {
        int32_t stopSignal = 6; //SIGTERM
        if (commandToTask(pCtx, task, stopSignal))
        {
            ErrorInfo errorInfo;
            stopProcess(errorInfo, task.command, stopSignal);
            sendErrorInfo(pCtx, errorInfo);
        }
    }
        break;
    default:
        sendErrorMsg(pCtx, "Unsupported HTTP method (GET/PUT/POST/DELETE only)", 
                     HTTPTypes::RESPONSE_CODE_BAD_REQUEST);                     
        break;
    }
}

#define EXTRACT_NODE_STRING(pNode, value)                               \
    if (pNode.isNotNull())                                              \
    {                                                                   \
        XMLNodeWrapperPtr pDataNode = pNode->firstNode();               \
        if (pDataNode.isNotNull())                                      \
            value = pDataNode->getValue();                              \
    }


#define EXTRACT_NODE_STRING2(pRootNode, nodeName, value)                \
    {                                                                   \
        XMLNodeWrapperPtr pSubNode = pRootNode->firstNode(nodeName);    \
        EXTRACT_NODE_STRING(pSubNode, value);                           \
    }


bool HTTPWatchdogService::commandToTask(EvHttpRequestContext* pCtx,
                                        ExecutionTask& task)
{
    int32_t stopSignal = 0;
    return commandToTask(pCtx, task, stopSignal);
}

bool HTTPWatchdogService::commandToTask(EvHttpRequestContext* pCtx,
                                        ExecutionTask& task, 
                                        int32_t& stopSignal)
{
    string sContent;
    if (pCtx->getMethodType() == EVHTTP_REQ_GET)
    {
        sContent = pCtx->getQuery();
        KeyValueParser kvParser;
        if (!kvParser.parse(sContent, "&", "="))
        {
            stringstream ss;
            ss << "Invalid request: [" << sContent << "]";
            sendErrorMsg(pCtx, ss.str(), HTTPTypes::RESPONSE_CODE_BAD_REQUEST);
            return false;
        }
        string sValue;
        if (kvParser.getValue("process_name", sValue))
        {
            task.command.processName = sValue;
        }
        if (kvParser.getValue("parameters", sValue))
        {
            StringTokenizer st(sValue, ",", StringTokenizer::TOKEN_TRIM
                    | StringTokenizer::TOKEN_IGNORE_EMPTY);
            task.command.parameters.assign(st.begin(), st.end());
        }
        return true;
    }
    else // POST/PUT/DELETE
    {
        const char* p = (const char*)(pCtx->getInBuffer()->getData());
        if (!p || !*p)
        {
            sendErrorMsg(pCtx, "Request missing content", 
                        HTTPTypes::RESPONSE_CODE_BAD_REQUEST);
            return false;
        }

        XMLDocumentWrapper xmlDoc;
        try
        {
            string str(p, pCtx->getInBuffer()->getLength());
            xmlDoc.parse(str);
        }
        catch(const FirteXException& ex)
        {
            sendErrorMsg(pCtx, ex.what(), HTTPTypes::RESPONSE_CODE_BAD_REQUEST);
            return false;
        }

        XMLNodeWrapperPtr pRootNode = xmlDoc.firstNode("process");
        if (pRootNode.isNull())
        {
            sendErrorMsg(pCtx, "Missing process info", 
                        HTTPTypes::RESPONSE_CODE_BAD_REQUEST);
            return false;
        }
            
        EXTRACT_NODE_STRING2(pRootNode, "process_name", 
                             task.command.processName);
        if (task.command.processName.empty())
        {
            sendErrorMsg(pCtx, "Missing process name", 
                        HTTPTypes::RESPONSE_CODE_BAD_REQUEST);
            return false;
        }
            
        XMLNodeWrapperPtr pParamRootNode = pRootNode->firstNode("parameters");
        if (pParamRootNode.isNotNull())
        {
            for (XMLNodeWrapperPtr pParamNode = pParamRootNode->firstNode("parameter");
                 pParamNode.isNotNull(); pParamNode = pParamNode->nextSibling())
            {
                string str;
                EXTRACT_NODE_STRING(pParamNode, str);
                task.command.parameters.push_back(str);
            }
        }

        XMLNodeWrapperPtr pEnvNode = pRootNode->firstNode("environment");
        if (pEnvNode.isNotNull())
        {
            for (XMLNodeWrapperPtr pValueNode = pEnvNode->firstNode("value");
                 pValueNode.isNotNull(); pValueNode = pValueNode->nextSibling())
            {
                XMLAttributeWrapperPtr pNameAttr = pValueNode->firstAttribute("name");
                XMLNodeWrapperPtr pDataNode = pValueNode->firstNode();
                task.envVariables.insert(make_pair(pNameAttr->getValue(), 
                                pDataNode->getValue()));
            }
        }

        XMLNodeWrapperPtr pProcInfoKeepTimeNode = 
            pRootNode->firstNode("process_info_keep_time");
        if (pProcInfoKeepTimeNode.isNotNull())
        {
            task.processInfoKeepTime = NumberParser::parseInt32(
                    pProcInfoKeepTimeNode->getValue());
        }

        XMLNodeWrapperPtr pStopSignalNode = pRootNode->firstNode("stop_signal");
        if (pStopSignalNode.isNotNull())
        {
            stopSignal = NumberParser::parseInt32(pStopSignalNode->getValue());
        }
            
        EXTRACT_NODE_STRING2(pRootNode, "work_directory", task.workDirectory);

        XMLNodeWrapperPtr pRestartAfterCrash = pRootNode->firstNode("restart_after_crash");
        if (pRestartAfterCrash.isNotNull())
        {
            if (!strCompareNoCase(pRestartAfterCrash->getValue(), "true"))
            {
                task.restartAfterCrash = true;
            }
            else
            {
                task.restartAfterCrash = false;
            }
        }
        return true;
    } //end else // POST/PUT/DELETE
}

#undef EXTRACT_NODE_STRING
#undef EXTRACT_NODE_STRING2

void HTTPWatchdogService::sendErrorInfo(EvHttpRequestContext* pCtx,
                                        const ErrorInfo& errorInfo)
{
    switch(errorInfo.errorCode)
    {
    case EC_PROCESS_EXIST:
        pCtx->setResponseCode(HTTPTypes::RESPONSE_CODE_NOT_MODIFIED);
        pCtx->getOutBuffer()->addData(errorInfo.errorMsg.c_str(), 
                errorInfo.errorMsg.length());        
        break;
    case EC_NO_PERMISSION:
        pCtx->setResponseCode(HTTPTypes::RESPONSE_CODE_FORBIDDEN);
        pCtx->getOutBuffer()->addData(errorInfo.errorMsg.c_str(), 
                errorInfo.errorMsg.length());        
        break;
    case EC_SERIALIZE_FAIL:
    case EC_SYSTEM_ERROR:
        pCtx->setResponseCode(HTTPTypes::RESPONSE_CODE_INTERNAL_SERVER_ERROR);
        pCtx->getOutBuffer()->addData(errorInfo.errorMsg.c_str(), 
                errorInfo.errorMsg.length());
    case EC_OK:
        pCtx->setResponseCode(HTTPTypes::RESPONSE_CODE_OK);
        break;
    case EC_PROCESS_NOT_FOUND:
        pCtx->setResponseCode(HTTPTypes::RESPONSE_CODE_NOT_FOUND);
        break;
    }
}

void HTTPWatchdogService::sendProcessInfo(EvHttpRequestContext* pCtx,
        const ProcessInfoPtr& pProcInfo)
{
    XMLDocumentWrapper xmlDoc;
    XMLNodeWrapperPtr pProcNode = xmlDoc.appendNode(
            XMLDocumentWrapper::NODE_ELEMENT, "process");
    procInfoToXML(pProcNode, pProcInfo);
    stringstream ss;
    xmlDoc.print(ss);
    pCtx->getOutBuffer()->addData(ss.str().c_str(), ss.str().length());
}

void HTTPWatchdogService::sendPID(EvHttpRequestContext* pCtx, PIDType pid)
{
    stringstream ss;
    ss << pid;
    XMLDocumentWrapper xmlDoc;    
    xmlDoc.appendNode(XMLDocumentWrapper::NODE_ELEMENT, "pid", ss.str());
    
    ostringstream oss;
    xmlDoc.print(oss);
    pCtx->getOutBuffer()->addData(oss.str().c_str(), oss.str().length());
}

void HTTPWatchdogService::sendErrorMsg(EvHttpRequestContext* pCtx,
                                       const string& sErrorMsg, 
                                       HTTPTypes::ResponseCode rc)
{
    pCtx->setResponseCode(rc);
    pCtx->getOutBuffer()->addData(sErrorMsg.c_str(), sErrorMsg.length());
}

FX_NS_END

