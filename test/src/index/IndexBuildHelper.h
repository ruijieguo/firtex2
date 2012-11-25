#ifndef __FX_INDEX_BUILD_HELPER_H
#define __FX_INDEX_BUILD_HELPER_H

#include "firtex/common/SharedPtr.h"
#include "firtex/document/DocumentSchema.h"
#include "firtex/index/IndexWriter.h"
#include <string>

FX_NS_DEF(index);

class IndexBuildHelper
{
public:
    static void buildIndex(const std::string& sRootPath, 
                           const FX_NS(document)::DocumentSchema& schema,
                           const std::string& fieldsStr, bool bInc);

    static void buildIndex(IndexWriterPtr& pIndexWriter,
                           const std::string& fieldsStr);

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif
