//
// Copyright(C) 2005--2009 Guo Ruijie
// All rights reserved.
// This file is part of FirteX (www.firtex.org)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software, and also available at
// http://www.firtex.org/license.html
//
// Author	: Ruijie Guo
// Email	: ruijieguo@gmail.com
// Created	: 2006/5/6
// Modified	: 2009/1/18
//

#ifndef __FX_LOGGER_H
#define __FX_LOGGER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/common/Appender.h"
#include "firtex/common/LoggingLevel.h"
#include "firtex/common/LoggingEvent.h"
#include "firtex/utility/DateTime.h"
#include "firtex/utility/Singleton.h"
#include "firtex/utility/Exception.h"
#include "firtex/thread/FastMutex.h"
#include "firtex/config/Configurable.h"

#include <map>
#include <vector>
#include <cstddef>
#include <istream>
#include <stdio.h>

#ifdef FX_WINDOWS
#pragma warning(push)
#pragma warning(disable:4355)
#endif

FX_NS_DEF(common);

class LoggerBuilder;

class LoggerBase : public FX_NS(config)::Configurable 
{
public:
    typedef LoggingLevel::LevelType LevelType;

    const static size_t MAX_LOG_MSG_LENGTH = 1024;

public:
    ~LoggerBase();

    /**
     * Return the name of the logger
     */
    const std::string& getName() const;

    /**
     * Attach the given Appender to the Logger.
     */
    void setAppender(const AppenderPtr& pAppender) 
    {
        m_pAppender = pAppender;
    }

    /** Return the Appender attached to the logger.*/
    AppenderPtr getAppender() const {return m_pAppender;}

    /** Sets the Logger's log level.*/
    void setLevel(LevelType level) { m_level = level; }

    /** Return the Logger's log level.*/
    LevelType getLevel() const;

    /**
     * Set the Logger's log level using a symbolic value.
     * @param sLevel level's symbolic value, valid values are:
     *   - fatal
     *   - critical
     *   - error
     *   - warn
     *   - notice
     *   - info
     *   - debug
     *   - trace
     */
    void setLevel(const std::string& sLevel);

    /** Log the given exception with LEVEL_ERROR level. */
    void log(const FX_NS(utility)::FirteXException& exc);

    /** Log the given message with given level. */
    void log(LevelType level, const std::string& msg);
    void log(LevelType level, const char* msg, ...);
    void log(LevelType level, const char* file, int32_t line, 
             const char* func, const char* msg, ...);
    void valog(LevelType level, const char* msg, va_list va);
    void valog(LevelType level, const char* file,
               int32_t line, const char* func, 
               const char* msg, va_list va);
    void doLog(const LoggingEvent& event);

    /**
     * If the Logger's log level is at least LEVEL_FATAL,
     * sends the given text to the attached appender.
     */
    void fatal(const std::string& msg);

    /**
     * If the Logger's log level is at least LEVEL_CRITICAL,
     * sends the given text to the attached appender.
     */
    void critical(const std::string& msg);

    /**
     * If the Logger's log level is at least LEVEL_ERROR,
     * sends the given text to the attached appender.
     */
    void error(const std::string& msg);

    /**
     * If the Logger's log level is at least LEVEL_WARN,
     * sends the given text to the attached appender.
     */
    void warning(const std::string& msg);

    /**
     * If the Logger's log level is at least LEVEL_NOTICE,
     * sends the given text to the attached appender.
     */
    void notice(const std::string& msg);

    /**
     * If the Logger's log level is at least LEVEL_INFO,
     * sends the given text to the attached appender.
     */
    void information(const std::string& msg);

    /**
     * If the Logger's log level is at least LEVEL_DEBUG,
     * sends the given text to the attached appender.
     */
    void debug(const std::string& msg);

    /**
     * If the Logger's log level is at least LEVEL_TRACE,
     * sends the given text to the attached appender.
     */
    void trace(const std::string& msg);

    /* Return true if at least the given log level is set.*/
    bool isEnabled(LoggerBase::LevelType level) const;

