#include "firtex/extension/util/ServerApplication.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/File.h"
#include "firtex/common/Logger.h"
#include "BuilderAppRunner.h"

#if defined(_MSC_VER)
#include <crtdbg.h>
#endif

using namespace std;

FX_NS_USE(common);
FX_NS_USE(utility);
FX_NS_USE(app);
FX_NS_USE(util);

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

    ServerApplication builderApp("fbuilder", "Index builder on local file system.");
    builderApp.addAppRunner(new BuilderAppRunner);
    if (builderApp.parse(argc, (const char**)argv))
    {
        return builderApp.run();
    }
    return 0;
}
