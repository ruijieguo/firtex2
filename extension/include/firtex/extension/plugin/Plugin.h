#ifndef __FX_PLUGIN_H
#define __FX_PLUGIN_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/common/Logger.h"
#include "firtex/utility/XMLDocumentWrapper.h"
#include "ComObject.h"

FX_NS_DEF(plugin);

class Plugin
{
public:
    Plugin();
    ~Plugin();
public:
    bool load(const FX_NS(utility)::XMLNodeWrapperPtr& pXMLPlugin);

    const tstring& getIdentifier() const {return m_identifier;}
    void setIdentifier(const tstring& id) {m_identifier = id;}

    const FX_NS(com)::FX_CLSID&	getCLSID() const {return m_clsid;}
    tstring	getCLSIDAsString() const {return GUIDX::encode(&m_clsid);}

    void setCLSID(const tstring& sCLSID) { GUIDX::decode(sCLSID.c_str(), &m_clsid);}

    const tstring& getName() const {return m_name;}

    const tstring& getFileName() const {return m_filename;}
    void setFileName(const tstring& sFileName) { m_filename = sFileName;}

    const tstring& getProgid() const {return m_progid;}
    bool isEnabled() const {return m_bEnable;}
    bool isInstalled() const {return m_bInstalled;}

protected:			
    tstring m_identifier;
    FX_NS(com)::FX_CLSID m_clsid;
    tstring	m_name;		//Plugin name
    tstring	m_progid;
    tstring	m_versionindependentprogid;
    tstring	m_threadingmodel;
    tstring	m_filename;	//Plugin path
    bool m_bEnable;		//enable or not
    bool m_bInstalled;

    friend class Plugins;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(Plugin);

FX_NS_END

#endif