    /* Flush log text to device */
    void flush();

protected:
    LoggerBase(const std::string& name, 
               const AppenderPtr& pAppender,
               LevelType level);

protected:
    LoggerBase();
    LoggerBase(const LoggerBase&);
    LoggerBase& operator = (const LoggerBase&);

protected:
    std::string m_sName;
    AppenderPtr m_pAppender;
    LevelType m_level;

private:
    static tchar PREFIX[9][20];
};

class Logger : public LoggerBase
{
public:
    using LoggerBase::LevelType;

protected:
    Logger(LoggerBuilder& builder, const std::string& name,
           const AppenderPtr& pAppender, LevelType level)
        : LoggerBase(name, pAppender, level)
        , m_loggerBuilder(builder)
    {
    }

    Logger(LoggerBuilder& builder)
        : m_loggerBuilder(builder)
    {
    }

public:
    /**
     * Configure the logger
     */
    void configure(FX_NS(config)::Configurator& conf);

protected:
    Logger(const Logger&);
    Logger& operator = (const Logger&);

    friend class LoggerBuilder;

private:
    LoggerBuilder& m_loggerBuilder;
};

DEFINE_TYPED_PTR(Logger);

//////////////////////////////////////////////////////////////////////
//LoggerBuilder
#define ROOT_LOGGER "/"

class LoggerBuilder : public FX_NS(utility)::Singleton<LoggerBuilder>
                    , public FX_NS(config)::Configurable
{
public:
    typedef std::map<std::string, LoggerPtr> LoggerMap;
    typedef std::map<std::string, AppenderPtr> AppenderMap;

    friend class Logger;

public://protected:
    LoggerBuilder();
    ~LoggerBuilder();

    DECLARE_STATIC_SINGLETON(LoggerBuilder);

private:
    LoggerBuilder(const LoggerBuilder&);

public:
    /**
     * Configure the logger by configure file
     */
    void configure(const std::string& sConfFile);

    /**
     * Configure the logger
     */
    void configure(FX_NS(config)::Configurator& conf);

    /** 
     * Set the given log level on all loggers that are
     * descendants of the Logger with the given name.
     */
    void setLevel(const std::string& name, Logger::LevelType level);

    /** 
     * Set the given log level using a synbolic value 
     * on all loggers that are descendants of the Logger
     * with the given name.
     * @param sLevel level's symbolic value, valid values are:
     *   - fatal
     *   - critical
     *   - error
     *   - warn
     *   - notice
     *   - info
     *   - debug
     *   - trace		
     */
    void setLevel(const std::string& name, const std::string& sLevel);
    void setLevelUnsafe(const std::string& name, const std::string& sLevel);

    /**
     * Attach the given appender to all loggers that are
     * descendants of the Logger with the given name.
     */
    void setAppender(const std::string& name, const AppenderPtr& pAppender);

    /**
     * Attach the console appender to all loggers that are
     * descendants of the Logger with the given name.
     */
    void setConsoleAppender(const std::string& name = ROOT_LOGGER);

    /**
     * Attach the file appender to all loggers that are
     * descendants of the Logger with the given name.
     * @param name name of logger
     * @param sFilePath file path for file appender
     */
    void setFileAppender(const std::string& name,
                         const std::string& sFilePath);

    /**
     * Return a reference to the Logger with the given name.
     * If the Logger does not yet exist, it is created, based
     * on its parent logger.
     */
    Logger& getLogger(const std::string& name);

    /**
     * Return a reference to the parent logger of the logger with 
     * the given name
     */
    Logger& parentLogger(const std::string& name);


    /**
     * Create and return a reference to a Logger with the
     * given name. The Logger's Appender and log level as set as
     * specified.
     */
    Logger& createLogger(const std::string& name, const AppenderPtr& pAppender,
                         Logger::LevelType level = LoggingLevel::LEVEL_INFO);

    /**
     * Return a reference to the root logger, which is the ultimate
     * ancestor of all Loggers.
     */
    Logger& rootLogger();

    /**
     * Return a pointer to the Logger with the given name if it
     * exists, or a null pointer otherwse.
     */
    LoggerPtr hasLogger(const std::string& name);

    /**
     * Destroy the logger with the specified name. Does nothing
     * if the logger is not found.
     */
    void destroyLogger(const std::string& name);

    /**
     * Flush the logger with the specified name. 
     */
    void flush(const std::string& name);

    /**
     * Shut down the logging framework and releases all
     * Loggers.
     */
    void shutdown();

    /**
     * List all currently defined loggers
     */
    void listLoggers(std::vector<std::string>& names);

    /// Find or create appender
    AppenderPtr findAppender(const std::string& sAppName);
    AppenderPtr findAppenderUnsafe(const std::string& sAppName);

    /// Return the start time of this logger instance
    const FX_NS(utility)::Timestamp& getStartTime() const
    {
        return m_startTime;
    }

protected:
    /**
     * Attach the given appender to all loggers that are
     * descendants of the Logger with the given name.
     */
    void setAppenderUnsafe(const std::string& name, const AppenderPtr& pAppender);

    /*
     * Return a reference to the Logger with the specified name.
     * If the Logger does not yet exist, it is created, based
     * on its parent logger. This method is not thread-safe
     */
    Logger& getLoggerUnsafe(const std::string& name);

    /**
     * Return a reference to the parent logger of the logger with 
     * the given name. This method is not thread-safe
     */
    Logger& parentLoggerUnsafe(const std::string& name);

    void addLogger(LoggerPtr& pLogger);

    LoggerPtr findLogger(const std::string& name);

    void configureAppender(const FX_NS(config)::Configurator& conf);
    void configureLogger(const FX_NS(config)::Configurator& conf);

private:
    LoggerMap m_loggerMap;
    AppenderMap m_appenderMap;
    FX_NS(thread)::FastMutex m_mapMtx;

    FX_NS(utility)::Timestamp m_startTime;
};

