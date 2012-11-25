#include "firtex/config/XMLConfigurator.h"
#include "firtex/utility/Exception.h"
#include "firtex/utility/BinaryFile.h"
#include "firtex/config/GlobalConfig.h"
#include <memory>

#define CONFIGURE_TAG_NAME "configure"

using namespace std;
FX_NS_USE(utility);
FX_NS_USE(store);
FX_NS_USE(common);
FX_NS_DEF(config);

XMLConfigurator::XMLConfigurator()
{
}

XMLConfigurator::XMLConfigurator(Configurator::Mode eMode)
    : Configurator(eMode)
{
}

XMLConfigurator::XMLConfigurator(const Configurator::ConfMap& confMap)
    : Configurator(confMap)
{
}

XMLConfigurator::~XMLConfigurator()
{
}

void XMLConfigurator::load(const string& sCfgFile)
{
    FIRTEX_ASSERT2(sCfgFile.empty() == false);
	
    m_sPath = sCfgFile;
    extractDir();

    XMLDocumentWrapper xmlDoc;
    xmlDoc.parseFile(sCfgFile);
    load(xmlDoc);
}

void XMLConfigurator::load(XMLDocumentWrapper& xmlDoc)
{
    setMode(Configurator::FROM_CONF);
    XMLNodeWrapperPtr pNode = xmlDoc.firstNode(CONFIGURE_TAG_NAME);
    if (pNode.isNull())
    {
        FIRTEX_THROW(InvalidConfigException, "Load xml configure FAILED: no configure element.");
    }
    load(pNode, getMap());
}

void XMLConfigurator::load(const string& sCfgFile, FileSystemPtr& pFileSys)
{
    InputStreamPtr pInStream = pFileSys->openFile(sCfgFile);
    string str;
    str.resize((size_t)pInStream->getSize());
    pInStream->read((void*)str.c_str(), str.length());
    loadFromBuffer(str);
}

void XMLConfigurator::loadFromBuffer(const string& sXML)
{
    setMode(Configurator::FROM_CONF);
    XMLDocumentWrapper xmlDoc;
    xmlDoc.parse(sXML);
    XMLNodeWrapperPtr pRoot = xmlDoc.firstNode(CONFIGURE_TAG_NAME);
    if (pRoot.isNotNull())
    {
        load(pRoot, getMap());
    }
}

void XMLConfigurator::load(XMLNodeWrapperPtr& pNode, Configurator::ConfMap& conf)
{
    XMLNodeWrapperPtr pChild = pNode->firstNode();
    if (pChild.isNull())
    {
        Configurator c(conf, Configurator::TO_CONF);
        string sValue = pNode->getValue();
        c.configure((const string&)pNode->getName(), 
                    sValue, (const string&)_T(""));
        conf = c.getMap();
    }
    else if (pChild->nextSibling().isNull() 
             && pChild->type() == XMLDocumentWrapper::NODE_DATA)
    {
        Configurator c(conf, Configurator::TO_CONF);
        string sValue = pChild->getValue();
        c.configure((const string&)pNode->getName(), 
                    sValue, (const string&)_T(""));
        conf = c.getMap();
    }
    else 
    {
        Configurator::ConfMap confMap;
        for (XMLNodeWrapperPtr pSubNode = pNode->firstNode();
             pSubNode.isNotNull(); pSubNode = pSubNode->nextSibling())
        {
            load(pSubNode, confMap);
        }

        if (strCompareNoCase(pNode->getName(), CONFIGURE_TAG_NAME))
        {
            Configurator c(conf, Configurator::TO_CONF);
            c.configure(pNode->getName(), confMap);
            conf = c.getMap();
        }
        else 
        {
            conf = confMap;
        }
    }
}

void XMLConfigurator::save(const string& sCfgFile)
{
    m_sPath = sCfgFile;
    XMLDocumentWrapper xmlDoc;
    XMLNodeWrapperPtr pRoot = xmlDoc.appendNode(
            XMLDocumentWrapper::NODE_ELEMENT, CONFIGURE_TAG_NAME);
    save(xmlDoc, pRoot, getMap());

    //TODO: UTF-8 encoding
    xmlDoc.addDeclarationNode("UTF-8");
    xmlDoc.printToFile(m_sPath);
}

void XMLConfigurator::save(const string& sCfgFile, FileSystemPtr& pFileSys)
{
    XMLDocumentWrapper xmlDoc;
    XMLNodeWrapperPtr pRoot = xmlDoc.appendNode(
            XMLDocumentWrapper::NODE_ELEMENT, CONFIGURE_TAG_NAME);
    Iterator it = iterator();
    while (it.hasNext())
    {
        Configurator::KeyValuePair kv = it.next();

        if (kv.second.getType() == typeid(Configurator::ConfMap))
        {
            XMLNodeWrapperPtr pNode = xmlDoc.allocateNode(
                    XMLDocumentWrapper::NODE_ELEMENT, kv.first.c_str());
            save(xmlDoc, pNode, AnyCast<Configurator::ConfMap>(kv.second));
            pRoot->appendNode(pNode);
        }
        else 
        {
            string str = AnyCast<string>(kv.second);
            XMLNodeWrapperPtr pNode = xmlDoc.allocateNode(
                    XMLDocumentWrapper::NODE_ELEMENT, kv.first.c_str(), str);
            pRoot->appendNode(pNode);
        }
    }

    OutputStreamPtr pOutStream = pFileSys->createFile(sCfgFile);
    if (pOutStream.isNull())
    {
        FIRTEX_THROW(FileIOException, "Save configure [%s] FAILED ",
                     sCfgFile.c_str());
    }
    string str;
    xmlDoc.addDeclarationNode("UTF-8");
    xmlDoc.print(str);
    pOutStream->write((const void*)str.c_str(), str.length());
}

void XMLConfigurator::save(XMLDocumentWrapper& xmlDoc, 
                           XMLNodeWrapperPtr& pXML,
                           const Configurator::ConfMap& confMap)
{
    Configurator conf(confMap);
    Configurator::Iterator it = conf.iterator();
    while (it.hasNext())
    {
        Configurator::KeyValuePair kv = it.next();
        
        if (kv.second.getType() == typeid(Configurator::ConfMap))
        {
            XMLNodeWrapperPtr pNode = xmlDoc.allocateNode(
                    XMLDocumentWrapper::NODE_ELEMENT, kv.first.c_str());
            save(xmlDoc, pNode, AnyCast<Configurator::ConfMap>(kv.second));
            pXML->appendNode(pNode);
        }
        else 
        {
            string str = AnyCast<string>(kv.second);
            XMLNodeWrapperPtr pNode = xmlDoc.allocateNode(
                    XMLDocumentWrapper::NODE_ELEMENT, kv.first.c_str(), str);
            pXML->appendNode(pNode);
        }
    }
}

void XMLConfigurator::extractDir()
{
    if(m_sPath.empty() == false)
    {
        if ( m_sPath.rfind( '\\' ) != string::npos )
            m_sDir = m_sPath.substr(0,m_sPath.rfind( '\\' ) );
        else if ( m_sPath.rfind( '/' ) != string::npos )
            m_sDir = m_sPath.substr(0,m_sPath.rfind( '/' ) ); 
    }
    else 
    {
        m_sDir.clear();
    }
}

FX_NS_END
