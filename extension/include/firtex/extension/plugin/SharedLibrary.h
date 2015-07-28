#ifndef __SHAREDLIBRARY_H
#define __SHAREDLIBRARY_H


#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"

#if defined(FX_WINDOWS)
#include "firtex/extension/plugin/SharedLibrary_WIN32.h"
#else
#include "firtex/extension/plugin/SharedLibrary_POSIX.h"
#endif

FX_NS_DEF(plugin);

/**
 * @class SharedLibrary
 * 	The SharedLibrary class dynamically 
 *  loads shared libraries at run-time.
 */
class SharedLibrary : private SharedLibraryImpl
{
public:
	SharedLibrary();
	SharedLibrary(const tstring& path);
	virtual ~SharedLibrary();

public:
    /**
     * Load a shared library from the given path.
     * @throw throw LibraryAlreadyLoadedException if a library 
     *  has already been loaded. Throw a LibraryLoadException if the library
     * cannot be loaded.
     */
	void load(const tstring& path);

	/**
     * Unloads a shared library.
     */
	void unload();

    /** 
     * Return true iff a library has been loaded.
     */
	bool isLoaded() const;

    /**
     * Return true iff the loaded library contains
     * a symbol with the given name.
     */
	bool hasSymbol(const tstring& name);


    /**
     * Return the address of the symbol with the given name. 
     * @throw throw a NotFoundException if the symbol does not exist.
     */
	void* getSymbol(const tstring& name);

    /**
     * Return the path of the library, as specified in a call to load() or the 
     * constructor.		
     */
	const tstring& getPath() const;

public:
    /**
     * Return the platform-specific filename suffix
     * for shared libraries.
     */
	static tstring suffix();
		
private:
	SharedLibrary(const SharedLibrary&);
	SharedLibrary& operator = (const SharedLibrary&);
};

DEFINE_TYPED_PTR(SharedLibrary);

FX_NS_END

#endif 