//////////////////////////////////////////////////////////////////////
// inline functions 
//
inline const std::string& LoggerBase::getName() const
{
    return m_sName;
}

inline LoggerBase::LevelType LoggerBase::getLevel() const
{
    return m_level;
}

inline void LoggerBase::log(LevelType level, const std::string& text)
{
    if (m_level >= level)
    {
        LoggingEvent event(getName(), text, level);
        doLog(event);
    }
}
     
inline void LoggerBase::log(LevelType level, const char* msg, ...)
{
    if (isEnabled(level))
    {
        va_list va;
        va_start(va, msg);
        valog(level, msg, va);
        va_end(va);
    }
}

inline void LoggerBase::log(LevelType level, const char* file, int32_t line, 
                        const char* func, const char* msg, ...)
{
    if (isEnabled(level))
    {
        va_list va;
        va_start(va, msg);
        valog(level, file, line, func, msg, va);
        va_end(va);
    }
}

inline void LoggerBase::valog(LevelType level, const char* msg, va_list va)
{
    char buffer[MAX_LOG_MSG_LENGTH];
    (void)VSNPRINTF(buffer, MAX_LOG_MSG_LENGTH, msg, va);
    
    LoggingEvent event(getName(), buffer, level);
    doLog(event);
}

inline void LoggerBase::valog(LevelType level, const char* file,
                          int32_t line, const char* func, 
                          const char* msg, va_list va)
{
    char buffer[MAX_LOG_MSG_LENGTH];
    (void)VSNPRINTF(buffer, MAX_LOG_MSG_LENGTH, msg, va);
    
    LoggingEvent event(getName(), buffer, level,
                       file, line, func);
    doLog(event);
}

inline void LoggerBase::doLog(const LoggingEvent& event)
{
    if (m_pAppender)
    {
        m_pAppender->append(event);
    }
}

inline void LoggerBase::fatal(const std::string& msg)
{
    log(LoggingLevel::LEVEL_FATAL, msg);
}

inline void LoggerBase::critical(const std::string& msg)
{
    log(LoggingLevel::LEVEL_CRITICAL, msg);
}

inline void LoggerBase::error(const std::string& msg)
{
    log(LoggingLevel::LEVEL_ERROR, msg);
}

inline void LoggerBase::warning(const std::string& msg)
{
    log(LoggingLevel::LEVEL_WARN, msg);
}

inline void LoggerBase::notice(const std::string& msg)
{
    log(LoggingLevel::LEVEL_NOTICE, msg);
}

inline void LoggerBase::information(const std::string& msg)
{
    log(LoggingLevel::LEVEL_INFO, msg);
}

inline void LoggerBase::debug(const std::string& msg)
{
    log(LoggingLevel::LEVEL_DEBUG, msg);
}

