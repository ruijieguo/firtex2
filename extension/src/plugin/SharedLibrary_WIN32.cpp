#include "firtex/extension/plugin/SharedLibrary_WIN32.h"
#include "firtex/extension/util/LibraryException.h"
#include "firtex/utility/Path.h"
#include <windows.h>

FX_NS_USE(utility);
FX_NS_USE(util);
FX_NS_USE(thread);

FX_NS_DEF(plugin);

FastMutex SharedLibraryImpl::ms_mutex;


SharedLibraryImpl::SharedLibraryImpl()
{
	m_handle = 0;
}

SharedLibraryImpl::~SharedLibraryImpl()
{
}

void SharedLibraryImpl::loadImpl(const tstring& path)
{
	FastMutex::Guard lock(ms_mutex);

	if (m_handle)
    {
        throw LibraryAlreadyLoadedException(m_sPath);
    }
	DWORD flags(0);
	Path p(path);
	if (p.isAbsolute()) 
    {
        flags |= LOAD_WITH_ALTERED_SEARCH_PATH;
    }
	m_handle = LoadLibraryExA(path.c_str(), 0, flags);
	if (!m_handle)
    {
        throw LibraryLoadException(path);
    }
	m_sPath = path;
}

void SharedLibraryImpl::unloadImpl()
{
	FastMutex::Guard lock(ms_mutex);

	if (m_handle)
	{
		FreeLibrary((HMODULE) m_handle);
		m_handle = 0;
	}
	m_sPath.clear();
}

bool SharedLibraryImpl::isLoadedImpl() const
{
	return (m_handle != NULL); 
}

void* SharedLibraryImpl::findSymbolImpl(const tstring& name)
{
	FastMutex::Guard lock(ms_mutex);

	if (m_handle)
	{
		return (void*) GetProcAddress((HMODULE) m_handle, name.c_str());
	}
	else return NULL;
}

const tstring& SharedLibraryImpl::getPathImpl() const
{
	return m_sPath;
}

tstring SharedLibraryImpl::suffixImpl()
{
	return _T("dll");
}

FX_NS_END
