#include "firtex/store/BlockFileSystem.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Path.h"
#include "firtex/utility/NumberFormatter.h"
#include "firtex/utility/NumberParser.h"
#include "firtex/utility/KeyValueParser.h"
#include "firtex/utility/Exception.h"
#include "firtex/thread/ScopedLock.h"
#include "firtex/store/BlockFileOutputStream.h"
#include "firtex/store/BlockFileInputStream.h"
#include <iostream>
#include <sstream>

using namespace std;

FX_NS_USE(utility);
FX_NS_USE(thread);

FX_NS_DEF(store);

SETUP_STREAM_LOGGER(store, BlockFileSystem);

BlockFileSystem::BlockFileSystem() 
    : m_mode(FileSystem::READ)
    , m_nBlockSizeOfOS((uint32_t)FX_OS_BLOCKSIZE())
    , m_nBlockSize(m_nBlockSizeOfOS)
{
}

BlockFileSystem::~BlockFileSystem() 
{
    close();
}

void BlockFileSystem::open(const string& sFsPath, FileSystem::OpenMode mode)
{
    ScopedLock<FastMutex> lock(m_lock);

    Path path(sFsPath);
    path.makeDirectory();
    m_sFsPath = path.toString() + Path::separator();
    m_mode = mode;
    if (mode == FileSystem::READ)
    {
        if (sFsPath.empty() || !FX_NS(utility)::File::exists(sFsPath))
        {
            FIRTEX_THROW(FileIOException, "Directory : [%s] doese not exist.",
                         sFsPath.c_str());
        }
    }
    else if(mode == FileSystem::CREATE || FileSystem::APPEND)
    {
        if (sFsPath.empty() || !FX_NS(utility)::File::exists(sFsPath)) //direcotry does not exist
        {
            FX_NS(utility)::File f(sFsPath);
            if (!f.createDirectory())
            {
                FIRTEX_THROW(FileIOException, "Create directory : [%s] FAILED", sFsPath.c_str());
            }
        }

        if (mode == FileSystem::CREATE)
        {
            clearFiles();
        }
    }
}

void BlockFileSystem::close()
{
    ScopedLock<FastMutex> lock(m_lock);
    if (m_filesOpened.size() != 0)
    {
        stringstream ss;
        ss << "There are some files still opened: [";
        for (FileMap::const_iterator it = m_filesOpened.begin();
             it != m_filesOpened.end(); ++it)
        {
            ss << m_sFsPath + it->first << "(" << it->second.size() << "); ";
            for (FileList::const_iterator fileIt = it->second.begin();
                 fileIt != it->second.end(); ++fileIt)
            {
                if (FX_OS_CLOSE((*fileIt)->fileHandle))
                {
                    FX_NS(utility)::File::handleLastError(m_sFsPath + it->first);
                }
            }
        }
        ss << "]";
        FX_LOG(WARN, "%s", ss.str().c_str());
    }
}

void BlockFileSystem::clearFiles()
{
    if (!m_sFsPath.empty())
    {
        FX_NS(utility)::File f(m_sFsPath);
        if (f.exists())
        {
            f.remove(true);
            f.createDirectory();
        }
    }
}

OutputStreamPtr BlockFileSystem::createFile(const string& sFileName)
{
    FilePtr file = doCreateFile(sFileName);
    if (!file)
    {
        return OutputStreamPtr();
    }
    return OutputStreamPtr(new BlockFileOutputStream(*this, file));
}

BlockFileSystem::FilePtr BlockFileSystem::doCreateFile(const string& sFileName)
{
    ScopedLock<FastMutex> lock(m_lock);

    string str = toLower(sFileName);
    if (!isPermitted(str, true))
    {
        FIRTEX_THROW(FileIOException, "File: [%s] is opened, write permission is denied",
                     sFileName.c_str());
    }

    FileHandle fd = FX_OS_OPEN_FOR_CRTR(m_sFsPath + str);
    if (FX_OS_IS_VALID_FILE_DESCRIPTOR(fd))
    {

        FilePtr pFile(new File(str, fd, false));
        FileList& fileList = m_filesOpened[str];
        fileList.push_back(pFile);
        return pFile;
    }
    else
    {
        FX_NS(utility)::File::handleLastError(m_sFsPath + sFileName);
    }
    return FilePtr();
}

InputStreamPtr BlockFileSystem::openFile(const string& sFileName)
{
    FX_DEBUG("Open file: [%s]", sFileName.c_str());
    FilePtr file = doOpenFile(sFileName);
    if (!file)
    {
        return InputStreamPtr();
    }
    return InputStreamPtr(new BlockFileInputStream(*this, file));
}

