#ifndef __FX_SNIPPETGENERATOR_H
#define __FX_SNIPPETGENERATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/index/FieldFilter.h"
#include "firtex/search/Query.h"
#include "firtex/analyzer/AnalyzerMapper.h"
#include <string>

FX_NS_DEF(app);

class SnippetGenerator : public FX_NS(index)::FieldFilter
{
public:
    SnippetGenerator()
        : m_pAnalyzer(NULL)
    {
    }

    bool init(const FX_NS(search)::QueryPtr& pQuery,
              const FX_NS(analyzer)::AnalyzerMapper* pAnalyzerMap,
              const std::string& sField,
              const std::string& sPreTag = "<B>",
              const std::string& sPostTag = "</B>",
              const std::string& sSeparator = "...")
    {
        m_pQuery = pQuery;
        m_pAnalyzer = pAnalyzerMap->getAnalyzer(sField);
        if (!m_pAnalyzer)
        {
            FX_LOG(WARN, "No analyzer for field: [%s]", sField.c_str());
            return false;
        }
        m_sField = sField;
        m_sPreTag = sPreTag.empty() ? "<B>" : sPreTag;
        m_sPostTag = sPostTag.empty() ? "</B>" : sPostTag;
        m_sSeparator = sSeparator.empty() ? "..." : sSeparator;
        return true;
    }

    /**
     * Make some modification on sOrgValue and output to sNewValue
     *
     * @param sOrgValue original value 
     * @param sNewValue the output value
     * 
     * @return true if any modification performed on sOrgValue
     */
    bool filter(const std::string& sOrgValue, 
                std::string& sNewValue) const;

private:
    FX_NS(search)::QueryPtr m_pQuery;
    const FX_NS(analyzer)::Analyzer* m_pAnalyzer;
    std::string m_sField;
    std::string m_sPreTag;
    std::string m_sPostTag;
    std::string m_sSeparator;
    
private:
    DECLARE_LOGGER();
};

DEFINE_TYPED_PTR(SnippetGenerator);

FX_NS_END

#endif //__FX_SNIPPETGENERATOR_H
