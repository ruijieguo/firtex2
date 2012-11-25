#ifndef __FX_APPENDRESULTCOMBINER_H
#define __FX_APPENDRESULTCOMBINER_H

#include "ResultCombiner.h"
#include "firtex/common/Logger.h"
#include "firtex/utility/XMLDocumentWrapper.h"

FX_NS_DEF(app);

class AppendResultCombiner : public ResultCombiner
{
public:
    AppendResultCombiner() {}
    virtual ~AppendResultCombiner() {}

public:
    /// Begin result combination
    virtual void beginCombine();

    /**
     * Combine one result
     *
     * @param sClusterId id of search cluster 
     * @param sResult result of the search cluster
     */
    virtual void combine(const std::string& sClusterId, 
                         const std::string& sResult);

    /// extract combined result
    virtual void endCombine(std::string& sResult);

private:
    void appendErrorResult(const std::string& sClusterId,
                           const std::string& sErrorMsg);

private:
    FX_NS(utility)::XMLDocumentWrapperPtr m_pXmlDoc;
    FX_NS(utility)::XMLNodeWrapperPtr m_pRootNode;

private:
    DECLARE_STREAM_LOGGER();
};

FX_NS_END

#endif