inline void LoggerBase::trace(const std::string& msg)
{
    log(LoggingLevel::LEVEL_TRACE, msg);
}

inline bool LoggerBase::isEnabled(LevelType level) const
{
    return m_level >= level;
}

////////////////////////////////////////////////////////////////////////
///LoggerWrapper
class LoggerWrapper
{
public:
    LoggerWrapper(const std::string& loggerName)
        : m_logger(LoggerBuilder::instance()->getLogger(loggerName))
    {
    }

    ~LoggerWrapper() {}

public:
    bool isEnabled(Logger::LevelType level) const 
    {
        return m_logger.isEnabled(level); 
    }

    void log(const FX_NS(utility)::FirteXException& exc)
    {
        m_logger.log(exc);
    }

    void log(Logger::LevelType level, const std::string& text)
    {
        m_logger.log(level, text);
    }

    void log(Logger::LevelType level, const char* msg, ...)
    {
        if (m_logger.isEnabled(level))
        {
            va_list va;                                                                                                   
            va_start(va, msg);
            m_logger.valog(level, msg, va);
            va_end(va);
        }
    }

    void log(Logger::LevelType level, const char* file, int32_t line, 
             const char* func, const char* msg, ...)
    {
        if (m_logger.isEnabled(level))
        {
            va_list va;                                                                                                   
            va_start(va, msg);
            m_logger.valog(level, file, line, func, msg, va);
            va_end(va);
        }
    }

    void debug(const std::string& msg)
    {
        m_logger.debug(msg);
    }

    void trace(const std::string& msg)
    {
        m_logger.trace(msg);
    }

    void flush()
    {
        m_logger.flush();
    }

public:
    Logger& getLogger() { return m_logger; }

private:
    Logger& m_logger;
};

////////////////////////////////////////////////////////////////////////
/// LoggerStream
/**
 * @class LoggerStream
 * @brief This class implements a streambuf interface to a Logger.
 */
template <typename CharT, typename TraitsT>
class LoggerStreamBuf: public std::basic_streambuf<CharT, TraitsT>
{
public:
    typedef std::basic_streambuf<CharT, TraitsT> BaseStreamBuf;
    typedef std::basic_ios<CharT, TraitsT> BaseIOS;
    typedef CharT CharType;
    typedef TraitsT CharTraits;
    typedef typename BaseStreamBuf::int_type IntType;
    typedef typename BaseStreamBuf::pos_type PosType;
    typedef typename BaseStreamBuf::off_type OffType;
    typedef typename BaseIOS::openmode OpenMode;

public:
    LoggerStreamBuf(Logger& logger, Logger::LevelType level);
    ~LoggerStreamBuf() {}

public:
    virtual IntType overflow(IntType c)
    {
        if (c != CharTraits::eof()) 
        {
            return writeToDevice(CharTraits::to_char_type(c));
        }
        else
        {
            return c;
        }
    }

    virtual IntType underflow()
    {
        if (m_bIspb)
        {
            return m_pb;
        }
        else
        {
            IntType c = readFromDevice();
            if (c != CharTraits::eof())
            {
                m_bIspb = true;
                m_pb   = c;
            }
            return c;
        }
    }

    virtual IntType uflow()
    {
        if (m_bIspb)
        {
            m_bIspb = false;
            return m_pb;
        }
        else
        {
            IntType c = readFromDevice();
            if (c != CharTraits::eof())
            {
                m_pb = c;
            }
            return c;
        }
    }
    virtual IntType pbackfail(IntType c)
    {
        if (m_bIspb)
        {
            return CharTraits::eof();
        }
        else
        {
            m_bIspb = true;
            m_pb   = c;
            return c;
        }
    }

    /**
     * Some platforms (for example, Compaq C++) have buggy implementations of 
     * xsgetn that handle null buffers incorrectly.
     * Anyway, it does not hurt to provide an optimized implementation
     * of xsgetn for this streambuf implementation.
     */
//    virtual std::streamsize xsgetn(CharType* p, std::streamsize count);

public:
    inline Logger& getLogger() const;
    inline void setLevel(Logger::LevelType level);
    inline Logger::LevelType getLevel() const;

