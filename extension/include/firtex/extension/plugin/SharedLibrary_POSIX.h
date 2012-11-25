#ifndef __SHAREDLIBRARY_POSIX_H
#define __SHAREDLIBRARY_POSIX_H

#include "firtex/common/StdHeader.h"
#include "firtex/thread/FastMutex.h"

FX_NS_DEF(plugin);

class SharedLibraryImpl
{
protected:
	SharedLibraryImpl();
	~SharedLibraryImpl();
	void loadImpl(const tstring& path);
	void unloadImpl();
	bool isLoadedImpl() const;
	void* findSymbolImpl(const tstring& name);
	const tstring& getPathImpl() const;

	static tstring suffixImpl();

private:
	tstring m_sPath;
	void* m_handle;

	static FX_NS(thread)::FastMutex ms_mutex;
};

FX_NS_END

#endif 
