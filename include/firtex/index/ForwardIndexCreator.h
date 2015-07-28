#ifndef __FX_FORWARDINDEXCREATOR_H
#define __FX_FORWARDINDEXCREATOR_H

#include "firtex/common/StdHeader.h"

FX_NS_DEF(index);

class ForwardIndexWriter;
class ForwardIndexReader;
class ForwardIndexMerger;

namespace FdIndexType
{
    const static fdindex_t UNKNOWN = 0;
    const static fdindex_t INT32 = 1;
    const static fdindex_t INT64 = 2;
    const static fdindex_t UINT32 = 3;
    const static fdindex_t UINT64 = 4;
    const static fdindex_t FLOAT = 5;
    const static fdindex_t DOUBLE = 6;
}

#define FORWARD_INDEX_DEFINE(type, identifier)  \
    m_identifier += "." identifier;              \
    m_fdIndexType = type


class ForwardIndexCreator
{
protected:
    ForwardIndexCreator() 
        : m_identifier(_T("fdindex"))
        , m_fdIndexType(FdIndexType::UNKNOWN)
	{}

    virtual ~ForwardIndexCreator(){}
    
public:
    virtual ForwardIndexWriter* createWriter() const = 0;
    virtual ForwardIndexReader* createReader() const = 0;
    virtual ForwardIndexMerger* createMerger() const = 0;
    virtual const tstring& getIdentifier() const {return m_identifier;}
    virtual fdindex_t getType() const {return m_fdIndexType;}
    
protected:
    tstring m_identifier;
    fdindex_t m_fdIndexType;
};

FX_NS_END

#endif
