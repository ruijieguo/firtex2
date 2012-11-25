#ifndef __FX_ENVIRONMENT_H
#define __FX_ENVIRONMENT_H

#include "firtex/common/StdHeader.h"

FX_NS_DEF(utility)

/**
 * @class Environment
 * @brief provides access to environment variables
 * and some general system information.
 */
class Environment
{
public:
    /**
     * \if EN
     * Returns the value of the environment variable
     * @param name name of the environment variable
     * @return empty string if the variable does not exist.
     * \endif
     */
    static std::string get(const std::string& name);

    /**
     * \if EN
     * Returns the value of the environment variable
     * @param name name of the environment variable
     * @param defaultValue default value if the
     *  variable does not exist.
     * @return value of the environment variable,
     * or defaultValue if the variable does not exist.
     * \endif
     */
    static std::string get(const std::string& name, const std::string& defaultValue);

    /**
     * \if EN
     * @param name name of the environment variable
     * @return return true iff an environment variable
     * with the given name is defined.
     * \endif
     */
    static bool has(const std::string& name);

    /**
     * \if EN
     * Sets the environment variable with the given name
     * to the given value.
     * @param name name of the environment variable
     * @param value value of the environment variable
     * \endif
     */
    static void set(const std::string& name, const std::string& value);

    /**
     * \if EN
     * Returns the operating system name.
     * \endif
     */
    static std::string osName();

    /**
     * \if EN
     * Returns the operating system version.
     * \endif
     */
    static std::string osVersion();

    /**
     * \if EN
     * Returns the operating system architecture.
     * \endif
     */
    static std::string osArchitecture();
    
    /** 
     * \if EN
     * get node name of computer
     * \endif
     */
    static std::string nodeName();
};

FX_NS_END

#endif 
