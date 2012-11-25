#include "firtex/config/Configurator.h"
#include "firtex/utility/Exception.h"

FX_NS_USE(utility);
FX_NS_USE(common);
FX_NS_DEF(config);

Configurator::Configurator()
    : m_eMode(TO_CONF)
{
}

Configurator::Configurator(Mode eMode)
   : m_eMode(eMode)
{
}

Configurator::Configurator(const Any& conf, Mode eMode)
    : m_eMode(eMode)
{
    m_confMap = AnyCast<ConfMap>(conf);
}

Configurator::~Configurator()
{
}

Configurator::Iterator Configurator::findConf(const tstring& sKey) const
{
    Configurator::ConfMapIterator it = m_confMap.begin();
    for (; it != m_confMap.end(); ++it)
    {
        if (it->first == sKey)
        {
            return Configurator::Iterator(this->getMap(), it);
        }
    }
    return Configurator::Iterator(this->getMap(), m_confMap.end());
}

FX_NS_END
