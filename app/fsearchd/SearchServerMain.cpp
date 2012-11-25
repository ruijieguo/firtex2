#include "firtex/extension/util/ServerApplication.h"
#include "SearchServerAppRunner.h"

FX_NS_USE(app);
FX_NS_USE(util);

int main(int argc,char* argv[])
{
    ServerApplication app("SearchServer", "FirteX search server.");
    app.addAppRunner(new SearchServerAppRunner);
    if (app.parse(argc, (const char**)argv))
    {
        return app.run();
    }
    return 1;
}
