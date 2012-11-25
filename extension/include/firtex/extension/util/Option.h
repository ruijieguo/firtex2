#ifndef __FXOPTION_H
#define __FXOPTION_H

#include "firtex/common/StdHeader.h"
#include "firtex/extension/util/OptionCallback.h"
#include <string>

FX_NS_DEF(util);

/**
 * @class Option
 * This class represents and stores the properties
 * of a command line option.
 */
class Option
{
public:
    typedef int32_t OptType;
    const static int BAD_OPTION = 0;
    const static int LONG_OPTION = 1;
    const static int SHORT_OPTION = 2;

public:
    Option();
    Option(const Option& option);

    /** Creates an option with the given properties.*/
    Option(const std::string& fullName, const std::string& shortName);

    /** Creates an option with the given properties.*/
    Option(const std::string& fullName, const std::string& shortName,
           const std::string& description, bool required = false);

    /** Creates an option with the given properties.*/
    Option(const std::string& fullName, const std::string& shortName, 
           const std::string& description, bool required, 
           const std::string& argName, bool argRequired = false);

    ~Option();

public:
    /** Sets the short name of the option.*/		
    Option& shortName(const std::string& name);

    /** Sets the full name of the option.*/	
    Option& fullName(const std::string& name);

    /** Sets the description of the option. */
    Option& description(const std::string& text);

    /**
     * Sets whether the option is required or not 
     * @param bRequired required if true, otherwise optional.
     */
    Option& required(bool bRequired);
    
    /**
     * Sets whether the option can be specified more than once
     * @param bRep more than once if true, otherwise
     * at most once.
     */
    Option& repeatable(bool bRep);

    /**
     * Specifies that the option takes an (optional or required)
     * argument.		
     * @param sArgName argument name
     * @param bRequired the argument is required or optional
     */
    Option& argument(const std::string& sArgName, bool bReq = true);

    /**
     * Specifies that the option does not take an argument (default).
     */		
    Option& noArgument();

    /** 
     * Binds the option to the given method.
     * The callback method will be called when the option
     * has been specified on the command line.
     */
    Option& callback(const AbstractOptionCallback& cb);

    /** Returns the short name of the option.*/
    const std::string& shortName() const;

    /** Returns the full name of the option.*/
    const std::string& fullName() const;

    /** Returns the description of the option.*/
    const std::string& description() const;

    /** @return Return true if the option is required, false if not.*/
    bool required() const;
    
    /**
     * @return Return true if the option can be specified more than
     * once, or false if at most once.
     */
    bool repeatable() const;

    /**
     * Returns true if the options takes an (optional) argument.
     */
    bool takesArgument() const;

    /**
     * Returns true if the argument is required.
     */
    bool argumentRequired() const;

    /**
     * Returns the argument name, if specified.
     */
    const std::string& argumentName() const;
    
    /**
     * Returns the option group the option is part of,
     * or an empty string, if the option is not part of
     * a group.
     */
    const std::string& group() const;
    
    /**
     * get option callback method
     * @return return a pointer to the callback method for the option,
     * or NULL if no callback has been specified.
     */
    AbstractOptionCallback* callback() const;

    /**
     * Match short name
     * @param sOption option string
     * @return return true if the given option string matches the
     * short name.
     *
     * The first characters of the option string must match
     * the short name of the option (case sensitive),
     * or the option string must partially match the full
     * name (case insensitive).
     */
    bool matchShort(const std::string& sOption) const;
    
    /**
     * Match full name 
     * @param sOption option string
     * @return return true if the given option string match the
     * full name.
     *
     * The option string must match the full
     * name (case insensitive).
     */
    bool matchFull(const std::string& sOption) const;

    /**
     * Verifies that the given option string matches the
     * requirements of the option, and extracts the option argument,
     * if present.
     * 
     * @param sOption option string 
     * @param sArg if the option string is okay and carries an argument,
     * the argument is returned in sArg.
     *
     * Throws a MissingArgumentException if a required argument
     * is missing. Throws an UnexpectedArgumentException if an
     * argument has been found, but none is expected. 
     */
    void process(const std::string& option, const std::string& sArg ) const;

public:
    /*
     * get option type
     * @return option type, "--": prefix of LONG_OPTION, "-": prefix of SHORT_OPTION, 
     * otherwise, BAD_OPTION returned.
     */
    static OptType getOptionType(const std::string& option);

private:
    std::string m_sShortName;
    std::string m_sFullName;
    std::string m_sDescription;
    bool        m_bRequired;
    bool        m_bRepeatable;
    std::string m_sArgName;
    bool        m_bArgRequired;
    AbstractOptionCallback* m_pCallback;
};


////////////////////////////////////////////////
// inlines
//

inline const std::string& Option::shortName() const
{
    return m_sShortName;
}
	
inline const std::string& Option::fullName() const
{
    return m_sFullName;
}
	
inline const std::string& Option::description() const
{
    return m_sDescription;
}

inline bool Option::required() const
{
    return m_bRequired;
}

inline bool Option::repeatable() const
{
    return m_bRepeatable;
}

inline bool Option::takesArgument() const
{
    return !m_sArgName.empty();
}

inline bool Option::argumentRequired() const
{
    return m_bArgRequired;
}

inline const std::string& Option::argumentName() const
{
    return m_sArgName;
}

inline AbstractOptionCallback* Option::callback() const
{
    return m_pCallback;
}

FX_NS_END

#endif 
