#ifndef __FX_CONSOLEAPPENDER_H
#define __FX_CONSOLEAPPENDER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/LayoutAppender.h"
#include "firtex/thread/FastMutex.h"
#include <ostream>

FX_NS_DEF(common);

/**
 * @class 
 * @brief A Appender that writes to an ostream.
 */
class ConsoleAppender : public LayoutAppender
{
public:
    ConsoleAppender();
    ConsoleAppender(std::ostream& str);
    ~ConsoleAppender();

    DECLARE_APPENDER_CREATOR(ConsoleAppender, "console");

public:
    virtual void append(const LoggingEvent& event);
    virtual void flush();

    virtual void configure(FX_NS(config)::Configurator& conf);
		
private:
    static FX_NS(thread)::FastMutex ms_mutex;

    std::ostream& m_ostream;
    bool m_bAutoFlush;
};

//////////////////////////////////////////////////////////
///

FX_NS_END

#endif