BlockFileSystem::FilePtr BlockFileSystem::doOpenFile(const string& sFileName)
{
    string str = toLower(sFileName);

    ScopedLock<FastMutex> lock(m_lock);

    if (!isPermitted(str, false))
    {
        FIRTEX_THROW(FileIOException, "File: [%s] is opened, read "
                     "permission is denied", sFileName.c_str());
    }

    FileHandle fd = FX_OS_OPEN_FOR_READ(m_sFsPath + str);
    if (!FX_OS_IS_VALID_FILE_DESCRIPTOR(fd))
    {
        FX_NS(utility)::File::handleLastError(m_sFsPath + sFileName);
    }

    FilePtr pFile(new File(str, fd, true));
    FX_NS(utility)::File f(m_sFsPath + str);
    pFile->fileActualSize = (offset_t)f.getSize();;
    pFile->totalBlocks = (pFile->fileActualSize + m_nBlockSize - 1) 
                         / m_nBlockSize;

    FX_TRACE("Open file: %s, size: %llu",
             sFileName.c_str(), pFile->fileActualSize);

    FileMap::iterator it = m_filesOpened.find(str);
    if (it != m_filesOpened.end())
    {
        FileList& fileList = it->second;
        fileList.push_back(pFile);        
    }
    else
    {
        FileList& fileList = m_filesOpened[str];
        fileList.push_back(pFile);
    }
    return pFile;
}

MMapFilePtr BlockFileSystem::mmapFile(const std::string& sFileName)
{
    string str = toLower(sFileName);

    ScopedLock<FastMutex> lock(m_lock);

    if (!isPermitted(str, false))
    {
        FIRTEX_THROW(FileIOException, "File: [%s] is opened, read permission is denied",
                     sFileName.c_str());
    }
    
    FX_NS(utility)::File file(m_sFsPath + str);
    MMapFilePtr pFile = new MMapFile();
    pFile->open(file, MMapFile::AM_READ);
    return pFile;
}

void BlockFileSystem::closeFile(FilePtr& pFile)
{
    FX_DEBUG("Close file: %s", pFile->fileName.c_str());

    ScopedLock<FastMutex> lock(m_lock);

    FileMap::iterator it = m_filesOpened.find(pFile->fileName);
    if (it == m_filesOpened.end())
    {
        FIRTEX_THROW(FileIOException, "File: [%s] is not opened yet",
                     pFile->fileName.c_str());
    }

    if (pFile.referenceCount() > 2)
    {
        /// some one still refer to this file
        pFile.reset();
        return ;
    }

    FileList& fileList = m_filesOpened[pFile->fileName];
    fileList.remove_if(ListElemRemover(pFile));
    if (fileList.size() == 0)
    {
        m_filesOpened.erase(it);
    }

    if (!pFile->readOnly && FX_OS_FTRUNCATE(pFile->fileHandle, pFile->fileActualSize))
    {
        FX_NS(utility)::File::handleLastError(m_sFsPath + pFile->fileName);
    }

    if (FX_OS_CLOSE(pFile->fileHandle))
    {
        FX_NS(utility)::File::handleLastError(m_sFsPath + pFile->fileName);
    }
}

void BlockFileSystem::readBlock(FilePtr& pFile, blockid_t blockId, void* block)
{
    FX_OS_OFFSET offset = blockIdToFileOffset(blockId);
    if (FX_OS_LSEEK(pFile->fileHandle, offset, FX_OS_FLAG_SEEK_SET) != offset)
    {
        FX_NS(utility)::File::handleLastError(m_sFsPath + pFile->fileName);
    }
    
    FX_OS_OFFSET left = (pFile->fileActualSize - offset);
    uint32_t uReadSize = (left > m_nBlockSize) ? 
                         m_nBlockSize : (uint32_t)left;
    if (FX_OS_READ(pFile->fileHandle, block, uReadSize) != (ssize_t)uReadSize)
    {
        FX_NS(utility)::File::handleLastError(m_sFsPath + pFile->fileName);
    }
    if (uReadSize < m_nBlockSize)
    {
        memset((char*)block + uReadSize, 0, m_nBlockSize - uReadSize);
    }
}

void BlockFileSystem::writeBlock(FilePtr& pFile, blockid_t blockId, const void* block)
{
    FX_OS_OFFSET offset = blockIdToFileOffset(blockId);
    if (FX_OS_LSEEK(pFile->fileHandle, offset, FX_OS_FLAG_SEEK_SET) != offset)
    {
        FX_NS(utility)::File::handleLastError(m_sFsPath + pFile->fileName);
    }

    if (FX_OS_WRITE(pFile->fileHandle, block, m_nBlockSize) != 
        (ssize_t)m_nBlockSize)
    {
        FX_NS(utility)::File::handleLastError(m_sFsPath + pFile->fileName);
    }
}

size_t BlockFileSystem::openedFiles()
{
    ScopedLock<FastMutex> lock(m_lock);
    size_t openedFiles = 0;
    for (FileMap::const_iterator it = m_filesOpened.begin();
           it != m_filesOpened.end(); ++it)
    {
        openedFiles += it->second.size();
    }
    return openedFiles;
}

