#include "firtex/utility/File.h"
#include "firtex/utility/Exception.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/DirectoryIterator.h"


FX_NS_DEF(utility)

File::File()
{
}

File::File(const tstring& path): m_sPath (path)
{
    FileImpl::transPath(m_sPath);    
}

File::File(const Path& path): m_sPath(path.toString())
{
    FileImpl::transPath(m_sPath);
}

File::File(const File& file): m_sPath(file.getPath())
{
    FileImpl::transPath(m_sPath);
}

File::~File()
{
}

void File::setPath(const tstring& sPath)
{     
    m_sPath = sPath;
    FileImpl::transPath(m_sPath);
}

File& File::operator = (const File& file)
{
    setPath(file.getPath());
    return *this;
}

File& File::operator = (const tstring& path)
{
    setPath(path);
    return *this;
}

File& File::operator = (const tchar* path)
{
    FIRTEX_ASSERT2(path);
    setPath(path);
    return *this;
}

File& File::operator = (const Path& path)
{
    setPath(path.toString());
    return *this;
}

void File::swap(File& file)
{
    std::swap(m_sPath, file.m_sPath);
}

bool File::exists() const
{
    return FileImpl::exists(m_sPath);
}

bool File::isFile() const
{
    return FileImpl::isFile(m_sPath);
}
	
bool File::isDirectory() const
{
    return FileImpl::isDirectory(m_sPath);
}

bool File::isLink() const
{
    return FileImpl::isLink(m_sPath);
}

File::FileSize File::getSize() const
{
    return FileImpl::getSize(m_sPath);
}
	
void File::setSize(FileSize size)
{
    FileImpl::setSize(m_sPath, size);
}

Timestamp File::created() const
{
    return FileImpl::created(m_sPath);
}

	
Timestamp File::lastModified() const
{
    return FileImpl::lastModified(m_sPath);
}

	
void File::setLastModified(const Timestamp& ts)
{
    FileImpl::setLastModified(m_sPath, ts);
}
	
void File::copyTo(const tstring& path) const
{
    Path src(getPath());
    Path dest(path);
    File destFile(path);
    if ((destFile.exists() && destFile.isDirectory()) || dest.isDirectory())
    {
        dest.makeDirectory();
        dest.setFileName(src.getFileName());
    }
    if (isDirectory())
        copyDirectory(dest.toString());
    else
        FileImpl::copyTo(m_sPath, dest.toString());
}

void File::copyDirectory(const tstring& path) const
{
    File target(path);
    target.createDirectories();

    Path src(getPath());
    src.makeFile();
    DirectoryIterator it(src, false);
    while(it.hasNext())
    {
        it.next().copyTo(path);
    }
}

void File::moveTo(const tstring& path)
{
    copyTo(path);
    remove(true);
    setPath(path);
}

void File::renameTo(const tstring& path)
{
    FileImpl::renameTo(m_sPath, path);
    setPath(path);
}

void File::remove(bool bRecursive)
{
    if (bRecursive && !isLink() && isDirectory())
    {
        FileVector files;
        list(files, false);
        for (FileVector::iterator it = files.begin(); it != files.end(); ++it)
        {
            it->remove(true);
        }
    }
    FileImpl::remove(m_sPath);
}

bool File::createFile()
{
    return FileImpl::createFile(m_sPath);
}

bool File::createDirectory()
{
    return FileImpl::createDirectory(m_sPath);
}

bool File::createDirectories()
{
    if (!exists())
    {
        Path p(getPath());
        p.makeDirectory();
        if (p.depth() > 1)
        {
            p.makeParent();
            File f(p);
            if (!f.createDirectories())
            {
                return false;
            }
        }
        if (!FileImpl::createDirectory(m_sPath))
        {
            return false;
        }
    }
    return true;
}

void File::list(std::vector<tstring>& files, bool bRecursive) const
{
    files.clear();
    DirectoryIterator it(*this, bRecursive);
    while (it.hasNext())
    {
        it.next();        
        files.push_back(it.getFileName());
    }
}

void File::list(FileVector& files, bool bRecursive) const
{
    files.clear();
    DirectoryIterator it(*this, bRecursive);
    while (it.hasNext())
    {
        files.push_back(it.next());
    }
}

//static
bool File::exists(const tstring& sFile)
{
    return FileImpl::exists(sFile);
}

void File::handleLastError(const tstring& sPath)
{
    FileImpl::handleLastError(sPath);
}

FX_NS_END
