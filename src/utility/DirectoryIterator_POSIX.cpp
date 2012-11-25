#include "firtex/utility/DirectoryIterator_POSIX.h"
#include "firtex/utility/File_LINUX.h"
#include "firtex/utility/Path.h"

FX_NS_DEF(utility);

DirectoryIteratorImpl::DirectoryIteratorImpl(const tstring& path)
  : m_pDir(0)
{
    Path p(path);
    p.makeFile();

    m_pDir = opendir(p.toString().c_str());
    if (!m_pDir) 
    {
        FileImpl::handleLastError(path);
    }

    doNext();
}

DirectoryIteratorImpl::~DirectoryIteratorImpl()
{
    if (m_pDir)
    {
        closedir(m_pDir);
    }    
}

const tstring& DirectoryIteratorImpl::doNext()
{
	do
	{
		struct dirent* pEntry = readdir(m_pDir);
		if (pEntry)
        {
			m_sCurrent = pEntry->d_name;
        }
        else
        {
            m_sCurrent.clear();
        }        
	}while (m_sCurrent == _T(".") || m_sCurrent == _T(".."));
    return m_sCurrent;    
}

FX_NS_END