    /* Return true if at least the given log level is set.*/
    bool isEnabled(Logger::LevelType level) const { return getLogger().isEnabled(level);}
protected:
    inline static IntType charToInt(CharType c)
    {    return CharTraits::to_int_type(c);
    }

private:
    virtual IntType readFromDevice() { return CharTraits::eof();}
    virtual IntType writeToDevice(CharType c)
    {
        if (c == '\n' || c == '\r')
        {
            m_logger.log(m_level, m_message);
            m_message.clear();
        }
        else
        {
            m_message += c;
        }
        return c;
    }

private:
    IntType m_pb;
    bool m_bIspb;

    Logger& m_logger;
    Logger::LevelType m_level;
    std::string m_message;
};
/////////////////////////////////////////////////////////////////
///LoggerStreamBuf Implementation
template <typename CharT, typename TraitsT>
LoggerStreamBuf<CharT, TraitsT>::LoggerStreamBuf(Logger& logger, Logger::LevelType level)
    : m_pb(CharTraits::eof())
    , m_bIspb(false)
    , m_logger(logger)
    , m_level(level)
{
    this->setg(0, 0, 0);
    this->setp(0, 0);
}

/*template <typename CharT, typename TraitsT>
inline std::streamsize LoggerStreamBuf<CharT, TraitsT>::xsgetn(CharType* p, std::streamsize count)
{
    std::streamsize copied = 0;
    while (count > 0)
    {
        IntType c = uflow();
        if (c == CharTraits::eof()) break;
        *p++ = CharTraits::to_char_type(c);
        ++copied;
        --count;
    }
    return copied;
}*/

template <typename CharT, typename TraitsT>
inline Logger& LoggerStreamBuf<CharT, TraitsT>::getLogger() const
{
    return m_logger;
}

template <typename CharT, typename TraitsT>
inline void LoggerStreamBuf<CharT, TraitsT>::setLevel(Logger::LevelType level)
{
    m_level = level;
}

template <typename CharT, typename TraitsT>
inline Logger::LevelType LoggerStreamBuf<CharT, TraitsT>::getLevel() const
{
    return m_level;
}

////////////////////////////////////////////////////////////////
///LoggerIOS
template <typename CharT, typename TraitsT>
class LoggerIOS: public virtual std::ios
{
public:
    LoggerIOS(Logger& logger, Logger::LevelType level)
        : m_buf(logger, level)
    {
    }

    ~LoggerIOS() {}

public:
    inline Logger& getLogger() const {return m_buf.getLogger();}

public:
    LoggerStreamBuf<CharT, TraitsT>* rdbuf() { 	return &m_buf;}

protected:
    LoggerStreamBuf<CharT, TraitsT> m_buf;
};

////////////////////////////////////////////////////////////////
//LoggerStream
/**
 * The stream's buffer appends all characters written to it
 * to a string. As soon as a CR or LF (std::endl) is written,
 * the string is sent to the Logger, with the current logger level.
 *
 * Usage example:
 *     LoggerStream ls(aLogger);
 *     ls << "Here is informational message" << std::endl;
 *     ls.error() << "Here is error message here" << std::endl;
 */
template <typename CharT, typename TraitsT>
class LoggerStreamBase: public LoggerIOS<CharT, TraitsT>, public std::ostream
{
public:
    LoggerStreamBase(Logger& logger, Logger::LevelType level = LoggingLevel::LEVEL_INFO)
        : LoggerIOS<CharT, TraitsT>(logger, level)
        , std::ostream(&(this->m_buf))
    {
    }

    LoggerStreamBase(const std::string& loggerName, Logger::LevelType level = LoggingLevel::LEVEL_INFO)
        : LoggerIOS<CharT, TraitsT>(LoggerBuilder::instance()->getLogger(loggerName), level)
        , std::ostream(&(this->m_buf))
    {
    }

    ~LoggerStreamBase() {}

public:		
    /** Set the level for log messages to LoggingLevel::LEVEL_FATAL.*/
    LoggerStreamBase& fatal();

    /**
     * Set the level for log messages to LoggingLevel::LEVEL_FATAL
     * and writes the given message.
     */
    LoggerStreamBase& fatal(const std::string& message);

