#include "firtex/extension/plugin/Plugin.h"

FX_NS_USE(utility);

FX_NS_DEF(plugin);

SETUP_STREAM_LOGGER(plugin, Plugin);

Plugin::Plugin()
    : m_bEnable(false)
    , m_bInstalled(false)
{
}

Plugin::~Plugin()
{
}

bool Plugin::load(const XMLNodeWrapperPtr& pXMLPlugin)
{
    //plugin.xml∏Ò Ω£∫
    // <plugins>
    //	 <plugin>			
    //		<identifier>identifier</identifier>
    //		<clsid></clsid>
    //		<name></name>			
    //		<progid></progid>
    //		<versionindependentprogid></versionindependentprogid>
    //		<filename></filename>	
    //		<enable>true/false</enable>
    //	 </plugin>
    // </plugins>
    try
    {
        XMLNodeWrapperPtr pNode = pXMLPlugin->firstNode(_T("identifier"));
        m_identifier = pNode->getValue();
        pNode = pXMLPlugin->firstNode(_T("clsid"));
        tstring str = pNode->getValue();
        GUIDX::decode(str.c_str(),&m_clsid);
        pNode = pXMLPlugin->firstNode(_T("name"));
        m_name = pNode->getValue();
        pNode = pXMLPlugin->firstNode(_T("progid"));
        m_progid = pNode->getValue();
        pNode = pXMLPlugin->firstNode(_T("versionindependentprogid"));
        m_versionindependentprogid = pNode->getValue();
        pNode = pXMLPlugin->firstNode(_T("filename"));
        m_filename = pNode->getValue();
        pNode = pXMLPlugin->firstNode(_T("enable"));
        str = pNode->getValue();
        m_bEnable = (!strCompareNoCase(str.c_str(),("true")));

        m_bInstalled = false;				
    }
    catch (const BadXmlFormatException& e)
    {
        FX_STREAM_LOG(WARN) << _T("Load plugin.xml error: ") 
                            << e.what() << FIRTEX_ENDL;
        return false;
    }
    return true;			
}

FX_NS_END
