#ifndef __FX_THRIFTSEARCHBASE_H
#define __FX_THRIFTSEARCHBASE_H

#include "firtex/common/StdHeader.h"
#include "gen-cpp/ThriftSearchService.h"

FX_NS_DEF(app);

class ThriftSearchServiceHelper
{
public:
    static void queryCommandToStatmentString(std::string& sUri, 
            const QueryCommand& cmd);
    static void vectorToString(std::string& sResult, 
                               const std::vector<std::string>& strs,
                               const std::string& sSep);
};

FX_NS_END

#endif
