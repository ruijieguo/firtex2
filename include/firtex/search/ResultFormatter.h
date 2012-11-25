#ifndef __FX_RESULT_FORMATTER_H
#define __FX_RESULT_FORMATTER_H

#include "firtex/common/StdHeader.h"
#include "firtex/search/QueryResult.h"
#include <sstream>

FX_NS_DEF(search);

class ResultFormatter
{
public:
    virtual ~ResultFormatter() {}

public:
    /**
     * Format query Result
     */
    virtual void format(const FX_NS(search)::QueryResult& result,
                        std::stringstream& ss) = 0;
};

FX_NS_END

#endif //__FX_RESULT_FORMATTER_H
