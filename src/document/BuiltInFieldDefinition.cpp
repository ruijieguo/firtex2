#include "firtex/document/BuiltInFieldDefinition.h"

FX_NS_DEF(document);

BuiltInFieldDefinition::BuiltInFieldDefinition()
{
    init();
}

BuiltInFieldDefinition::~BuiltInFieldDefinition()
{
}

void BuiltInFieldDefinition::init()
{
    // gbk text field
    {
        FieldType textField("TEXT", FieldType::TEXT);
        FieldType::IndexType& indexType = textField.getIndexType();
        indexType.identifier = "indexer.text";
        indexType.analyzer.identifier = "analyzer.standard";
        //Using gbk dictionary, max forward segment algorithm
        indexType.analyzer.parameter = "encode=gbk;algorithm=max_forward";
        addFieldType(textField);
    }

    // utf-8 text field
    {
        FieldType utf8TextField("UTF8_TEXT", FieldType::TEXT);
        FieldType::IndexType& indexType2 = utf8TextField.getIndexType();
        indexType2.identifier = "indexer.text";
        indexType2.analyzer.identifier = "analyzer.standard";
        //Using utf-8 dictionary, max forward segment algorithm
        indexType2.analyzer.parameter = "encode=utf-8;algorithm=max_forward";
        addFieldType(utf8TextField);
    }

    // Define GBK Chinese single word text
    {
        FieldType swTextField("SW_TEXT", FieldType::TEXT);
        FieldType::IndexType& indexType3 = swTextField.getIndexType();
        indexType3.identifier = "indexer.text";
        indexType3.analyzer.identifier = "analyzer.single";
        //Using gbk dictionary
        indexType3.analyzer.parameter = "encode=gbk";
        addFieldType(swTextField);
    }

    // Define UTF-8 Chinese single word text
    {
        FieldType utf8SWTextField("UTF8_SW_TEXT", FieldType::TEXT);
        FieldType::IndexType& indexType4 = utf8SWTextField.getIndexType();
        indexType4.identifier = "indexer.text";
        indexType4.analyzer.identifier = "analyzer.single";
        //Using gbk dictionary
        indexType4.analyzer.parameter = "encode=utf-8";
        addFieldType(utf8SWTextField);
    }

    // int8 field
    {
        FieldType sortInt8Field("INT8", FieldType::INT8);
        sortInt8Field.getForwardIndexType().identifier = "fdindex.int8";
        addFieldType(sortInt8Field);
    }

    // uint8 field
    {
        FieldType sortUint8Field("UINT8", FieldType::UINT8);
        sortUint8Field.getForwardIndexType().identifier = "fdindex.uint8";
        addFieldType(sortUint8Field);
    }

    // int16 field
    {
        FieldType sortInt16Field("INT16", FieldType::INT16);
        sortInt16Field.getForwardIndexType().identifier = "fdindex.int16";
        addFieldType(sortInt16Field);
    }

    // uint16 field
    {
        FieldType sortUint16Field("UINT16", FieldType::UINT16);
        sortUint16Field.getForwardIndexType().identifier = "fdindex.uint16";
        addFieldType(sortUint16Field);
    }

    // int32 field
    {
        FieldType sortInt32Field("INT32", FieldType::INT32);
        sortInt32Field.getForwardIndexType().identifier = "fdindex.int32";
        addFieldType(sortInt32Field);
    }

    // uint32 field
    {
        FieldType sortUint32Field("UINT32", FieldType::UINT32);
        sortUint32Field.getForwardIndexType().identifier = "fdindex.uint32";
        addFieldType(sortUint32Field);
    }

    // int64 field
    {
        FieldType sortInt64Field("INT64", FieldType::INT64);
        sortInt64Field.getForwardIndexType().identifier = "fdindex.int64";
        addFieldType(sortInt64Field);
    }

    // uint64 field
    {
        FieldType sortUint64Field("UINT64", FieldType::UINT64);
        sortUint64Field.getForwardIndexType().identifier = "fdindex.uint64";
        addFieldType(sortUint64Field);
    }

    // float field
    {
        FieldType sortFloatField("FLOAT", FieldType::FLOAT);
        sortFloatField.getForwardIndexType().identifier = "fdindex.float";
        addFieldType(sortFloatField);
    }

    // double field
    {
        FieldType sortDoubleField("DOUBLE", FieldType::DOUBLE);
        sortDoubleField.getForwardIndexType().identifier = "fdindex.double";
        addFieldType(sortDoubleField);
    }

    // int8 field, indexed
    {
        FieldType int8Field("INT8_I", FieldType::INT8);
        FieldType::IndexType& int8IndexType = int8Field.getIndexType();
        int8IndexType.identifier = "indexer.int8";
        addFieldType(int8Field);
    }

    // uint8 field, indexed
    {
        FieldType uint8Field("UINT8_I", FieldType::UINT8);
        FieldType::IndexType& uint8IndexType = uint8Field.getIndexType();
        uint8IndexType.identifier = "indexer.uint8";
        addFieldType(uint8Field);
    }

    // int16 field, indexed
    {
        FieldType int16Field("INT16_I", FieldType::INT16);
        FieldType::IndexType& int16IndexType = int16Field.getIndexType();
        int16IndexType.identifier = "indexer.int16";
        addFieldType(int16Field);
    }

    // uint16 field, indexed
    {
        FieldType uint16Field("UINT16_I", FieldType::UINT16);
        FieldType::IndexType& uint16IndexType = uint16Field.getIndexType();
        uint16IndexType.identifier = "indexer.uint16";
        addFieldType(uint16Field);
    }

    // int32 field, indexed
    {
        FieldType int32Field("INT32_I", FieldType::INT32);
        FieldType::IndexType& int32IndexType = int32Field.getIndexType();
        int32IndexType.identifier = "indexer.int32";
        addFieldType(int32Field);
    }

    // uint32 field, indexed
    {
        FieldType uint32Field("UINT32_I", FieldType::UINT32);
        FieldType::IndexType& uint32IndexType = uint32Field.getIndexType();
        uint32IndexType.identifier = "indexer.uint32";
        addFieldType(uint32Field);
    }

    // int64 field, indexed
    {
        FieldType int64Field("INT64_I", FieldType::INT64);
        FieldType::IndexType& int64IndexType = int64Field.getIndexType();
        int64IndexType.identifier = "indexer.int64";
        addFieldType(int64Field);
    }

    // uint64 field, indexed
    {
        FieldType uint64Field("UINT64_I", FieldType::UINT64);
        FieldType::IndexType& uint64IndexType = uint64Field.getIndexType();
        uint64IndexType.identifier = "indexer.uint64";
        addFieldType(uint64Field);
    }

    // int8 field, indexed, forward index
    {
        FieldType int8Field("INT8_IF", FieldType::INT8);
        FieldType::IndexType& int8IndexType = int8Field.getIndexType();
        int8IndexType.identifier = "indexer.int8";
        int8Field.getForwardIndexType().identifier = "fdindex.int8";
        addFieldType(int8Field);
    }

    // uint8 field, indexed
    {
        FieldType uint8Field("UINT8_IF", FieldType::UINT8);
        FieldType::IndexType& uint8IndexType = uint8Field.getIndexType();
        uint8IndexType.identifier = "indexer.uint8";
        uint8Field.getForwardIndexType().identifier = "fdindex.uint8";
        uint8IndexType.identifier = "indexer.uint8";
        addFieldType(uint8Field);
    }

    // int16 field, indexed, forward index
    {
        FieldType int16Field("INT16_IF", FieldType::INT16);
        FieldType::IndexType& int16IndexType = int16Field.getIndexType();
        int16IndexType.identifier = "indexer.int16";
        int16Field.getForwardIndexType().identifier = "fdindex.int16";
        addFieldType(int16Field);
    }

    // uint16 field, indexed
    {
        FieldType uint16Field("UINT16_IF", FieldType::UINT16);
        FieldType::IndexType& uint16IndexType = uint16Field.getIndexType();
        uint16IndexType.identifier = "indexer.uint16";
        uint16Field.getForwardIndexType().identifier = "fdindex.uint16";
        uint16IndexType.identifier = "indexer.uint16";
        addFieldType(uint16Field);
    }

    // int32 field, indexed, forward index
    {
        FieldType int32Field("INT32_IF", FieldType::INT32);
        FieldType::IndexType& int32IndexType = int32Field.getIndexType();
        int32IndexType.identifier = "indexer.int32";
        int32Field.getForwardIndexType().identifier = "fdindex.int32";
        addFieldType(int32Field);
    }

    // uint32 field, indexed
    {
        FieldType uint32Field("UINT32_IF", FieldType::UINT32);
        FieldType::IndexType& uint32IndexType = uint32Field.getIndexType();
        uint32IndexType.identifier = "indexer.uint32";
        uint32Field.getForwardIndexType().identifier = "fdindex.uint32";
        uint32IndexType.identifier = "indexer.uint32";
        addFieldType(uint32Field);
    }

    // int64 field, indexed, forward index
    {
        FieldType int64Field("INT64_IF", FieldType::INT64);
        FieldType::IndexType& int64IndexType = int64Field.getIndexType();
        int64IndexType.identifier = "indexer.int64";
        int64Field.getForwardIndexType().identifier = "fdindex.int64";
        addFieldType(int64Field);
    }

    // uint64 field, indexed
    {
        FieldType uint64Field("UINT64_IF", FieldType::UINT64);
        FieldType::IndexType& uint64IndexType = uint64Field.getIndexType();
        uint64IndexType.identifier = "indexer.uint64";
        uint64Field.getForwardIndexType().identifier = "fdindex.uint64";
        uint64IndexType.identifier = "indexer.uint64";
        addFieldType(uint64Field);
    }

    //datetime (32bit) field, forward index 
    {
        FieldType dateTimeField("DATETIME", FieldType::INT32);

        FieldType::ForwardIndexType& dateTimeFDType =
            dateTimeField.getForwardIndexType();
        dateTimeFDType.identifier = "fdindex.int32";
        dateTimeFDType.analyzer.identifier = "analyzer.datetime";
        dateTimeField.getSearchType().analyzer.identifier = "analyzer.datetime";

        addFieldType(dateTimeField);
    }

    // datetime (64bit) field, forward indexed
    {
        FieldType dateTimeField("DATETIME64", FieldType::INT64);

        FieldType::ForwardIndexType& dateTimeFDType =
            dateTimeField.getForwardIndexType();
        dateTimeFDType.identifier = "fdindex.int64";
        dateTimeFDType.analyzer.identifier = "analyzer.datetime";
        dateTimeField.getSearchType().analyzer.identifier = "analyzer.datetime";

        addFieldType(dateTimeField);
    }

    // datetime field, indexed
    {
        FieldType dateTimeField("DATETIME_I", FieldType::STRING);

        FieldType::IndexType& dateTimeIndexType = dateTimeField.getIndexType();
        dateTimeIndexType.identifier = "indexer.datetime";
        dateTimeIndexType.analyzer.identifier = "analyzer.datetime";

        dateTimeField.getSearchType().analyzer.identifier = "analyzer.datetime";

        addFieldType(dateTimeField);
    }

    // datetime field, indexed and forward indexed (64bit)
    {
        FieldType dateTimeField("DATETIME_IF", FieldType::INT64);

        FieldType::IndexType& dateTimeIndexType = dateTimeField.getIndexType();
        dateTimeIndexType.identifier = "indexer.datetime";
        dateTimeIndexType.analyzer.identifier = "analyzer.datetime";

        FieldType::ForwardIndexType& dateTimeFDType =
            dateTimeField.getForwardIndexType();
        dateTimeFDType.identifier = "fdindex.int64";
        dateTimeFDType.analyzer.identifier = "analyzer.datetime";

        dateTimeField.getSearchType().analyzer.identifier = "analyzer.datetime";
        addFieldType(dateTimeField);
    }

    // keyword field
    {
        FieldType keywordField("KEYWORD", FieldType::STRING);
        FieldType::IndexType& keywordIndexType = keywordField.getIndexType();
        keywordIndexType.identifier = "indexer.keyword";
        keywordIndexType.analyzer.identifier = "analyzer.keyword";
        addFieldType(keywordField);
    }

    // primkey field
    {
        FieldType primkeyField("PRIMARY_KEY", FieldType::STRING);
        primkeyField.getIndexType().identifier = "indexer.primarykey";
        addFieldType(primkeyField);
    }
}

FX_NS_END
