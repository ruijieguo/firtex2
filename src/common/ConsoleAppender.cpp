#include "firtex/common/ConsoleAppender.h"
#include "firtex/config/Configurator.h"
#include "firtex/common/LoggingLayout.h"
#include <iostream>

FX_NS_USE(thread);
FX_NS_USE(config);

FX_NS_DEF(common);

FastMutex ConsoleAppender::ms_mutex;

ConsoleAppender::ConsoleAppender() 
  : m_ostream(std::clog)
  , m_bAutoFlush(false)
{
}

ConsoleAppender::ConsoleAppender(std::ostream& str)
  : m_ostream(str)
  , m_bAutoFlush(false)
{
}

ConsoleAppender::~ConsoleAppender()
{
}

void ConsoleAppender::configure(Configurator& conf)
{
    conf.configure("flush", m_bAutoFlush, false);
}

void ConsoleAppender::append(const LoggingEvent& event)
{
    FX_NS(thread)::FastMutex::Guard lock(ms_mutex);
    
    std::ostringstream out;
    LoggingLayout& layout = getLayout();
    layout.format(out, event);
    
    m_ostream << out.str();

    if (m_bAutoFlush || event.level <= LoggingLevel::LEVEL_WARN)
    {
        m_ostream.flush();
    }
}

void ConsoleAppender::flush()
{
    FastMutex::Guard lock(ms_mutex);
    m_ostream.flush();
}

FX_NS_END
