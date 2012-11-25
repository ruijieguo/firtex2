#include "firtex/index/BarrelDirectory.h"

FX_NS_USE(store);

FX_NS_DEF(index);

void BarrelDirectory::createBarrel(FileSystemPtr& pFileSys, const std::string& sSuffix)
{
    if (pFileSys->fileExists(sSuffix))
    {
        pFileSys->deleteDirectory(sSuffix, true);
    }
    pFileSys->makeDirectory(sSuffix);
}

FX_NS_END

