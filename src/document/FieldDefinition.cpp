#include "firtex/document/FieldDefinition.h"
#include "firtex/utility/Exception.h"
#include "firtex/common/Any.h"

FX_NS_USE(config);
FX_NS_USE(utility);
FX_NS_USE(common);

FX_NS_DEF(document);

FieldDefinition::FieldDefinition()
{
}

FieldDefinition::FieldDefinition(const FieldDefinition& other)
{
    for (TypeVector::const_iterator it = other.m_types.begin();
         it != other.m_types.end(); ++it)
    { 
        addFieldType(*(*it));
    }  
}

FieldDefinition::~FieldDefinition()
{
    for (TypeVector::iterator it = m_types.begin();
         it != m_types.end(); ++it)
    { 
        delete *it;      
    }  
    m_types.clear();
    m_name2TypeMap.clear();
}

void FieldDefinition::addFieldType(const FieldType& fieldType)
{
    TypeMap::const_iterator it = m_name2TypeMap.find(fieldType.getName());
    if (it != m_name2TypeMap.end())
    {
        FIRTEX_THROW(IllegalArgumentException, _T("Field type duplicate: [%s]"), 
                     fieldType.getName().c_str());
    }
    FieldType* pFieldType = new FieldType(fieldType);
    m_types.push_back(pFieldType);
    m_name2TypeMap.insert(make_pair(pFieldType->getName(), pFieldType));
}

void FieldDefinition::configure(Configurator& conf)
{
    if (conf.getMode() == Configurator::TO_CONF)
    {
        Configurator ftRootConf;
        for (TypeVector::iterator it = m_types.begin(); it != m_types.end(); ++it)
        { 
            Configurator ftConf;
            (*it)->configure(ftConf);
            ftRootConf.configure(_T("field_type"), ftConf.getMap());
        }      
        conf.configure(_T("type_define"), ftRootConf.getMap());
    }
    else 
    {
        Configurator::Iterator it = conf.findConf(_T("type_define"));
        if (it.hasNext())
        {
            Configurator::ConfMap rootConfMap = 
                AnyCast<Configurator::ConfMap>(it.next().second);
            Configurator rootConf(rootConfMap);
            Configurator::Iterator it2 = rootConf.iterator();
            while (it2.hasNext())
            {
                Configurator::KeyValuePair kv = it2.next();
                if (kv.first == _T("field_type"))
                {
                    FieldType* pType = new FieldType;
                    Configurator::ConfMap ftMap = AnyCast<Configurator::ConfMap>(
                        kv.second);
                    Configurator ftConf(ftMap);
                    pType->configure(ftConf);
                    addFieldType(*pType);
                }
            }
            
        }
    }
}

const FieldType* FieldDefinition::getFieldType(const tstring& sTypeName) const
{
    TypeMap::const_iterator it = m_name2TypeMap.find(sTypeName);
    if (it != m_name2TypeMap.end())
    {
        return it->second;
    }
    return NULL;
}

const FieldType* FieldDefinition::getForwardIndexFieldType(
        FieldType::ValueType vt) const
{
    for (TypeVector::const_iterator it = m_types.begin();
         it != m_types.end(); ++it)
    {
        if ((*it)->hasForwardIndex() && vt == (*it)->getValueType())
        {
            return *it;
        }
    }
    return NULL;
}

FX_NS_END
