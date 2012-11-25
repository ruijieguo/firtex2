
#include "firtex/utility/DirectoryIterator_WIN32.h"
#include "firtex/utility/File_WIN32.h"
#include "firtex/utility/Path.h"

FX_NS_DEF(utility);


DirectoryIteratorImpl::DirectoryIteratorImpl(const Path& path)
    : m_fileHandle(INVALID_HANDLE_VALUE)
{
	tstring sFindPath = path.toString();
    sFindPath.push_back(Path::separator());
	sFindPath.append(_T("*"));

    findFirstFile(sFindPath);
}

DirectoryIteratorImpl::DirectoryIteratorImpl(const tstring& sPath)
    : m_fileHandle(INVALID_HANDLE_VALUE)
{
	tstring sFindPath = sPath;
    sFindPath.push_back(Path::separator());
    sFindPath.append(_T("*"));

    findFirstFile(sFindPath);
}

DirectoryIteratorImpl::~DirectoryIteratorImpl()
{
	if (m_fileHandle != INVALID_HANDLE_VALUE)
		FindClose(m_fileHandle);
}

const tstring& DirectoryIteratorImpl::doNext()
{
	do
	{
		if (FindNextFile(m_fileHandle, &m_findData) != 0)
        {
            m_sCurrent = m_findData.cFileName;
        }        
		else
        {
			m_sCurrent.clear();
        }        
	}while (m_sCurrent == _T(".") || m_sCurrent == _T(".."));
    return m_sCurrent;    
}

void DirectoryIteratorImpl::findFirstFile(const tstring& sFindPath)
{
	m_fileHandle = FindFirstFile(sFindPath.c_str(), &m_findData);
	if (m_fileHandle == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() != ERROR_NO_MORE_FILES)
			FileImpl::handleLastError(sFindPath);
	}
	else
	{
		m_sCurrent = m_findData.cFileName;
		if (m_sCurrent == _T(".") || m_sCurrent == _T(".."))
			doNext();
	}
}

FX_NS_END
