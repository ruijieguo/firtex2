#ifndef __FXDOCUMENTOVERVIEWAPPRUNNER_H
#define __FXDOCUMENTOVERVIEWAPPRUNNER_H

#include "firtex/common/StdHeader.h"
#include "firtex/document/Document.h"
#include "firtex/extension/util/AppRunner.h"
#include "firtex/index/ResultDoc.h"

FX_NS_DEF(util);
class Application;
FX_NS_END

FX_NS_DEF(app);

class DocumentOverviewAppRunner : public FX_NS(util)::AppRunner
{
public:
    DocumentOverviewAppRunner();
    Status run();
    void registerOption(FX_NS(util)::Application* pApp);
private:
    void optionCallback(const FX_NS(util)::Option& option, const tstring& sValue);
    void printDocument();
    void displayDoc(const FX_NS(index)::ResultDoc& resDoc);
private:
    tstring m_sIndexPath;
    docid_t m_nDocId;
};

FX_NS_END

#endif
