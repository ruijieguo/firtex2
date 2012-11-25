#include "firtex/extension/com/Com.h"
#include "firtex/extension/com/ComPtr.h"
#include "firtex/extension/com/DllServer.h"
#include "PDFParserPlugin.h"

FX_NS_USE(com)
FX_NS_DEF(plugin)

FX_STDMETHODIMP_(uint32_t) PDFParserFactory::AddRef()
{ 
	if ( !m_count ) 
		DllServer::DllLockServerImpl(true); 
	return ++m_count; 
};

FX_STDMETHODIMP_(uint32_t) PDFParserFactory::Release()
{ 
	unsigned long c = --m_count; 
	if ( !c ) 
	{
		DllServer::DllLockServerImpl(false); 
		// only used as a static object
		// don't delete the object
		// not on heap

		/* delete this; */
	}
	return c; 
};

FX_STDMETHODIMP PDFParserFactory::QueryInterface( FX_NS(com)::FX_IID const& riid, void** ptr )
{
	if ( FX_NS(com)::FX_IsEqualIID( riid, FX_NS(com)::IUnknown::iid ) )
	{
		*ptr = static_cast<FX_NS(com)::IUnknown*>( this );
		AddRef();
		return FX_S_OK;
	}
	if ( FX_NS(com)::FX_IsEqualIID( riid, FX_NS(com)::IClassFactory::iid ) )
	{
		*ptr = static_cast<FX_NS(com)::IClassFactory*>( this );
		AddRef();
		return FX_S_OK;
	}

	return FX_E_NOINTERFACE;
}

FX_STDMETHODIMP PDFParserFactory::LockServer(int32_t fLock)
{
	return DllServer::DllLockServerImpl( fLock );
}

FX_STDMETHODIMP PDFParserFactory::CreateInstance( FX_NS(com)::IUnknown* pUnkOuter, FX_NS(com)::FX_REFIID riid, void** ppvObject )
{
	if ( pUnkOuter ) 
		return FX_CLASS_E_NOAGGREGATION;

	PDFParserPlugin* ptr = 0;
	ptr = new PDFParserPlugin();
	if ( !ptr ) 
	{
		return FX_E_OUTOFMEMORY;
	}

	FX_NS(com)::FX_HRESULT hr = ptr->QueryInterface( riid, ppvObject );
	if ( FX_FAILED(hr) ) 
	{
		delete ptr;
	}
	return hr;
}

FX_NS_END
