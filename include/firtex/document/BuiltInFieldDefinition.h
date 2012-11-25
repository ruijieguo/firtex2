#ifndef __FX_BUILTIN_FIELD_DEFINITION_H
#define __FX_BUILTIN_FIELD_DEFINITION_H

#include "firtex/document/FieldDefinition.h"
#include "firtex/utility/Singleton.h"

FX_NS_DEF(document);

class BuiltInFieldDefinition : public FieldDefinition,
    public FX_NS(utility)::Singleton<BuiltInFieldDefinition>
{
protected:
    BuiltInFieldDefinition();
    virtual ~BuiltInFieldDefinition();

    DECLARE_LAZY_SINGLETON(BuiltInFieldDefinition)

public:
    void init();
};

FX_NS_END

#endif //__FX_BUILTIN_FIELD_DEFINITION_H
