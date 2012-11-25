#include "firtex/extension/collection/DefaultFileFilter.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/KeyValueParser.h"
#include "firtex/utility/StringTokenizer.h"

FX_NS_USE(utility);

FX_NS_DEF(collection);

SETUP_STREAM_LOGGER(collection, DefaultFileFilter);

DefaultFileFilter::DefaultFileFilter(const std::string& sBasePath) 
{
    Path path(sBasePath);
    m_nBasePathDepth = path.depth();
}

DefaultFileFilter::~DefaultFileFilter() 
{
}

bool DefaultFileFilter::init(const std::string& sFilter)
{
    bool bSucc = false;
    KeyValueParser pp;
    if (pp.parse(sFilter.c_str()))
    {
        std::string sValue;
        // format: filter=filter1,filter2...
        if (pp.getValue("filter", sValue))
        {
            m_filters.clear();

            StringTokenizer st(sValue, ",", StringTokenizer::TOKEN_TRIM
                    | StringTokenizer::TOKEN_IGNORE_EMPTY);
            for (size_t i = 0; i < st.getNumTokens(); ++i)
            {
                m_filters.push_back(st[i]);
            }
            bSucc = true;
        }

        // format: file_extension=ext1,ext2...
        if (pp.getValue("file_extension", sValue))
        {
            m_fileTypes.clear();

            StringTokenizer st(sValue, ",", StringTokenizer::TOKEN_TRIM
                    | StringTokenizer::TOKEN_IGNORE_EMPTY);
            for (size_t i = 0; i < st.getNumTokens(); ++i)
            {
                m_fileTypes.push_back(st[i]);
            }
            bSucc = true;
        }
    }
    return bSucc;
}        

bool DefaultFileFilter::isFiltered(const FX_NS(utility)::Path& filePath)
{
    if (m_filters.size() > 0)
    {
        for (StringVector::const_iterator it = m_filters.begin();
             it != m_filters.end(); ++it)
        {
            size_t nCurDepth = filePath.depth();
            for (size_t i = m_nBasePathDepth; i < nCurDepth; ++i)
            {
                if (filePath.directory(i) == *it)
                {
                    return true;
                }
            }
        }
    }

    if (m_fileTypes.size() > 0)
    {
        for (StringVector::const_iterator it = m_fileTypes.begin();
             it != m_fileTypes.end(); ++it)
        {
            if (filePath.getExtension() == *it)
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }
    return true;
}

FX_NS_END

