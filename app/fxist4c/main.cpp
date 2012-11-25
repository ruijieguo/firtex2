#include "firtex/extension/util/Application.h"
#include "IndexBuilderAppRunner.h"
#include "FileIndexerAppRunner.h"
#include "IndexSearcherAppRunner.h"
#include "IndexMergerAppRunner.h"
#include "IndexOverviewAppRunner.h"
#include "DocumentOverviewAppRunner.h"
#include "IndexPrinterAppRunner.h"
#include "firtex/utility/Path.h"
#include "firtex/config/XMLConfigurator.h"

#if defined(_MSC_VER)
#include <crtdbg.h>
#endif

FX_NS_USE(app);
FX_NS_USE(util);
FX_NS_USE(common);
FX_NS_USE(config);
FX_NS_USE(utility);

#define LOGGER_CONF_FILE_NAME "logger_conf.xml"

int main(int argc, char* argv[])
{
    Path logFilePath(Path::current());
    logFilePath.makeDirectory();
    logFilePath.setFileName(LOGGER_CONF_FILE_NAME);

    string sLogConf = logFilePath.toString();
    File confFile(sLogConf);
    if (confFile.exists())
    {
        cout << "[INFO] Load logger configure file: [" << sLogConf << "]" << endl;
        LoggerBuilder::instance()->configure(sLogConf);
    }

    Application fxistApp("fxist4c", "FirteX utility for index buiding, "
                                       "searching, printing, etc.");
    fxistApp.addAppRunner(new IndexBuilderAppRunner);
    fxistApp.addAppRunner(new IndexSearcherAppRunner);
    fxistApp.addAppRunner(new IndexOverviewerAppRunner);
    fxistApp.addAppRunner(new FileIndexerAppRunner);
    fxistApp.addAppRunner(new IndexMergerAppRunner);
    fxistApp.addAppRunner(new IndexPrinterAppRunner);
    fxistApp.addAppRunner(new DocumentOverviewAppRunner);
    if (fxistApp.parse(argc, (const char**)argv))
    {
        return fxistApp.run();
    }

    if (confFile.exists())
    {
        FX_LOG_FLUSH_ALL();
    }

    return 0;
}