bool BlockFileSystem::exists(const string& sFileName)
{
    ScopedLock<FastMutex> lock(m_lock);
    if (m_sFsPath.empty() && sFileName.empty())
    {
        return false;
    }
    FX_NS(utility)::File f(m_sFsPath + sFileName);
    return f.exists();
}

bool BlockFileSystem::isPermitted(const string& sFileName, bool bForWrite) const
{
    FileMap::const_iterator it = m_filesOpened.find(sFileName);
    if (it == m_filesOpened.end())
    {
        return true;
    }
    else 
    {
        if (bForWrite)
        {
            return false;
        }
        else 
        {
            const FileList& fileList = it->second;
            return fileList.front()->readOnly;
        }
    }
}

bool BlockFileSystem::fileExists(const string& sFileName)
{
    ScopedLock<FastMutex> lock(m_lock);

    if (m_sFsPath.empty() && sFileName.empty())
    {
        return false;
    }

    string str = toLower(sFileName);    
    
    FX_NS(utility)::File f(m_sFsPath + sFileName);
    return f.exists();
}

void BlockFileSystem::deleteFile(const string& sFileName)
{
    ScopedLock<FastMutex> lock(m_lock);
    if (m_mode == READ)
    {
        FIRTEX_THROW(FileIOException, "Block file system instance : [%s]"
                     " is in readonly mode, deletion is denied.",  m_sFsPath.c_str());
    }

    string str = toLower(sFileName);

    FileMap::iterator fmIt = m_filesOpened.find(str);
    if (fmIt != m_filesOpened.end())
    {
        FIRTEX_THROW(FileIOException, "Delete file [%s] FAILED, "
                     "file is still opened.", sFileName.c_str());
    }

    FX_NS(utility)::File f(m_sFsPath + str);
    f.remove();    
}

void BlockFileSystem::renameFile(const string& sFrom,
                                 const string& sTo)
{
    ScopedLock<FastMutex> lock(m_lock);
    
    if (m_mode == READ)
    {
        FIRTEX_THROW(FileIOException, "Block file system instance : [%s]"
                     " is in readonly mode, rename is denied.", m_sFsPath.c_str());
            
    }

    string str = toLower(sFrom);

    FileMap::iterator fmIt = m_filesOpened.find(str);
    if (fmIt != m_filesOpened.end())
    {
        FIRTEX_THROW(FileIOException, "Rename file [%s] FAILED, "
                     "file is still opend.", sFrom.c_str());
    }

    string str2 = toLower(sTo);
    FX_NS(utility)::File f(m_sFsPath + str);
    f.renameTo(m_sFsPath + str2);
}

void BlockFileSystem::moveTo(const string& sFrom, const string& sTo)
{
    ScopedLock<FastMutex> lock(m_lock);
    
    if (m_mode == READ)
    {
        FIRTEX_THROW(FileIOException, "Block file system instance : [%s]"
                     " is in readonly mode, rename is denied.", m_sFsPath.c_str());
            
    }

    string str = toLower(sFrom);

    FileMap::iterator fmIt = m_filesOpened.find(str);
    if (fmIt != m_filesOpened.end())
    {
        FIRTEX_THROW(FileIOException, "Rename file [%s] FAILED, "
                     "file is still opend.", sFrom.c_str());
    }

    string str2 = toLower(sTo);
    FX_NS(utility)::File f(m_sFsPath + str);
    f.moveTo(m_sFsPath + str2);
}

FileSystem::FileSet BlockFileSystem::listFiles()
{
    ScopedLock<FastMutex> lock(m_lock);

    FX_NS(utility)::File f(m_sFsPath);
    vector<tstring> files;
    f.list(files, true);

    FileSet fileSet;
    for (vector<tstring>::const_iterator it = files.begin();
         it != files.end(); ++it)
    {
        fileSet.insert(*it);
    }
    return fileSet;
}

void BlockFileSystem::makeDirectory(const std::string& sDir, bool bRecursive)
{
    ScopedLock<FastMutex> lock(m_lock);

    FX_NS(utility)::Path p(m_sFsPath + sDir);
    p.makeDirectory();
    FX_NS(utility)::File f(p);
    if (bRecursive)
    {
        f.createDirectories();
    }
    else
    {
        if (!f.createDirectory())
        {
            FIRTEX_THROW_AND_LOG(FileIOException, "Create directory: [%s] FAILED",
                    sDir.c_str());
        }
    }
}

void BlockFileSystem::deleteDirectory(const std::string& sDir, bool bRecursive)
{
    ScopedLock<FastMutex> lock(m_lock);
    FX_NS(utility)::Path p(m_sFsPath + sDir);
    p.makeDirectory();
    FX_NS(utility)::File f(p);
    f.remove(bRecursive);
}

Timestamp BlockFileSystem::fileModified(const string& sFileName)
{
    ScopedLock<FastMutex> lock(m_lock);
    FX_NS(utility)::File f(m_sFsPath + sFileName);
    return f.lastModified();
}


FX_NS_END

