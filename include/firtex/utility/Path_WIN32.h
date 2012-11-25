#ifndef __PATH_WIN32_H
#define __PATH_WIN32_H

#include "firtex/common/StdHeader.h"
#include <vector>

FX_NS_DEF(utility)

class PathImpl
{
public:
	static tstring currentImpl();
	static tstring homeImpl();
	static tstring tempImpl();
	static tstring expandImpl(const tstring& path);
};

FX_NS_END

#endif //  __PATH_WIN32_H
