//
// Copyright(C) 2005--2011 FirteX Development Team. 
// All rights reserved.
// This file is part of FirteX (http://sourceforge.net/projects/firtex)
//
// Use of the FirteX is subject to the terms of the software license set forth in 
// the LICENSE file included with this software.
//
// Author  : Ruijie Guo
// Email   : ruijieguo@gmail.com
// Created : 2011-07-02 19:54:48

#ifndef __FX_PATTERNLOGGINGLAYOUT_H
#define __FX_PATTERNLOGGINGLAYOUT_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/common/LoggingLayout.h"

FX_NS_DEF(common);

class PatternLoggingLayout : public LoggingLayout
{
public:
    DECLARE_LAYOUT_CREATOR(PatternLoggingLayout, "pattern");

public:
    /// The default conversion pattern
    static const char* DEFAULT_CONVERSION_PATTERN;
    
    /// A conversion pattern equivalent to the BasicLayout.
    static const char* BASIC_CONVERSION_PATTERN;

    /// A conversion pattern equivalent to the TTCCLayout.
    static const char* TTCC_CONVERSION_PATTERN;

public:
    PatternLoggingLayout();
    ~PatternLoggingLayout();

public:        
    /**
     * Format the logging data to a string
     * Implement this method to create your own layout format.
     *
     * @param message appendable string 
     * @param event The LoggingEvent.
     */
    virtual void format(std::ostringstream& message, const LoggingEvent& event);

    /**
     * Set the format of log lines handled by this
     * PatternLoggingLayout. By default, set to "%%m%%n".<br>
     * Format characters are as follows:<br>
     * <li><b>%%</b> - a single percent sign</li>
     * <li><b>%%m</b> - the message</li>
     * <li><b>%%n</b> - the platform specific line separator</li>
     * <li><b>%%c</b> - the logger name </li>
     * <li><b>%%d</b> - the date\n
     *  Date format: The date format character may be followed by a date format 
     *  specifier enclosed between braces. For example, %%d{%%H:%%M:%%S,%%l} or %%d{%%d %%m %%Y %%H:%%M:%%S,%%l}.
     *  If no date format specifier is given then the following format is used:
     *  "Wed Jan 02 02:03:55 1980". The date format specifier admits the same syntax 
     *  as the ANSI C function strftime, with 1 addition. The addition is the specifier
     *  %%l for milliseconds, padded with zeros to make 3 digits.</li>
     * <li><b>%%l</b> - the logger level</li>
     * <li><b>%%F</b> - the file path </li>
     * <li><b>%%L</b> - the line number </li>
     * <li><b>%%f</b> - the function name </li>
     * <li><b>%%r</b> - milliseconds since this layout was created.</li>
     * <li><b>%%R</b> - seconds since Jan 1, 1970</li>
     * <li><b>%%t</b> - the thread name</li>
     * <li><b>%%u</b> - clock ticks since process start</li>
     *
     * @param sPattern the conversion pattern
     * @exception InvalidConfigException if the pattern is invalid
     */
    virtual void setPattern(const std::string& sPattern);

    virtual std::string getPattern() const;

    virtual void clearPattern();

    virtual void configure(FX_NS(config)::Configurator& conf);

    class PatternComponent 
    {
    public:
        inline virtual ~PatternComponent() {};
        virtual void append(std::ostringstream& out, const LoggingEvent& event) = 0;
    };

private:
    typedef std::vector<PatternComponent*> ComponentVector; 
    
    ComponentVector m_components;
    std::string m_sPattern;
};

FX_NS_END

#endif //__FX_PATTERNLOGGINGLAYOUT_H