    /** Set the level for log messages to LoggingLevel::LEVEL_CRITICAL.*/
    LoggerStreamBase& critical();

    /**
     * Set the level for log messages to LoggingLevel::LEVEL_CRITICAL
     * and writes the given message.
     */
    LoggerStreamBase& critical(const std::string& message);

    /**
     * Set the level for log messages to LoggingLevel::LEVEL_ERROR.
     */
    LoggerStreamBase& error();

    /**
     * Set the level for log messages to LoggingLevel::LEVEL_ERROR
     * and writes the given message.
     */
    LoggerStreamBase& error(const std::string& message);

    /**
     * Set the level for log messages to LoggingLevel::LEVEL_WARN.
     */
    LoggerStreamBase& warning();

    /**
     * Set the level for log messages to LoggingLevel::LEVEL_WARN
     * and writes the given message.
     */
    LoggerStreamBase& warning(const std::string& message);

    /**
     * Set the level for log messages to LoggingLevel::LEVEL_NOTICE.
     */
    LoggerStreamBase& notice();

    /**
     * Set the level for log messages to LoggingLevel::LEVEL_NOTICE
     * and writes the given message.
     */
    LoggerStreamBase& notice(const std::string& message);

    /**
     * Set the level for log messages to LoggingLevel::LEVEL_INFO.
     */
    LoggerStreamBase& information();

    /**
     * Set the level for log messages to LoggingLevel::LEVEL_INFO
     * and writes the given message.
     */
    LoggerStreamBase& information(const std::string& message);

    /**
     * Set the level for log messages to LoggingLevel::LEVEL_DEBUG.
     */
    LoggerStreamBase& debug();

    /**
     * Set the level for log messages to LoggingLevel::LEVEL_DEBUG
     * and writes the given message.
     */
    LoggerStreamBase& debug(const std::string& message);

    /** Set the level for log messages to LoggingLevel::LEVEL_TRACE.*/
    LoggerStreamBase& trace();

    /**
     * Set the level for log messages to LoggingLevel::LEVEL_TRACE
     * and writes the given message.
     */
    LoggerStreamBase& trace(const std::string& message);

    /** Set the level for log messages.*/
    LoggerStreamBase& level(Logger::LevelType level);

    /** Log exception */
    void log(const FX_NS(utility)::FirteXException& exc);

    /** Log message */
    void log(Logger::LevelType level, const std::string& text);
    void log(Logger::LevelType level, const char* msg, ...);
    void log(Logger::LevelType level, const char* file, int32_t line, 
             const char* func, const char* msg, ...);

    /* Return true if at least the given log level is set.*/
    bool isEnabled(Logger::LevelType level) const;

    /* Flush log text to device */
    void flush();

public:
    Logger& getLogger();
};

////////////////////////////////////////////////////////////////////////
///LoggerStreamBase implementation
template <typename CharT, typename TraitsT>
inline LoggerStreamBase<CharT, TraitsT>& LoggerStreamBase<CharT, TraitsT>::fatal()
{
    return level(LoggingLevel::LEVEL_FATAL);
}

template <typename CharT, typename TraitsT>
inline LoggerStreamBase<CharT, TraitsT>& LoggerStreamBase<CharT, TraitsT>::fatal(const std::string& message)
{
    this->m_buf.getLogger().fatal(message);
    return level(LoggingLevel::LEVEL_FATAL);
}

template <typename CharT, typename TraitsT>
inline LoggerStreamBase<CharT, TraitsT>& LoggerStreamBase<CharT, TraitsT>::critical()
{
    return level(LoggingLevel::LEVEL_CRITICAL);
}

template <typename CharT, typename TraitsT>
inline LoggerStreamBase<CharT, TraitsT>& LoggerStreamBase<CharT, TraitsT>::critical(const std::string& message)
{
    this->m_buf.getLogger().critical(message);
    return level(LoggingLevel::LEVEL_CRITICAL);
}

template <typename CharT, typename TraitsT>
inline LoggerStreamBase<CharT, TraitsT>& LoggerStreamBase<CharT, TraitsT>::error()
{
    return level(LoggingLevel::LEVEL_ERROR);
}

