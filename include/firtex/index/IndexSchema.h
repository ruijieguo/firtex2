#ifndef __INDEXSCHEMA_H
#define __INDEXSCHEMA_H

#include "firtex/common/StdHeader.h"
#include "firtex/document/DocumentSchema.h"
#include "firtex/document/FieldDefinition.h"

FX_NS_DEF(index);

class IndexSchema
{
public:
    IndexSchema();
    ~IndexSchema();

public:
    
private:
    FX_NS(document)::FieldDefinition m_fieldDefinition;
    FX_NS(document)::DocumentSchema m_docSchema;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif //__INDEXSCHEMA_H
