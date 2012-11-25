#include "firtex/extension/plugin/SharedLibrary.h"
#include "firtex/utility/Exception.h"


#if defined(FX_WINDOWS)
#include "SharedLibrary_WIN32.cpp"
#else
#include "SharedLibrary_POSIX.cpp"
#endif

FX_NS_USE(utility);

FX_NS_DEF(plugin);

SharedLibrary::SharedLibrary()
{
}

SharedLibrary::SharedLibrary(const tstring& path)
{
	loadImpl(path);
}

SharedLibrary::~SharedLibrary()
{
}

void SharedLibrary::load(const tstring& path)
{
	loadImpl(path);
}

void SharedLibrary::unload()
{
	unloadImpl();
}

bool SharedLibrary::isLoaded() const
{
	return isLoadedImpl();
}

bool SharedLibrary::hasSymbol(const tstring& name)
{
	return findSymbolImpl(name) != 0;
}

void* SharedLibrary::getSymbol(const tstring& name)
{
	void* result = findSymbolImpl(name);
	if (result)
    {
		return result;
    }
	else
    {
        throw NotFoundException(name);
    }
}

const tstring& SharedLibrary::getPath() const
{
	return getPathImpl();
}


tstring SharedLibrary::suffix()
{
	return suffixImpl();
}

FX_NS_END
