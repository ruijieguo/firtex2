#include "RPCSearchClientAppRunner.h"
#include "../common/RPCSyncClient.h"
#include "firtex/extension/util/Application.h"
#include "firtex/extension/util/SharedOptionCallback.h"
#include "firtex/extension/util/OptionException.h"
#include "firtex/utility/NumberParser.h"
#include "firtex/common/Logger.h"

#include "../thrift/gen-cpp/ThriftSearchService.h"

using namespace std;

FX_NS_USE(util);
FX_NS_USE(utility);
FX_NS_USE(common);
FX_NS_USE(config);
FX_NS_USE(thread);

FX_NS_DEF(app);

SETUP_LOGGER(app, RPCSearchClientAppRunner);

typedef RPCSyncClient<ThriftSearchServiceClient> SearchServiceClient;

RPCSearchClientAppRunner::RPCSearchClientAppRunner()
    : m_nPort(-1)
    , m_bStartShell(false)
{
}

RPCSearchClientAppRunner::~RPCSearchClientAppRunner()
{
}

AppRunner::Status RPCSearchClientAppRunner::run()
{
    if (m_sHost.empty() || m_nPort < 0)
    {
        cout << "Please specify host name and port" << endl;
        return S_NONE;
    }

    if (!m_bStartShell && m_sQueryString.empty() 
        && m_sQueryFile.empty())
    {
        cout << "You must make a choice: start shell, " 
             << "send a query or search from file." << endl;
        return S_NONE;
    }

    if (m_bStartShell)
    {
        startShell();
        return S_EXIT;
    }
    else
    {
        doSearch();
        return S_EXIT;
    }
}

void RPCSearchClientAppRunner::doSearch()
{
    try
    {
        SearchServiceClient client;
        client.open(m_sHost, m_nPort);
        
        string sResult;
        client.syntaxSearch(sResult, m_sQueryString);
        cout << "Search Result: " << endl;
        cout << "================================ " << endl;
        cout << sResult << endl;

        client.close();
    }
    catch(const FirteXException& e)
    {
        cout << "ERROR: " << e.what() << endl;
    }
}

void RPCSearchClientAppRunner::registerOption(Application* pApp)
{
    pApp->addDescription("Run search client : ");
    pApp->addDescription(string("  ") + pApp->getProgName() + 
                         " -s [HOST_NAME] -p [PORT_NUM]");

    pApp->addOption(Option("host", "s", "search host name")
                    .required(true)
                    .repeatable(false)
                    .argument("[HOST_NAME]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option("port", "p", "search host port")
                    .required(true)
                    .repeatable(false)
                    .argument("[PORT_NUM]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option("query", "q", "query string")
                    .required(false)
                    .repeatable(false)
                    .argument("[QUERY_STRING]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option("query-file", "f", "search from query file")
                    .required(false)
                    .repeatable(false)
                    .argument("[QUERY_FILE]", true)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 

    pApp->addOption(Option("shell", "e", "start search client shell")
                    .required(false)
                    .repeatable(false)
                    .callback(SharedOptionCallback(this, &AppRunner::optionCallback))); 
}

void RPCSearchClientAppRunner::optionCallback(const Option& option,
        const string& sValue)
{
    if (option.matchFull("host") || option.matchShort("s"))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--host/-s");
        }
        m_sHost = sValue;
    }

    if (option.matchFull("port") || option.matchShort("p"))
    {
        if (sValue.empty())
        {
            throw MissingArgumentException("--port/-p");
        }
        m_nPort = (int)NumberParser::parseInt32(sValue);
    }

    if (option.matchFull("query") || option.matchShort("q"))
    {
        m_sQueryString = sValue;
    }

    if (option.matchFull("query-file") || option.matchShort("f"))
    {
        m_sQueryFile = sValue;
    }

    if (option.matchFull("shell") || option.matchShort("e"))
    {
        m_bStartShell = true;
    }
}

void RPCSearchClientAppRunner::startShell()
{
    cout << "Starting shell..." << endl;

    try
    {
        SearchServiceClient client;
        client.open(m_sHost, m_nPort);

        cout << "FirteX search shell, input quit to exit, "
             << "input help to display help message." 
             << endl;

        char buffer[1024];
        size_t len;

        string strCmd;
        while (true)
        {
            cout << "$ ";
            char* p = fgets(buffer, 1024, stdin);
            (void)p; //avoid warning
            len = strlen(buffer);
            buffer[len - 1] = 0;
            strCmd = buffer;

            if (!strCmd.compare("quit"))
            {
                break;
             }
             else if (!strCmd.compare("help"))
             {
                 displayShellHelp();
                 continue;
             }
             try
             {
                 string sResult;
                 client.syntaxSearch(sResult, strCmd);
                 cout << "Search Result: " << endl;
                 cout << "================================ " << endl;
                 cout << sResult << endl;
             }
             catch(const FirteXException& e)
             {
                 cout << "Error: " << e.what() << endl;
             }
             catch(const std::exception& e)
             {
                 cout << "Error: " << e.what() << endl;
             }
             catch(...)
             {
                 cout << "Unknown error."  << endl;
             }

             cout << endl;
        }// end while

        client.close();
    }
    catch (FirteXException &e) 
    {
        cout << "ERROR: " <<  e.what() << endl;
    }
}

void RPCSearchClientAppRunner::displayShellHelp()
{
    cout << "syntax: " << endl;
    cout << "  query=query_expr;[sort_by=sort_expr;]"
        "[filter=filter_expr;][parameter=param_string;]" << endl;

    cout << endl;
    cout << "example1: boolean query" << endl;
    cout << "  query=BODY:a AND BODY:b OR c" << endl;

    cout << endl;
    cout << "example2: query with sort and filter clause" << endl;
    cout << "  query=BODY:a AND BODY:b;sort_by=FLOOR(price)+MAX(tax);filter=SQRT(id)>10" << endl;

    cout << endl;
    cout << "example3: query with parameter" << endl;
    cout << "  query=BODY:a AND BODY:b;parameter=top:10,start:5,snippet:BODY" << endl;
}

void RPCSearchClientAppRunner::stop()
{
    // if (m_pServer)
    // {
    //     m_pServer->stop();
    // }
}
 
void RPCSearchClientAppRunner::waitStop()
{
}


FX_NS_END
