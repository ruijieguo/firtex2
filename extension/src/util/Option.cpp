#include "firtex/common/StdHeader.h"
#include "firtex/extension/util/Option.h"
#include "firtex/extension/util/OptionException.h"
#include <algorithm>

FX_NS_DEF(util);

Option::Option()
    : m_bRequired(false)
    , m_bRepeatable(false)
    , m_bArgRequired(false)
    , m_pCallback(NULL)
{
}

Option::Option(const std::string& fullName, const std::string& shortName)
    : m_sShortName(shortName)
    , m_sFullName(fullName)
    , m_bRequired(false)
    , m_bRepeatable(false)
    , m_bArgRequired(false)
    , m_pCallback(NULL)
{
}

Option::Option(const std::string& fullName, const std::string& shortName,
               const std::string& description, bool required)
    : m_sShortName(shortName)
    , m_sFullName(fullName)
    , m_sDescription(description)
    , m_bRequired(required)
    , m_bRepeatable(false)
    , m_bArgRequired(false)
    , m_pCallback(NULL)
{
}

Option::Option(const std::string& fullName, const std::string& shortName, 
               const std::string& description, bool required, 
               const std::string& argName, bool argRequired)
    : m_sShortName(shortName)
    , m_sFullName(fullName)
    , m_sDescription(description)
    , m_bRequired(required)
    , m_bRepeatable(false)
    , m_sArgName(argName)
    , m_bArgRequired(argRequired)
    , m_pCallback(NULL)
{
}

Option::Option(const Option& option):
    m_sShortName(option.m_sShortName),
    m_sFullName(option.m_sFullName),
    m_sDescription(option.m_sDescription),
    m_bRequired(option.m_bRequired),
    m_bRepeatable(option.m_bRepeatable),
    m_sArgName(option.m_sArgName),
    m_bArgRequired(option.m_bArgRequired),
    m_pCallback(option.m_pCallback)
{
    if (m_pCallback) m_pCallback = m_pCallback->clone();
}

Option::~Option()
{
    if (m_pCallback) 
    {
        delete m_pCallback;
        m_pCallback = NULL;
    }
}

Option& Option::shortName(const std::string& name)
{
    m_sShortName = name;
    return *this;
}

Option& Option::fullName(const std::string& name)
{
    m_sFullName = name;
    return *this;
}

Option& Option::description(const std::string& text)
{
    m_sDescription = text;
    return *this;
}

Option& Option::required(bool flag)
{
    m_bRequired = flag;
    return *this;
}

Option& Option::repeatable(bool flag)
{
    m_bRepeatable = flag;
    return *this;
}

Option& Option::argument(const std::string& name, bool required)
{
    m_sArgName     = name;
    m_bArgRequired = required;
    return *this;
}

Option& Option::noArgument()
{
    m_sArgName.clear();
    m_bArgRequired = false;
    return *this;
}

Option& Option::callback(const AbstractOptionCallback& cb)
{
    m_pCallback = cb.clone();
    return *this;
}

bool Option::matchShort(const std::string& option) const
{
    return option.length() > 0 
        && !m_sShortName.empty() && option.compare(0, m_sShortName.length(), m_sShortName) == 0;
}

bool Option::matchFull(const std::string& option) const
{
    std::string::size_type pos = option.find_first_of("=");
    std::string::size_type len = pos == std::string::npos ? option.length() : pos;
    return len == m_sFullName.length()
        && option.compare(0, len, m_sFullName, 0, len) == 0;
}

void Option::process(const std::string& option, const std::string& sArg ) const
{
    OptType optType = getOptionType(option);
    std::string sArgument;
    if (optType == Option::LONG_OPTION)
    {
        std::string::size_type pos = option.find_first_of("=");
        std::string::size_type len = (pos == std::string::npos) ? option.length() : pos;
        if (option.compare(2, len - 2, m_sFullName, 0, len) == 0)
        {
            if (takesArgument())
            {
                if (argumentRequired() && pos == std::string::npos)
                {
                    FIRTEX_THROW(MissingArgumentException, "--[%s] requires [%s]",
                            m_sFullName.c_str(), argumentName().c_str());
                }
                if (pos != std::string::npos)
                    sArgument.assign(option, pos + 1, option.length() - pos - 1);
                else
                    sArgument.clear();
            }
            else if (pos != std::string::npos)
            {
                throw UnexpectedArgumentException(option);
            }
            else sArgument.clear();
        }
    }
    else if(optType == Option::SHORT_OPTION)
    {
        OptType argType = getOptionType(sArg);
        if (takesArgument())
        {
            if (argumentRequired() && (sArg.empty() || argType != Option::BAD_OPTION))
            {
                FIRTEX_THROW(MissingArgumentException, "-[%s requires [%s]",
                        m_sFullName.c_str(), argumentName().c_str());
            }
            if (argType == Option::BAD_OPTION)
                sArgument = sArg;
            else 
                sArgument.clear();
        }
        else if (!sArg.empty() && argType == Option::BAD_OPTION)
        {
            throw UnexpectedArgumentException(option);
        }
        else sArgument.clear();
    }
    else 
    {
        throw UnexpectedArgumentException(option);
    }
    if (m_pCallback)
    {
        m_pCallback->invoke(*this, sArgument);
    }
}

//static
Option::OptType Option::getOptionType(const std::string& option)
{
    if (option.compare(0, 2, _T("--")) == 0)
    {
        return LONG_OPTION;
    }
    else if (option.compare(0, 1, _T("-")) == 0)
    {
        return SHORT_OPTION;
    }
    return BAD_OPTION;        
}

FX_NS_END
