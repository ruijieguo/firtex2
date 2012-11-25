#ifndef Foundation_Path_UNIX_INCLUDED
#define Foundation_Path_UNIX_INCLUDED

#include "firtex/common/StdHeader.h"
#include <vector>

FX_NS_DEF(utility)

class PathImpl
{
public:
    static std::string currentImpl();
    static std::string homeImpl();
    static std::string tempImpl();
    static std::string expandImpl(const std::string& path);
};

FX_NS_END

#endif // Foundation_Path_UNIX_INCLUDED
