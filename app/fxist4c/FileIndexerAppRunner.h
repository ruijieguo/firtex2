#ifndef __FXFILEINDEXERAPPRUNNER_H
#define __FXFILEINDEXERAPPRUNNER_H

#include "firtex/common/StdHeader.h"
#include "firtex/extension/util/AppRunner.h"

FX_NS_DEF(util)
class Application;
FX_NS_END

FX_NS_DEF(app);

class FileIndexerAppRunner : public FX_NS(util)::AppRunner
{
public:
    FileIndexerAppRunner();
    Status run();
    void registerOption(FX_NS(util)::Application* pApp);
private:
    void optionCallback(const FX_NS(util)::Option& option, const tstring& sValue);
    void indexFile();
private:
    tstring m_sFilePath;
    tstring m_sIndexPath;
    tstring m_sFileType;
};

FX_NS_END

#endif
