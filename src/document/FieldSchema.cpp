#include "firtex/document/FieldSchema.h"
#include "firtex/utility/NumberParser.h"

FX_NS_USE(utility);
FX_NS_USE(config);
FX_NS_USE(common);

FX_NS_DEF(document);

SETUP_STREAM_LOGGER(document, FieldSchema);
	
//////////////////////////////////////////////////////////////////////////
//
FieldSchema::FieldSchema()
    : m_id(INVALID_FIELDID)
    , m_bStore(false)
    , m_bSortAndFilterable(false)
    , m_bHasLengthNorm(false)
    , m_fBoost(1.0)
    , m_pFieldType(NULL)
{
}

FieldSchema::FieldSchema(const tstring& sName, const FieldType* pFieldType,
                         bool bStore, float fBoost)
    : m_id(INVALID_FIELDID)
    , m_sName(sName)
    , m_sType(pFieldType ? pFieldType->getName() : _T("TEXT"))
    , m_bStore(bStore)
    , m_bSortAndFilterable(false)
    , m_bHasLengthNorm(false)
    , m_fBoost(fBoost)
    , m_pFieldType(pFieldType)
{				
    m_bSortAndFilterable = hasForwardIndex();
}

FieldSchema::FieldSchema(const FieldSchema& src)
{
    m_id = src.m_id;
    m_sName = src.m_sName;
    m_sType = src.m_sType;
    m_bStore = src.m_bStore;
    m_fBoost = src.m_fBoost;

    m_bSortAndFilterable = src.m_bSortAndFilterable;
    m_bHasLengthNorm = src.m_bHasLengthNorm;

    m_pFieldType = src.m_pFieldType;
}

FieldSchema::~FieldSchema()
{
}

void FieldSchema::setFieldType(const FieldType* pFieldType)
{
    m_pFieldType = pFieldType;
    if (m_pFieldType)
    {
        m_sType = pFieldType->getName();
    }
    else 
    {
        m_sType = _T("TEXT");
    }
}

FieldSchema& FieldSchema::operator = (const FieldSchema& right)
{
    m_id = right.m_id;
    m_sName = right.m_sName;
    m_sType = right.m_sType;
    m_bStore = right.m_bStore;
    m_fBoost = right.m_fBoost;
    m_pFieldType = right.m_pFieldType;

    m_bSortAndFilterable = right.m_bSortAndFilterable;
    m_bHasLengthNorm = right.m_bHasLengthNorm;

    return (*this);
}

bool FieldSchema::isEqual(const FieldSchema& other) const 
{
    return (m_id == other.m_id) && (m_sName == other.m_sName) 
        && (m_sName == other.m_sName) && (m_pFieldType == other.m_pFieldType) 
        && (m_bStore == other.m_bStore) && (m_fBoost == other.m_fBoost)
        && (m_bSortAndFilterable == other.m_bSortAndFilterable)
        && (m_bHasLengthNorm == other.m_bHasLengthNorm);
}

FieldType::ValueType FieldSchema::getValueType() const
{
    return (m_pFieldType ? m_pFieldType->getValueType() : FieldType::TEXT);
}

void FieldSchema::configure(Configurator& conf)
{
    conf.configure(_T("name"), m_sName, _T(""));

    if (conf.getMode() == Configurator::TO_CONF)
    {   
        if (m_pFieldType)
        {
            tstring str = m_pFieldType->getName();
            conf.configure(_T("type"), str, _T(""));
        }
    
        if (m_bStore)
        {
            conf.configure(_T("store"), m_bStore, false);
        }

        if (m_bSortAndFilterable)
        {
            conf.configure(_T("sortable"), m_bSortAndFilterable, false);
        }
        if (m_bHasLengthNorm)
        {
            conf.configure(_T("length_norm"), m_bHasLengthNorm, false);
        }

        if (m_fBoost != 1.0)
        {
            Number n(m_fBoost);
            tstring str = n.toString();
            conf.configure(_T("boost"), str, _T(""));
        }
    }
    else 
    {
        conf.configure(_T("type"), m_sType, _T(""));

        tstring str;
        conf.configure(_T("store"), m_bStore, false);

        conf.configure(_T("sortable"), m_bSortAndFilterable, false);
        conf.configure(_T("length_norm"), m_bHasLengthNorm, false);
            
        conf.configure(_T("boost"), str, _T(""));
        if (!str.empty())
        {
            m_fBoost = (float)NumberParser::parseFloat(str);
        }
        else 
        {
            m_fBoost = 1.0;
        }
    }
}

FX_NS_END
