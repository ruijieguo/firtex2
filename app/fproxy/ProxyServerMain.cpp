#include "firtex/extension/util/ServerApplication.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/File.h"
#include "firtex/common/Logger.h"
#include "ProxyServerAppRunner.h"

FX_NS_USE(common);
FX_NS_USE(app);
FX_NS_USE(util);
FX_NS_USE(utility);

using namespace std;

int main(int argc,char* argv[])
{
    ServerApplication app("fproxy", "FirteX proxy server.");
    app.addAppRunner(new ProxyServerAppRunner);
    if (app.parse(argc, (const char**)argv))
    {
        return app.run();
    }
    return 1;
}