template <typename CharT, typename TraitsT>
inline LoggerStreamBase<CharT, TraitsT>& LoggerStreamBase<CharT, TraitsT>::error(const std::string& message)
{
    this->m_buf.getLogger().error(message);
    return level(LoggingLevel::LEVEL_ERROR);
}

template <typename CharT, typename TraitsT>
inline LoggerStreamBase<CharT, TraitsT>& LoggerStreamBase<CharT, TraitsT>::warning()
{
    return level(LoggingLevel::LEVEL_WARN);
}

template <typename CharT, typename TraitsT>
inline LoggerStreamBase<CharT, TraitsT>& LoggerStreamBase<CharT, TraitsT>::warning(const std::string& message)
{
    this->m_buf.getLogger().warning(message);
    return level(LoggingLevel::LEVEL_WARN);
}

template <typename CharT, typename TraitsT>
inline LoggerStreamBase<CharT, TraitsT>& LoggerStreamBase<CharT, TraitsT>::notice()
{
    return level(LoggingLevel::LEVEL_NOTICE);
}

template <typename CharT, typename TraitsT>
inline LoggerStreamBase<CharT, TraitsT>& LoggerStreamBase<CharT, TraitsT>::notice(const std::string& message)
{
    this->m_buf.getLogger().notice(message);
    return level(LoggingLevel::LEVEL_NOTICE);
}

template <typename CharT, typename TraitsT>
inline LoggerStreamBase<CharT, TraitsT>& LoggerStreamBase<CharT, TraitsT>::information()
{
    return level(LoggingLevel::LEVEL_INFO);
}

template <typename CharT, typename TraitsT>
inline LoggerStreamBase<CharT, TraitsT>& LoggerStreamBase<CharT, TraitsT>::information(const std::string& message)
{
    this->m_buf.getLogger().information(message);
    return level(LoggingLevel::LEVEL_INFO);
}

template <typename CharT, typename TraitsT>
inline LoggerStreamBase<CharT, TraitsT>& LoggerStreamBase<CharT, TraitsT>::debug()
{
    return level(LoggingLevel::LEVEL_DEBUG);
}

template <typename CharT, typename TraitsT>
inline LoggerStreamBase<CharT, TraitsT>& LoggerStreamBase<CharT, TraitsT>::debug(const std::string& message)
{
    this->m_buf.getLogger().debug(message);
    return level(LoggingLevel::LEVEL_DEBUG);
}

template <typename CharT, typename TraitsT>
inline LoggerStreamBase<CharT, TraitsT>& LoggerStreamBase<CharT, TraitsT>::trace()
{
    return level(LoggingLevel::LEVEL_TRACE);
}

template <typename CharT, typename TraitsT>
inline LoggerStreamBase<CharT, TraitsT>& LoggerStreamBase<CharT, TraitsT>::trace(const std::string& message)
{
    this->m_buf.getLogger().trace(message);
    return level(LoggingLevel::LEVEL_TRACE);
}

template <typename CharT, typename TraitsT>
inline LoggerStreamBase<CharT, TraitsT>& LoggerStreamBase<CharT, TraitsT>::level(Logger::LevelType level)
{
    this->m_buf.setLevel(level);
    return *this;
}

template <typename CharT, typename TraitsT>
inline void LoggerStreamBase<CharT, TraitsT>::log(const FX_NS(utility)::FirteXException& exc)
{
    this->m_buf.getLogger().log(exc);
}

template <typename CharT, typename TraitsT>
inline void LoggerStreamBase<CharT, TraitsT>::log(Logger::LevelType level, const std::string& text)
{
    this->m_buf.getLogger().log(level, text);
}

template <typename CharT, typename TraitsT>
inline void LoggerStreamBase<CharT, TraitsT>::log(Logger::LevelType level, const char* msg, ...)
{
    if (isEnabled(level))
    {
        va_list va;                                                                                                   
        va_start(va, msg);
        this->m_buf.getLogger().valog(level, msg, va);
        va_end(va);
    }
}

template <typename CharT, typename TraitsT>
inline void LoggerStreamBase<CharT, TraitsT>::log(Logger::LevelType level,
        const char* file, int32_t line, const char* func, const char* msg, ...)
{
    if (isEnabled(level))
    {
        va_list va;                                                                                                   
        va_start(va, msg);
        this->m_buf.getLogger().valog(level, file, line, func, msg, va);
        va_end(va);
    }
}

