#include "firtex/utility/DirectoryIterator.h"

#if defined(FX_WINDOWS)
#include "firtex/utility/DirectoryIterator_WIN32.h"
#elif defined(FX_POSIX) || defined(FX_MACOS)
#include "firtex/utility/DirectoryIterator_POSIX.h"
#else
#error "No DirectoryIterator Implementation"
#endif

FX_NS_DEF(utility);

DirectoryIterator::DirectoryIterator()
    : m_pImpl(NULL)
    , m_bRecursive(false)
{
}
	
DirectoryIterator::DirectoryIterator(const tstring& path, bool bRecursive)
    : m_path(path)
    , m_pImpl(new DirectoryIteratorImpl(path))
    , m_bRecursive(bRecursive)
{
    m_path.makeDirectory();
    m_path.setFileName(m_pImpl->getCurrent());
    m_file = m_path;
}

DirectoryIterator::DirectoryIterator(const File& file, bool bRecursive)
    : m_path(file.getPath())
    , m_pImpl(new DirectoryIteratorImpl(file.getPath()))
    , m_bRecursive(bRecursive)
{
    m_path.makeDirectory();
    m_path.setFileName(m_pImpl->getCurrent());
    m_file = m_path;
}

DirectoryIterator::DirectoryIterator(const Path& path, bool bRecursive)
    : m_path(path)
    , m_pImpl(new DirectoryIteratorImpl(path.toString()))
    , m_bRecursive(bRecursive)
{
    m_path.makeDirectory();
    m_path.setFileName(m_pImpl->getCurrent());
    m_file = m_path;
}

DirectoryIterator::~DirectoryIterator()
{
    clear();    
}

DirectoryIterator& DirectoryIterator::operator = (const File& file)
{
    clear();
    m_pImpl = new DirectoryIteratorImpl(file.getPath());
    m_path.parseDirectory(file.getPath());
    m_path.setFileName(m_pImpl->getCurrent());
    m_file = m_path;
    return *this;
}

DirectoryIterator& DirectoryIterator::operator = (const Path& path)
{
    clear();
    
    m_pImpl = new DirectoryIteratorImpl(path.toString());
    m_path = path;
    m_path.makeDirectory();
    m_path.setFileName(m_pImpl->getCurrent());
    m_file = m_path;
    return *this;
}

DirectoryIterator& DirectoryIterator::operator = (const tstring& path)
{
    clear();
    m_pImpl = new DirectoryIteratorImpl(path);
    m_path.parseDirectory(path);
    m_path.setFileName(m_pImpl->getCurrent());
    m_file = m_path;
    return *this;
}

void DirectoryIterator::reset()
{
    clear();
    
    m_pImpl = new DirectoryIteratorImpl(m_path.toString());
    m_path.setFileName(m_pImpl->getCurrent());
    m_file = m_path;
}

const File& DirectoryIterator::next()
{
    m_path.setFileName(m_pImpl->getCurrent());
    m_file = m_path;
    if(m_bRecursive && m_file.isDirectory())
    {
        m_dirStack.push(m_pImpl);
        m_path.makeDirectory();            
        m_pImpl = new DirectoryIteratorImpl(m_path.toString());        
    }
    else
    {
        m_pImpl->doNext();
    }    
    while(m_bRecursive && !hasNext() && !m_dirStack.empty())
    {
        delete m_pImpl;
        m_pImpl = m_dirStack.top();
        m_dirStack.pop();
        m_path.popDirectory();        
        m_pImpl->doNext();
    }        
    return m_file;
}

bool DirectoryIterator::hasNext() const
{
    return (m_pImpl && !m_pImpl->getCurrent().empty());
}

void DirectoryIterator::clear()
{
    if (m_pImpl)
    {
        delete m_pImpl;
        m_pImpl = NULL;
    }

    DirectoryIteratorImpl* pImpl;    
    while (!m_dirStack.empty())
    {
        pImpl = m_dirStack.top();
        m_dirStack.pop();        
        delete pImpl;        
    }
}

FX_NS_END
