#include "firtex/document/FieldType.h"
#include "firtex/document/Field.h"
#include "firtex/utility/Exception.h"
#include "firtex/common/Any.h"
#include "firtex/utility/StringUtils.h"

FX_NS_USE(config);
FX_NS_USE(utility);
FX_NS_USE(common);

FX_NS_DEF(document);

FieldType::FieldType()
    : m_eValueType(FieldType::UNKNOWN)
{
}

FieldType::FieldType(const std::string& sName, ValueType eValueType)
    : m_sName(sName)
    , m_eValueType(eValueType)
{
}

FieldType::FieldType(const FieldType& other)
{
    m_sName = other.m_sName;
    m_eValueType = other.m_eValueType;
    m_indexType = other.m_indexType;
    m_forwardIndexType = other.m_forwardIndexType;
    m_searchType = other.m_searchType;
}

FieldType::~FieldType()
{
}

void FieldType::configure(FX_NS(config)::Configurator& conf)
{
    conf.configure("name", m_sName, "");

    if (conf.getMode() == Configurator::TO_CONF)    
    {
        std::string str = valueTypeToString(m_eValueType);
        conf.configure("value_type", str, "");
    }
    else 
    {
        std::string str;
        conf.configure("value_type", str, "");
        m_eValueType = stringToValueType(str);
    }

    if (conf.getMode() == Configurator::TO_CONF)
    {
        if (!m_indexType.identifier.empty())
        {
            Configurator c;
            m_indexType.configure(c);
            conf.configure("index_type", c.getMap());    
        }

        if (!m_forwardIndexType.identifier.empty())
        {
            Configurator c;
            m_forwardIndexType.configure(c);
            conf.configure("forward_index_type", c.getMap());    
        }

        if (!m_searchType.analyzer.identifier.empty())
        {
            Configurator c;
            m_searchType.configure(c);
            conf.configure("search_type", c.getMap());
        }
    }
    else
    {
        Configurator::Iterator it = conf.findConf("index_type");
        if (it.hasNext())
        {
            Configurator c(AnyCast<Configurator::ConfMap>(it.next().second));
            m_indexType.configure(c);
        }

        it = conf.findConf("forward_index_type");
        if (it.hasNext())
        {
            Configurator c(AnyCast<Configurator::ConfMap>(it.next().second));
            m_forwardIndexType.configure(c);
        }

        it = conf.findConf("search_type");
        if (it.hasNext())
        {
            Configurator c(AnyCast<Configurator::ConfMap>(it.next().second));
            m_searchType.configure(c);
        }
    }    
}

void FieldType::setForwardIndex(bool bNeedForwardIndex)
{
    if (bNeedForwardIndex)
    {
        switch(m_eValueType)
        {
        case INT32:
            m_forwardIndexType.identifier = "fdindex.int32";
            break;
        case INT64:
            m_forwardIndexType.identifier = "fdindex.int64";
            break;
        case FLOAT:
            m_forwardIndexType.identifier = "fdindex.float";
			break;
        case DOUBLE:
            m_forwardIndexType.identifier = "fdindex.double";
            break;
        default:
            FIRTEX_THROW(UnsupportedException, "Unsupport forward index for field type: [%s]",
                         valueTypeToString(m_eValueType).c_str());
            break;
        }
    }
    else 
    {
        m_forwardIndexType.identifier.clear();
    }
}

//static 
std::string FieldType::valueTypeToString(FieldType::ValueType eValueType)
{
    switch(eValueType)
    {
    case FieldType::TEXT:
        return "TEXT";
    case FieldType::UNICODETEXT:
        return "UNICODETEXT";
    case FieldType::STRING:
        return "STRING";
    case FieldType::BINARY:
        return "BINARY";
    case FieldType::INT32:
        return "INT32";
    case FieldType::INT64:
        return "INT64";
    case FieldType::FLOAT:
        return "FLOAT";
    case FieldType::DOUBLE:
        return "DOUBLE";
    case FieldType::UNKNOWN:
        return "";
    default:
        return "";
    }
    return "";
}
 
//static 
FieldType::ValueType FieldType::stringToValueType(const std::string& sValueType)
{
    std::string str = toUpper(sValueType);
    if (sValueType == "TEXT")
    {
        return FieldType::TEXT;
    }
    else if (sValueType == "UNICODETEXT")
    {
        return FieldType::UNICODETEXT;
    }
    else if (sValueType == "STRING")
    {
        return FieldType::STRING;
    }
    else if (sValueType == "BINARY")
    {
        return FieldType::BINARY;
    }
    else if (sValueType == "INT32")
    {
        return FieldType::INT32;
    }
    else if (sValueType == "INT64")
    {
        return FieldType::INT64;
    }
    return FieldType::UNKNOWN;
}

///////////////////////////////////////////////////////////
//FieldType::Ananlyzer

void FieldType::Analyzer::configure(Configurator& conf)
{
    conf.configure("identifier", identifier, "");
    conf.configure("parameter", parameter, "");
    conf.configure("ante_filter", anteFilter, "");
    conf.configure("post_filter", postFilter, "");
}

///////////////////////////////////////////////////////////
//FieldType::IndexType

void FieldType::IndexType::configure(Configurator& conf)
{
    conf.configure("identifier", identifier, "");
    if (conf.getMode() == Configurator::TO_CONF)
    {
        Configurator aConf;
        analyzer.configure(aConf);
        
        conf.configure("analyzer", aConf.getMap());
    }
    else 
    {
        Configurator::Iterator it = conf.findConf("analyzer");
        if (it.hasNext())
        {
            Configurator aConf(AnyCast<Configurator::ConfMap>(it.next().second));
            analyzer.configure(aConf);
        }
    }
}

///////////////////////////////////////////////////////////
//FieldType::ForwardIndex

void FieldType::ForwardIndexType::configure(Configurator& conf)
{
    conf.configure("identifier", identifier, "");

    if (conf.getMode() == Configurator::TO_CONF)
    {
        if (!analyzer.isEmpty())
        {
            Configurator aConf;
            analyzer.configure(aConf);
        
            conf.configure("analyzer", aConf.getMap());
        }
    }
    else 
    {
        Configurator::Iterator it = conf.findConf("analyzer");
        if (it.hasNext())
        {
            Configurator aConf(AnyCast<Configurator::ConfMap>(it.next().second));
            analyzer.configure(aConf);
        }
    }
}

////////////////////////////////////////////////////////////
//FieldType::SearchType
void FieldType::SearchType::configure(Configurator& conf)
{
    if (conf.getMode() == Configurator::TO_CONF)
    {
        Configurator c;
        analyzer.configure(c);
        conf.configure("analyzer", c.getMap());
    }
    else
    {
        Configurator::Iterator it = conf.findConf("analyzer");
        if (it.hasNext())
        {
            Configurator c(AnyCast<Configurator::ConfMap>(it.next().second));
            analyzer.configure(c);
        }
    }
}    

FX_NS_END
