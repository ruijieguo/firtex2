#ifndef __FX_FORWARDINDEXWRITER_H
#define __FX_FORWARDINDEXWRITER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/document/AnalyzedField.h"
#include "firtex/store/FileSystem.h"
#include "firtex/index/ForwardIndexReader.h"
#include "firtex/index/ForwardIndexCreator.h"
#include "firtex/document/FieldSchema.h"

FX_NS_DEF(index);

class ForwardIndexWriter : public ForwardIndexReader
{
public:
    ForwardIndexWriter() 
        : m_pFieldSchema(NULL)
    {
    }
    virtual ~ForwardIndexWriter() {}

public:
    /**
     * Initialize 
     */
    virtual void init(const FX_NS(document)::FieldSchema* pFieldSchema)
    {
        m_pFieldSchema = pFieldSchema;
    }
    
    /**
     * Add field to build forward index
     */
    virtual void addField(docid_t docId,
                          const FX_NS(document)::AnalyzedField* pField) = 0;

    /**
     * Seal the forward index . After sealing, modification is forbidden
     * 
     * @param lastDocId the last docid in the barrel
     */
    virtual void seal(docid_t lastDocId) = 0;

    /**
     * Finalize forward index and commit data to file system
     *
     * @param pFileSys file system instance
     * @param sFileType the file type (norm or forward index)
     * @param sSuffix suffix of index barrel
     * @return length of forward index file
     */
    virtual int64_t commit(FX_NS(store)::FileSystemPtr& pFileSys,
                           const std::string& sFileType,
                           const std::string& sSuffix) = 0;

public:
    /**
     * Return field info of this indexer
     */
    const FX_NS(document)::FieldSchema* getFieldSchema() const 
    {
        return m_pFieldSchema;
    }

private:
    const FX_NS(document)::FieldSchema* m_pFieldSchema;
};

DEFINE_TYPED_PTR(ForwardIndexWriter);

FX_NS_END

#endif
