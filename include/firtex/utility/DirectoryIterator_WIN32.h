#ifndef __DIRECTORYITERATOR_WIN32_H
#define __DIRECTORYITERATOR_WIN32_H

#include "firtex/common/StdHeader.h"
#include <windows.h>

FX_NS_DEF(utility);

class Path;
class DirectoryIteratorImpl
{
public:
     DirectoryIteratorImpl(const Path& path);
     DirectoryIteratorImpl(const tstring& sPath);

     ~DirectoryIteratorImpl();
public:
     const tstring& doNext();
     const tstring& getCurrent() const;
private:
     void findFirstFile(const tstring& sFindPath);
private:
     tstring m_sCurrent;
     HANDLE m_fileHandle;
     WIN32_FIND_DATA m_findData;     
};

////////////////////////////////////////////////////////////////////////////////
//inline

inline const tstring& DirectoryIteratorImpl::getCurrent() const
{
     return m_sCurrent;
}

FX_NS_END

#endif
