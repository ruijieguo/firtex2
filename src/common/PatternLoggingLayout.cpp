#include "firtex/common/PatternLoggingLayout.h"
#include "firtex/utility/NumberParser.h"
#include "firtex/utility/DateTime.h"
#include "firtex/common/Logger.h"
#include "firtex/config/Configurator.h"
#include <ctime>
#include <cmath>
#include <iomanip>
#include <memory>
#include <stdio.h>

FX_NS_USE(utility);
FX_NS_USE(config);

FX_NS_DEF(common);

struct StringLiteralComponent : public PatternLoggingLayout::PatternComponent 
{
    StringLiteralComponent(const std::string& literal) 
        : m_sLiteral(literal) 
    {}

    virtual void append(std::ostringstream& out, const LoggingEvent& event)
    {
        out << m_sLiteral;
    }

private:
    std::string m_sLiteral;
};

struct LoggerNameComponent : public PatternLoggingLayout::PatternComponent 
{
    LoggerNameComponent(const std::string& specifier) 
    {
        if (specifier == "") 
        {
            m_nPrecision = -1;
        }
        else 
        {
            if (!NumberParser::tryParseInt32(specifier, m_nPrecision))
            {
                FIRTEX_THROW(InvalidConfigException, "Bad logger pattern: [%s].",
                        specifier.c_str());
            }
        }
    }

    virtual void append(std::ostringstream& out, const LoggingEvent& event) 
    {
        if (m_nPrecision == -1)
        {
            out << event.loggerName;
        }
        else 
        {
            std::string::size_type begin = std::string::npos;
            for (int i = 0; i < m_nPrecision; i++) 
            {
                begin = event.loggerName.rfind('.', begin - 2);
                if (begin == std::string::npos) 
                {
                    begin = 0;
                    break;
                }
                begin++;
            }
            out << event.loggerName.substr(begin);
        }
    }

private:
    int32_t m_nPrecision;
};

struct MessageComponent : public PatternLoggingLayout::PatternComponent 
{
    virtual void append(std::ostringstream& out, const LoggingEvent& event) 
    {
        out << event.message;
    }
};

struct LoggerLevelComponent : public PatternLoggingLayout::PatternComponent 
{
    virtual void append(std::ostringstream& out, const LoggingEvent& event) 
    {
        out << LoggingLevel::levelToStr((Logger::LevelType)event.level);
    }
};

struct FileComponent : public PatternLoggingLayout::PatternComponent 
{
    virtual void append(std::ostringstream& out, const LoggingEvent& event) 
    {
        out << event.file;
    }
};

struct LineComponent : public PatternLoggingLayout::PatternComponent 
{
    virtual void append(std::ostringstream& out, const LoggingEvent& event) 
    {
        out << event.line;
    }
};

struct FunctionComponent : public PatternLoggingLayout::PatternComponent 
{
    virtual void append(std::ostringstream& out, const LoggingEvent& event) 
    {
        out << event.function;
    }
};

struct ThreadNameComponent : public PatternLoggingLayout::PatternComponent 
{
    virtual void append(std::ostringstream& out, const LoggingEvent& event) 
    {
        out << event.threadId;
    }
};

struct ProcessorTimeComponent : public PatternLoggingLayout::PatternComponent 
{
    virtual void append(std::ostringstream& out, const LoggingEvent& event) 
    {
        out << std::clock();
    }
};

struct TimestampComponent : public PatternLoggingLayout::PatternComponent 
{
public:
    TimestampComponent(std::string sTimeFormat) 
        : m_sTimeFormat(sTimeFormat)
    {
        if (sTimeFormat.empty())
        {
            //sortable format
            m_sTimeFormat = "%Y-%m-%d %H:%M:%S.%i";
        }
    }

    virtual void append(std::ostringstream& out, const LoggingEvent& event) 
    {
        DateTime dt(event.timestamp);
        std::string sTime;
        sTime.reserve(20);
        dt.format(sTime, m_sTimeFormat);
        out << sTime;
    }

private:
    std::string m_sTimeFormat;
};

struct SecondsSinceEpochComponent : public PatternLoggingLayout::PatternComponent 
{
    virtual void append(std::ostringstream& out, const LoggingEvent& event) 
    {
        out << event.timestamp.time();
    }
};

struct MillisSinceEpochComponent : public PatternLoggingLayout::PatternComponent 
{
    virtual void append(std::ostringstream& out, const LoggingEvent& event) 
    {
        int64_t t = event.timestamp.microseconds() -
                    LoggerBuilder::instance()->getStartTime().microseconds();
        out << t;
    }
};

struct FormatModifierComponent : public PatternLoggingLayout::PatternComponent 
{
    FormatModifierComponent(PatternLoggingLayout::PatternComponent* component,
                            size_t minWidth, size_t maxWidth, bool alignLeft)
        : m_pComponent(component) 
        , m_nMinWidth(minWidth)
        , m_nMaxWidth(maxWidth)
        , m_bAlignLeft(alignLeft) 
    {}

    virtual ~FormatModifierComponent() 
    {
        delete m_pComponent;
    }

