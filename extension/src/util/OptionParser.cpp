#include "firtex/extension/util/OptionParser.h"
#include "firtex/extension/util/OptionException.h"

using namespace std;

FX_NS_DEF(util);

OptionParser::OptionParser() 
{
}

OptionParser::~OptionParser()
{
}

const Option& OptionParser::addOption(const Option& sOption)
{
    OptionVector::const_iterator it = m_options.begin();
    OptionVector::const_iterator itEnd = m_options.end();
    for (; it != itEnd; ++it)
    {
        if (it->fullName() == sOption.fullName())
        {
            throw DuplicateOptionException(it->fullName());
        }
    }
    m_options.push_back(sOption);
    return m_options.back();
}

void OptionParser::addDescription(const tstring& sDesc)
{
    m_sDescription.append(sDesc);
    m_sDescription.append(_T("\n"));
}

const Option& OptionParser::getOption(const std::string& sName) const
{
    Option::OptType optType = Option::getOptionType(sName);
    if (optType == Option::BAD_OPTION)
    {
        throw UnexpectedArgumentException(sName);
    }
    bool bMatchShort = (optType == Option::SHORT_OPTION);
    std::string sRealName(bMatchShort ? (sName.begin() + 1) : (sName.begin() + 2),
                          sName.end());

    const Option* pOption = NULL;
    for (OptionVector::const_iterator it = m_options.begin();
         it != m_options.end(); it++)
    {
        if ((bMatchShort && it->matchShort(sRealName))
            || (!bMatchShort && it->matchFull(sRealName)))
        {
            pOption = &*it;
            break;
        }
    }
    if (pOption)
        return *pOption;
    else
        throw UnknownOptionException(sName);
}

void OptionParser::parse(int argc, const char* argv[])
{
    for (int i = 1; i < argc; i++) 
    {
        const Option& opt = getOption(argv[i]);
        if (i == argc - 1)
        {
            opt.process(argv[i], _T(""));
        }            
        else 
        {
            Option::OptType optType = Option::getOptionType(argv[i + 1]);
            if (optType == Option::BAD_OPTION)
            {
                opt.process(argv[i], argv[i + 1]);
                if (opt.takesArgument())
                {
                    i++;
                }
            }
            else 
            {
                opt.process(argv[i], _T(""));
            }                    
        }
    }
}

void OptionParser::displayHelp()
{
    cout << m_sDescription << endl;
    cout << _T("Options: ") << endl;
    for (OptionVector::const_iterator it = m_options.begin(); 
         it != m_options.end(); it++)
    {
        size_t nOuted = 0;
        const Option& opt = *it;
        cout << _T("    ");
        nOuted += 4;
        if (!opt.shortName().empty())
        {
            cout << _T("-") << opt.shortName();
            nOuted += (opt.shortName().length() + 1);
        }
        else 
        {
            cout << _T("  ");
            nOuted += 2;
        }
        if (!opt.fullName().empty())
        {
            if (!opt.shortName().empty())
            {
                cout << _T(", ");
            }
            else 
            {
                cout << _T("  ");
            }
            cout << _T("--") << opt.fullName();
            nOuted += (opt.fullName().length() + 2);
        }
        while (nOuted < 24)
        {
            cout << _T(" ");
            nOuted++;
        }
        cout << _T(" ") << opt.description() << endl;
    }
}

FX_NS_END
