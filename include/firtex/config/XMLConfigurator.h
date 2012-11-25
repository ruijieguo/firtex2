#ifndef __XMLCONFIGURATOR_H
#define __XMLCONFIGURATOR_H

#include "firtex/config/Configurator.h"
#include "firtex/store/FileSystem.h"
#include "firtex/utility/XMLDocumentWrapper.h"

FX_NS_DEF(config);

class XMLConfigurator : public Configurator
{
public:
    XMLConfigurator();
    XMLConfigurator(Configurator::Mode eMode);
    XMLConfigurator(const Configurator::ConfMap& confMap);
    virtual ~XMLConfigurator();

public:
    /**
     * Load configurator file from file
     * @param sCfgFile file path
     */
    void load(const std::string& sCfgFile);

    /**
     * Load from file system
     */
    void load(const std::string& sCfgFile, FX_NS(store)::FileSystemPtr& pFileSys);

    /**
     * Load configure from xml document
     */
    void load(FX_NS(utility)::XMLDocumentWrapper& xmlDoc);

    /**
     * Load from buffer
     */
    void loadFromBuffer(const std::string& buf);

    /**
     * Save configure to file
     * @param sCfgFile file path
     */
    void save(const std::string& sCfgFile);

    /**
     * Save to file system
     */
    void save(const std::string& sCfgFile, FX_NS(store)::FileSystemPtr& pFileSys);

    /**
     * Save configurator file to XML fragment
     * @param xmlDoc xml document
     * @param pNode xml element node
     * @param confMap configurator map
     */
    void save(FX_NS(utility)::XMLDocumentWrapper& xmlDoc, 
              FX_NS(utility)::XMLNodeWrapperPtr& pNode,
              const Configurator::ConfMap& confMap);

    /**
     * get file path where configure is loaded 
     * @return file path
     */
    const std::string& getPath() const { return m_sPath; }

    /**
     * get directory of the configure file
     * @return file directory
     */
    const std::string& getDir() const { return m_sDir;}

protected:
    /**
     * Load configurator file from XML fragment
     * @param pXML xml fragment
     * @param conf result conf;
     */
    void load(FX_NS(utility)::XMLNodeWrapperPtr& pXML, Configurator::ConfMap& conf);

    void extractDir();

private:
    std::string m_sPath;
    std::string m_sDir;
};

FX_NS_END

#endif
