#ifndef __FILE_H
#define __FILE_H

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Timestamp.h"
#include "firtex/utility/Exception.h"
#include <vector>

#if defined FX_WINDOWS
#include "firtex/utility/File_WIN32.h"
#elif defined FX_POSIX || defined FX_MACOS
#include "firtex/utility/File_LINUX.h"
#else
#error "No File Implement."
#endif

FX_NS_DEF(utility)

class Path;

class File
{
public:
    typedef FileImpl::FileSizeImpl FileSize;
    typedef std::vector<File> FileVector;
    
public:
    File();
    File(const std::string& path);
    File(const Path& path);
    File(const File& file);
    virtual ~File();
public:	
    File& operator = (const File& file);
    File& operator = (const std::string& path);
    File& operator = (const tchar* path);
    File& operator = (const Path& path);

    /** Swaps the file with another one. */
    void swap(File& file);

    /** Returns the path. */
    const std::string& getPath() const;

    /**
     * set the path
     */
    void setPath(const std::string& sPath);
     
    /** Returns true iff the file exists. */
    bool exists() const;		

    /** Returns true iff the file is a regular file.*/
    bool isFile() const;		
		
    /** Returns true iff the file is a directory.*/
    bool isDirectory() const;

    /** Returns true iff the file is a symbolic link.*/
    bool isLink() const;		

    /** Returns the size of the file in bytes.*/
    FileSize getSize() const;	
		
    /** Sets the size of the file in bytes. Can be used
     * to truncate a file.
     */
    void setSize(FileSize size);	

    /**
     * get created time of file
     */
    Timestamp created() const;

    /**
     * get the modification date of the file.
     */
    Timestamp lastModified() const;
		
    /** Sets the modification date of the file.*/
    void setLastModified(const Timestamp& ts);
		
    /**
     * Copies the file (or directory) to the given path. 
     * The target path can be a directory.
     * A directory is copied recursively.
     */
    void copyTo(const std::string& path) const;

    /** 
     * Copies the file (or directory) to the given path and 
     * removes the original file. The target path can be a directory.
     */
    void moveTo(const std::string& path);

    /**
     * Rename the file to the new name.
     */
    void renameTo(const std::string& path);
		
    /**
     * Delete the file. 
     * @param bRecursive if true and the file is a directory,
     * recursively deletes all files in the directory.		
     */
    void remove(bool bRecursive = false);
		
    /** 
     * Creates a new, empty file in an atomic operation.
     * @return true if the file has been created and false
     * if the file already exists. Throws an exception if
     * an error occurs.
     */
    bool createFile();
	
    /**
     * Creates a directory. 
     * @return true if the directory has been created 
     * and false if it already exists.
     *
     * @throw an exception if an error occurs.
     */
    bool createDirectory();
	
    /**
     * Creates a directory (and all parent directories
     * if necessary).	
     * @return true if the directory has been created 
     * and false if it already exists.
     *
     * @throw an exception if an error occurs.
     */
    bool createDirectories();
		
    /** 
     * Fills the vector with the names of all
     * files in the directory.
     * @param files vector to store files
     * @param bRecursive true for recursive list
     */
    void list(std::vector<std::string>& files, bool bRecursive) const;
		
    /**
     * Fills the vector with the names of all
     * files in the directory.
     * @param files vector to store files
     * @param bRecursive true for recursive list
     */
    void list(FileVector& files, bool bRecursive) const;

    bool operator == (const File& file) const;
    bool operator != (const File& file) const;
public:
    /** Returns true iff the file exists. */
    static bool exists(const std::string& sFile);
    static void handleLastError(const std::string& sPath);

protected:
    /** Copies a directory. Used internally by copyTo(). */
    void copyDirectory(const std::string& path) const;

private:
    std::string m_sPath;
};


////////////////////////////////////////////////////////////////////////////////
// inlines
//
inline const std::string& File::getPath() const
{
     return m_sPath;     
}

inline bool File::operator == (const File& file) const
{
     return (m_sPath == file.getPath());     
}

inline bool File::operator != (const File& file) const
{
     return (m_sPath != file.getPath());
}

inline void swap(File& f1, File& f2)
{
     f1.swap(f2);
}

FX_NS_END

#endif

