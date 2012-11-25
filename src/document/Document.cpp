#include "firtex/document/Document.h"

FX_NS_DEF(document);

SETUP_STREAM_LOGGER(document, Document);

void Document::setBoost(float fBoost)
{
    m_fBoost = fBoost;
    if (fBoost != 1.0f)
    {
        setBoost(TypedDocument<Field>::getFields(), fBoost);
        setBoost(TypedDocument<Field>::getFieldPool(), fBoost);
    }
}

void Document::setBoost(FieldVector& fields, float fBoost)
{
    for (size_t i = 0; i < fields.size(); i++)
    {
        FieldType* pField = fields[i];
        if (pField)
        {
            pField->setBoost(fBoost);
        }
    }
}


FX_NS_END

