#ifndef __FX_CONFIGURABLE_H
#define __FX_CONFIGURABLE_H

#include "firtex/common/StdHeader.h"

FX_NS_DEF(config);

class Configurator;

/**
 * @class Configurable
 * @brief Base class for all configurable object
 */
class Configurable
{
public:
    virtual ~Configurable() {}

public:
    virtual void configure(Configurator& conf) = 0;
};

FX_NS_END
#endif
