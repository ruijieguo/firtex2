#ifndef __DIRECTORYITERATOR_POSIX_H
#define __DIRECTORYITERATOR_POSIX_H

#include "firtex/common/StdHeader.h"
#include <dirent.h>

FX_NS_DEF(utility);

class DirectoryIteratorImpl
{
public:
	DirectoryIteratorImpl(const tstring& path);
	~DirectoryIteratorImpl();
public:	
	const tstring& getCurrent() const;
    const tstring& doNext();
private:
	DIR*        m_pDir;
	tstring m_sCurrent;
};


///////////////////////////////////////////////////////////////
// inlines
//
inline const tstring& DirectoryIteratorImpl::getCurrent() const
{
	return m_sCurrent;
}

FX_NS_END

#endif 