template <typename CharT, typename TraitsT>
inline bool LoggerStreamBase<CharT, TraitsT>::isEnabled(Logger::LevelType level) const
{
    return this->m_buf.isEnabled(level);
}

template <typename CharT, typename TraitsT>
inline void LoggerStreamBase<CharT, TraitsT>::flush()
{
    this->m_buf.getLogger().flush();
}

template <typename CharT, typename TraitsT>
inline Logger& LoggerStreamBase<CharT, TraitsT>::getLogger()
{
    return this->m_buf.getLogger();
}

typedef LoggerStreamBase<char, std::char_traits<char> > LoggerStream;

////////////////////////////////////////////////////////////////////////
// convenience macros
//

#define FX_LOG(lvl, format, ...) ms_logger.log(FX_NS(common)::LoggingLevel::LEVEL_##lvl, __FILE__,\
            __LINE__, __FUNCTION__, format, ##__VA_ARGS__)

#if defined(_DEBUG)
#define FX_DEBUG(format, ...) ms_logger.log(FX_NS(common)::LoggingLevel::LEVEL_DEBUG, __FILE__,\
            __LINE__, __FUNCTION__, format, ##__VA_ARGS__)

#define FX_TRACE(format, ...) ms_logger.log(FX_NS(common)::LoggingLevel::LEVEL_TRACE, __FILE__, \
            __LINE__, __FUNCTION__, format, ##__VA_ARGS__)

#else
#define FX_DEBUG(format, ...)

#define FX_TRACE(format, ...)
#endif

#define DECLARE_LOGGER() static FX_NS(common)::LoggerWrapper ms_logger
#define SETUP_LOGGER(n, c) FX_NS(common)::LoggerWrapper c::ms_logger("firtex." #n "." #c)
#define SETUP_LOGGER_EMBED(n, c, sc) FX_NS(common)::LoggerWrapper c::sc::ms_logger("firtex." #n "." #c "." #sc)
#define SETUP_LOGGER_TEMP(n, c)                                         \
    template<typename T> FX_NS(common)::LoggerWrapper c<T>::ms_logger("firtex."#n"."#c)

#define SETUP_LOGGER_TEMP2(n, c)                                        \
    template<typename T1, typename T2> FX_NS(common)::LoggerWrapper c<T1, T2>::ms_logger("firtex."#n"."#c)

#define SETUP_LOGGER_TEMP3(n, c)                                        \
    template<typename T1, typename T2, typename T3> FX_NS(common)::LoggerWrapper c<T1, T2, T3>::ms_logger("firtex."#n"."#c)

#define FLUSH_LOG() ms_logger.flush()

#define FX_LOG_FLUSH_ALL() FX_NS(common)::LoggerBuilder::instance()->flush("")


///////////////////////////////////////////////////
/// stream logger macros
#define FX_STREAM_LOG(lvl)                                              \
    if (!(ms_logger).isEnabled(FX_NS(common)::LoggingLevel::LEVEL_##lvl)) (void) 0 ; else (ms_logger).level(FX_NS(common)::LoggingLevel::LEVEL_##lvl)

#define FIRTEX_ENDL std::endl

#define DECLARE_STREAM_LOGGER() static FX_NS(common)::LoggerStream ms_logger
#define SETUP_STREAM_LOGGER(n, c) FX_NS(common)::LoggerStream c::ms_logger("firtex."#n"."#c)
#define SETUP_STREAM_LOGGER_EMBED(n, c, sc) FX_NS(common)::LoggerStream c::sc::ms_logger("firtex."#n"."#c"."#sc)

#define SETUP_STREAM_LOGGER_TEMP(n, c)                                  \
    template<typename T> FX_NS(common)::LoggerStream c<T>::ms_logger("firtex."#n"."#c)

#define SETUP_STREAM_LOGGER_TEMP2(n, c)                                  \
    template<typename T1, typename T2> FX_NS(common)::LoggerStream c<T1, T2>::ms_logger("firtex."#n"."#c)

#define FLUSH_STREAM_LOG() ms_logger.flush()
	
FX_NS_END

#ifdef FX_WINDOWS
#pragma warning(pop)
#endif
#endif
