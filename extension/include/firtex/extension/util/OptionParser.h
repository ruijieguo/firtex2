#ifndef __FXOPTIONPARSER_H
#define __FXOPTIONPARSER_H

#include "firtex/common/StdHeader.h"
#include "firtex/extension/util/Option.h"
#include <vector>

FX_NS_DEF(util);

/**
 * @class OptionParser
 * This class represents and stores the properties
 * of a command line OptionParser.
 */
class OptionParser
{
public:
    OptionParser();
    ~OptionParser();
public:
    /**
     * add description header
     * @param sDesc header description 
     */
    void addDescription(const tstring& sDesc);

    /**
     * add option
     * @param sOption option to add 
     * @return added option
     */
    const Option& addOption(const Option& sOption);

    /**
     * get option by name
     * @param sName name of option
     * @return reference to option
     */
    const Option& getOption(const tstring& sName) const;

    /**
     * parse argument
     * @param argc number of argument in argv
     * @param argv argument string array
     * @exception throw MissingArgumentException if not all arguments matched.
     */
    void parse(int argc, const char* argv[]);

    /**
     * display help information
     * @param sCopyright copyright info
     */
    void displayHelp();
private:
    typedef std::vector<Option> OptionVector;

    OptionVector m_options;
    tstring m_sDescription;
};
FX_NS_END

#endif 
