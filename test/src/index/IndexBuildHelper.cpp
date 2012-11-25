#include "IndexBuildHelper.h"
#include "firtex/index/Index.h"
#include "firtex/utility/StringTokenizer.h"

using namespace std;
FX_NS_USE(document);
FX_NS_USE(utility);

FX_NS_DEF(index);

SETUP_STREAM_LOGGER(index, IndexBuildHelper);

void IndexBuildHelper::buildIndex(const string& sRootPath,
                                  const DocumentSchema& schema,
                                  const string& fieldsStr, bool bInc)
{
    try
    {
        Index::AccessMode am = (bInc ? Index::APPEND : Index::WRITE);
        Index index;
        index.open(sRootPath, am, &schema);

        IndexWriterPtr pIndexWriter = index.acquireWriter();

        try
        {
            buildIndex(pIndexWriter, fieldsStr);
        }
        catch(std::exception&e )
        {
            cout << "====exception: " << e.what() << endl;
        }

        try
        {
            pIndexWriter->close();
        }
        catch(std::exception&e )
        {
            cout << "====exception: " << e.what() << endl;
        }
    }
    catch (const FirteXException& e)
    {
        FX_STREAM_LOG(ERROR) << "Build index FAILED:" << e.what() << FIRTEX_ENDL;
        throw ;
    }
    catch(...)
    {
        cout << "====exception: " << endl;        
    }
}

void IndexBuildHelper::buildIndex(IndexWriterPtr& pIndexWriter,
                                  const std::string& fieldsStr)
{
    StringTokenizer st(fieldsStr, ";", StringTokenizer::TOKEN_TRIM | 
                       StringTokenizer::TOKEN_IGNORE_EMPTY);

    for (StringTokenizer::Iterator it = st.begin(); it != st.end(); ++it)
    {
        DocumentPtr pDoc = new Document(pIndexWriter->getDocSchema());

        StringTokenizer st2(*it, ",", StringTokenizer::TOKEN_TRIM | 
                            StringTokenizer::TOKEN_IGNORE_EMPTY);
            
        fieldid_t fieldId = 0;
        for (StringTokenizer::Iterator it2 = st2.begin(); it2 != st2.end(); ++it2)
        {
            pDoc->addField(fieldId++, *it2);
        }
        pIndexWriter->addDocument(pDoc);
    }
}

FX_NS_END
