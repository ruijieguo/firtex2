#include "firtex/extension/plugin/SharedLibrary_POSIX.h"
#include "firtex/extension/util/LibraryException.h"
#include <dlfcn.h>


// Note: cygwin is missing RTLD_LOCAL, set it to 0
#if defined(__CYGWIN__) && !defined(RTLD_LOCAL)
#define RTLD_LOCAL 0
#endif

FX_NS_USE(thread);
FX_NS_USE(util);
FX_NS_USE(utility);

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
        throw LibraryAlreadyLoadedException(path);
    }
	m_handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
	if (!m_handle)
	{
		const char* err = dlerror();
		throw LibraryLoadException(err ? tstring(err) : path);
	}
	m_sPath = path;
}


void SharedLibraryImpl::unloadImpl()
{
	FastMutex::Guard lock(ms_mutex);

	if (m_handle)
	{
		dlclose(m_handle);
		m_handle = 0;
	}
}


bool SharedLibraryImpl::isLoadedImpl() const
{
	return m_handle != 0; 
}

void* SharedLibraryImpl::findSymbolImpl(const tstring& name)
{
	FastMutex::Guard lock(ms_mutex);

	void* result = 0;
	if (m_handle)
	{
		result = dlsym(m_handle, name.c_str());
	}
	return result;
}

const tstring& SharedLibraryImpl::getPathImpl() const
{
	return m_sPath;
}

tstring SharedLibraryImpl::suffixImpl()
{
#if defined(__APPLE__)
    return "dylib";
#elif defined(__CYGWIN__)
    return "dll";
#else
    return "so";
#endif
}

FX_NS_END