    virtual void append(std::ostringstream& out, const LoggingEvent& event) 
    {
        std::ostringstream s;
        m_pComponent->append(s, event);
        std::string msg = s.str();
        if (m_nMaxWidth > 0 && m_nMaxWidth < msg.length()) 
        {
            msg.erase(m_nMaxWidth);
        }
        size_t fillCount = m_nMinWidth - msg.length();
        if (m_nMinWidth > msg.length()) 
        {
            if (m_bAlignLeft) 
            {
                out << msg << std::string(fillCount, ' ');
            } 
            else 
            {
                out << std::string(fillCount, ' ') << msg;
            }
        } 
        else 
        {
            out << msg;
        }
    }

private:
    PatternLoggingLayout::PatternComponent* m_pComponent;
    size_t m_nMinWidth;
    size_t m_nMaxWidth;
    bool m_bAlignLeft;
};

const char* PatternLoggingLayout::DEFAULT_CONVERSION_PATTERN = "%d [%l] %m%n";
const char* PatternLoggingLayout::BASIC_CONVERSION_PATTERN = "%R %p %c %x: %m%n";
const char* PatternLoggingLayout::TTCC_CONVERSION_PATTERN = "%r [%t] %p %c %x - %m%n";

PatternLoggingLayout::PatternLoggingLayout() 
{
    try 
    {
        setPattern(DEFAULT_CONVERSION_PATTERN);
    }
    catch(InvalidConfigException&) 
    {
    }
}

PatternLoggingLayout::~PatternLoggingLayout() 
{
    clearPattern();
}

void PatternLoggingLayout::configure(Configurator& conf)
{
    if (conf.getMode() == Configurator::FROM_CONF)
    {
        std::string sPattern;
        conf.configure("pattern", sPattern, "");
        if (!sPattern.empty())
        {
            setPattern(sPattern);
        }
    }
}

void PatternLoggingLayout::clearPattern() 
{
    for(ComponentVector::iterator it = m_components.begin();
        it != m_components.end(); ++it) 
    {
        delete (*it);
    }

    m_components.clear();
    m_sPattern = "";
}

void PatternLoggingLayout::setPattern(const std::string& sPattern)
{
    std::istringstream conversionStream(sPattern);
    std::string sLiteral;

    char ch;
    PatternLoggingLayout::PatternComponent* component = NULL;
    size_t minWidth = 0;
    size_t maxWidth = 0;
    clearPattern();
    while (conversionStream.get(ch)) 
    {
        if (ch == '%') 
        {
            // readPrefix;
            {
                char ch2;
                conversionStream.get(ch2);
                if ((ch2 == '-') || ((ch2 >= '0') && (ch2 <= '9'))) 
                {
                    conversionStream.putback(ch2);
                    conversionStream >> minWidth;
                    conversionStream.get(ch2);
                } 
                if (ch2 == '.') 
                {
                    conversionStream >> maxWidth;
                } 
                else 
                {
                    conversionStream.putback(ch2);
                }                        
            }
            if (!conversionStream.get(ch)) 
            {
                FIRTEX_THROW(InvalidConfigException, "unterminated conversion specifier in: "
                        "[%s] at index: [%d]", sPattern.c_str(), 
                        (int32_t)conversionStream.tellg());
            }
            std::string specPostfix = "";
            // read postfix
            {
                char ch2;
                if (conversionStream.get(ch2)) 
                {
                    if (ch2 == '{') 
                    {
                        while(conversionStream.get(ch2) && (ch2 != '}'))
                            specPostfix += ch2;
                    } 
                    else 
                    {
                        conversionStream.putback(ch2);
                    }
                }
            }
            switch (ch) 
            {
            case '%':
                sLiteral += ch;
                break;
            case 'm':
                component = new MessageComponent();
                break;
            case 'n':
            {
                std::ostringstream endline;
                endline << std::endl;
                sLiteral += endline.str();
            }
            break;
            case 'c':
                component = new LoggerNameComponent(specPostfix);
                break;
            case 'd':
                component = new TimestampComponent(specPostfix);
                break;
            case 'l':
                component = new LoggerLevelComponent();
                break;
            case 'F':
                component = new FileComponent();
                break;
            case 'L':
                component = new LineComponent();
                break;
            case 'f':
                component = new FunctionComponent();
                break;
            case 'r':
                component = new MillisSinceEpochComponent();
                break;
            case 'R':
                component = new SecondsSinceEpochComponent();
                break;
            case 't':
                component = new ThreadNameComponent();
                break;
            case 'u':
                component = new ProcessorTimeComponent();
                break;
            default:
                FIRTEX_THROW(InvalidConfigException, "unknown conversion specifier: "
                        "[%c] in [%s] at index: [%d]", ch, sPattern.c_str(), 
                        (int32_t)conversionStream.tellg());
            }
            if (component) 
            {
                if (!sLiteral.empty()) 
                {
                    m_components.push_back(new StringLiteralComponent(sLiteral));
                    sLiteral = "";
                }
                if ((minWidth != 0) || (maxWidth != 0)) 
                {
                    component = new FormatModifierComponent(component, 
                            std::abs((int)minWidth), maxWidth, (int)minWidth < 0);
                    minWidth = maxWidth = 0;
                }
                m_components.push_back(component);
                component = NULL;
            }
        } 
        else 
        {
            sLiteral += ch;
        }
    }
    if (!sLiteral.empty()) 
    {
        m_components.push_back(new StringLiteralComponent(sLiteral));
    }

    m_sPattern = sPattern;
}

std::string PatternLoggingLayout::getPattern() const 
{
    return m_sPattern;
}

void PatternLoggingLayout::format(std::ostringstream& message, const LoggingEvent& event) 
{
    for(ComponentVector::const_iterator i = m_components.begin();
        i != m_components.end(); ++i) 
    {
        (*i)->append(message, event);
    }
}

FX_NS_END

