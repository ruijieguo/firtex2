#include "firtex/extension/util/Application.h"
#include "LexiconBuilderAppRunner.h"

#if defined(_MSC_VER)
#include <crtdbg.h>
#endif

FX_NS_USE(util);
FX_NS_USE(app);

int main(int argc, char* argv[])
{
    Application lexiBuilderApp(_T("lexibuilder"),
                               _T("Tools for building lexicon. "));
    lexiBuilderApp.addAppRunner(new LexiconBuilderAppRunner);
    if (lexiBuilderApp.parse(argc, (const char**)argv))
    {
        return lexiBuilderApp.run();
    }
    return 0;
}
