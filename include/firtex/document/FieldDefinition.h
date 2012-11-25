#ifndef __FIELDDEFINITION_H
#define __FIELDDEFINITION_H

#include "firtex/common/StdHeader.h"
#include "firtex/config/Configurable.h"
#include "firtex/document/FieldType.h"
#include <vector>
#include <map>
 
FX_NS_DEF(document);

class FieldDefinition : public FX_NS(config)::Configurable
{
public:
    typedef std::vector<FieldType*> TypeVector;
    typedef std::map<tstring, FieldType*> TypeMap;
public:
    FieldDefinition();
    FieldDefinition(const FieldDefinition& other);
    virtual ~FieldDefinition();
    
public:
    void configure(FX_NS(config)::Configurator& conf);

    /**
     * Add field type 
     * @param fieldType field type to add
     */
    void addFieldType(const FieldType& fieldType);

    /**
     * Get field type by type name
     * @param sTypeName field type name
     * @return reference to field type
     */
    const FieldType* getFieldType(const tstring& sTypeName) const;

    /**
     * Get field type by value type
     * @param vt value type
     * @return reference to field type
     */
    const FieldType* getForwardIndexFieldType(FieldType::ValueType vt) const;

private:
    TypeVector m_types;
    TypeMap m_name2TypeMap;
};

FX_NS_END

#endif
