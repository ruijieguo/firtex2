#ifndef _PDFPARSERPLUGIN_H
#define _PDFPARSERPLUGIN_H

#include "firtex/extension/com/Com.h"
#include "firtex/extension/plugin/FirteXCOM.h"
#include "firtex/extension/com/ComPtr.h"
#include <map>

FX_NS_DEF(plugin)

class PDFDocReader;
class PDFParserPlugin : public IDocumentProcessorPlugin
{
public:
    const static int TITLE            = 0;
    const static int SUBJECT          = 1;
    const static int KEYWORDS         = 2;
    const static int AUTHOR           = 3;
    const static int CREATOR          = 4;
    const static int PRODUCER         = 5;
    const static int CREATIONDATE     = 6;
    const static int LASTMODIFIEDDATE = 7;
    const static int CONTENT          = 8;

public:
    PDFParserPlugin();
    PDFParserPlugin(const PDFParserPlugin& src);
    ~PDFParserPlugin();

public:
    FX_STDMETHOD_(uint32_t,AddRef)();
    FX_STDMETHOD_(uint32_t,Release)();
    FX_STDMETHOD(QueryInterface)( FX_NS(com)::FX_IID const& riid, void** ptr );

    FX_STDMETHOD(init)(/* [in] */ IDocumentSchema* pDocSchema,
                       /* [in] */ IDocumentTemplate* pDocTemp);
    FX_STDMETHOD(getIdentifier)(/* [out] */ FX_NS(com)::FX_BSTR* pIdent);
    FX_STDMETHOD(getDocType)(/* [out] */ FX_NS(com)::FX_BSTR* pDocType);
    FX_STDMETHOD(process)(/* [in] */ IDocumentSource* pDocSource);
    FX_STDMETHOD(clone)(/* [out] */IDocumentProcessorPlugin** ppPlugin);

protected:
    /**
     * get field id
     * @param pDocument document interface
     */
    void getFieldIds(IDocument* pDocument);

private:
    typedef std::map<FX_NS(com)::ComStr, int> NameMap;

    unsigned long m_count;
    int32_t m_schemaIds[CONTENT + 1];
    NameMap m_fieldName2Id;

    PDFDocReader* m_pReader;
    bool m_bNeedMeta;
};

/* uuid:  {DC8E0FBE-9DFF-4f99-8F2D-525EDE968392} */
/* VersionIndependentProgId: FirteX.PDFParserPlugin */
/* ProgId: FirteX.PDFParserPlugin.1 */
namespace pdfparserplugin 
{
extern const FX_NS(com)::FX_CLSID clsid;
extern const wchar_t VersionIndependentProgId[];
extern const wchar_t ProgId[];
};


class PDFParserFactory: public FX_NS(com)::IClassFactory
{
public:
    PDFParserFactory() { m_count = 0; };

    FX_STDMETHOD_(uint32_t,AddRef)();
    FX_STDMETHOD_(uint32_t,Release)();
    FX_STDMETHOD(QueryInterface)( FX_NS(com)::FX_IID const& riid, void** ptr );
    FX_STDMETHOD(LockServer)(int32_t fLock);
    FX_STDMETHOD(CreateInstance)( FX_NS(com)::IUnknown* pUnkOuter, FX_NS(com)::FX_REFIID riid, void** ppvObject );

private:
    unsigned long m_count;
};

FX_NS_END

#endif
