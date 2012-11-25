#ifndef __FIELDTYPE_H
#define __FIELDTYPE_H

#include "firtex/common/StdHeader.h"
#include "firtex/config/Configurable.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/config/Configurator.h"
#include "firtex/utility/StringUtils.h"
 
FX_NS_DEF(document);

/////////////////////////////////////////////////////////
//
class FieldType : public FX_NS(config)::Configurable
{
public:
    enum ValueType
    {
        UNKNOWN = 0,  ///text
        TEXT,         ///text
        UNICODETEXT,  ///Unicode text
        STRING,       ///string
        BINARY,       ///binary 
        INT8,         ///number
        INT16,        ///number
        INT32,        ///number
        INT64,        ///number
        UINT8,        ///number
        UINT16,       ///number
        UINT32,       ///number
        UINT64,       ///number
        FLOAT,        ///float
        DOUBLE,       ///double
    };

public:
    struct Analyzer : public FX_NS(config)::Configurable
    {
    public:
        bool isEmpty() const
        {
            return (identifier.empty() && parameter.empty()
                    && anteFilter.empty() && postFilter.empty());
        }

    public:
        std::string identifier;  //identifier of analyzer
        std::string parameter;   //parameter of analyzer
        std::string anteFilter;  //ante filter for analyzer
        std::string postFilter;  //post filter for analyzer

        void configure(FX_NS(config)::Configurator& conf);
    };

    struct IndexType : public FX_NS(config)::Configurable
    {
        std::string identifier;  //identifier of indexer
        Analyzer analyzer;

        void configure(FX_NS(config)::Configurator& conf);
    };

    struct ForwardIndexType : public FX_NS(config)::Configurable
    {
        std::string identifier;  //identifier of forward index
        Analyzer analyzer;

        void configure(FX_NS(config)::Configurator& conf);
    };

    struct SearchType : public FX_NS(config)::Configurable
    {
        Analyzer analyzer;
        void configure(FX_NS(config)::Configurator& conf);
    };

public:
    FieldType();
    FieldType(const std::string& sName, ValueType eValueType);
    FieldType(const FieldType& other);
    ~FieldType();

public:
    const std::string& getName() const {return m_sName;}
    void setName(const std::string& sName) { m_sName = sName;}

    ValueType getValueType() const {return m_eValueType;}
    void setValueType(FieldType::ValueType eValueType) { m_eValueType = eValueType;}

    const IndexType& getIndexType() const {return m_indexType;}
    IndexType& getIndexType() {return m_indexType;}

    const ForwardIndexType& getForwardIndexType() const {return m_forwardIndexType;}
    ForwardIndexType& getForwardIndexType() {return m_forwardIndexType;}

    const SearchType& getSearchType() const {return m_searchType;}
    SearchType& getSearchType() {return m_searchType;}

    bool isIndexed() const;
    bool isAnalyzed() const;
    bool hasForwardIndex() const;

    void setForwardIndex(bool bNeedForwardIndex);

public:
    void configure(FX_NS(config)::Configurator& conf);    

public:
    static std::string valueTypeToString(FieldType::ValueType eValueType);
    static FieldType::ValueType stringToValueType(const std::string& sValueType);

public:
    std::string m_sName;
    ValueType m_eValueType;
    IndexType m_indexType;
    ForwardIndexType m_forwardIndexType;
    SearchType m_searchType;
};

typedef FX_NS(common)::SharedPtr<FieldType> FieldTypePtr;

//////////////////////////////////////////////////////
//
inline bool FieldType::isIndexed() const 
{
    return (!m_indexType.identifier.empty());
}

inline bool FieldType::hasForwardIndex() const 
{
    return (!m_forwardIndexType.identifier.empty());
} 

inline bool FieldType::isAnalyzed() const 
{
    return (!m_indexType.analyzer.identifier.empty()
            || !m_forwardIndexType.analyzer.identifier.empty());
}

FX_NS_END

#endif
