#ifndef __FX_FIELDSCHEMA_H
#define __FX_FIELDSCHEMA_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/common/StdHeader.h"
#include "firtex/common/Logger.h"
#include "firtex/config/Configurable.h"
#include "firtex/document/FieldType.h"
#include <vector>
#include <map>

FX_NS_DEF(document);

class FieldSchema : public FX_NS(config)::Configurable
{
public:
    FieldSchema();
    FieldSchema(const std::string& sName, const FieldType* pFieldType,
                bool bStore, float fBoost = 1.0);
    FieldSchema(const FieldSchema& src);
    ~FieldSchema();

public:
    fieldid_t getId() const {return m_id;}
    void setId(fieldid_t fid) {m_id = fid;}

    const std::string& getName() const {return m_sName;}
    void setName(const std::string& sName){m_sName = sName;}

    const std::string& getTypeName() const {return m_sType;}

    const FieldType* getFieldType() const {return m_pFieldType;}
    void setFieldType(const FieldType* pFieldType);

    FieldType::ValueType getValueType() const;

    void setStore(bool bStore) { m_bStore = bStore;}

    bool isIndexed() const;
    bool isAnalyzed() const;
    bool isStored() const {return m_bStore;}
    bool isFilterable() const {return m_bSortAndFilterable;}
    bool hasLengthNorm() const {return m_bHasLengthNorm;}

    bool hasForwardIndex() const;

    float getBoost() const {return m_fBoost;}
    void setBoost(float fBoost){m_fBoost = fBoost;}

public:
    void configure(FX_NS(config)::Configurator& conf);

public:
    /** 
     * compare two schema
     * @return true if equal otherwise false
     */
    bool isEqual(const FieldSchema& item) const;

public:
    /** 
     * assign operator
     */
    FieldSchema& operator = (const FieldSchema& right);	

protected:
    fieldid_t m_id;
    std::string m_sName; /// field name
    std::string m_sType; /// field type name
    bool m_bStore; /// store strategy
    bool m_bSortAndFilterable;
    bool m_bHasLengthNorm;
    float m_fBoost;	 ///boost value

    const FieldType* m_pFieldType;

    friend class DocumentSchema;

private:
    DECLARE_STREAM_LOGGER();
};

DEFINE_TYPED_PTR(FieldSchema);

///////////////////////////////////////////////////////////
//
inline bool FieldSchema::isAnalyzed() const 
{
    return (m_pFieldType ? m_pFieldType->isAnalyzed() : false);
} 

inline bool FieldSchema::isIndexed() const 
{
    return (m_pFieldType ? m_pFieldType->isIndexed() : false);
}

inline bool FieldSchema::hasForwardIndex() const
{
    return (m_pFieldType ? (m_pFieldType->hasForwardIndex()) : false);
}



FX_NS_END

#endif
