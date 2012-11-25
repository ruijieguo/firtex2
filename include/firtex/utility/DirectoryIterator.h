#ifndef __DIRECTORYITERATOR_H
#define __DIRECTORYITERATOR_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Path.h"
#include <stack>

FX_NS_DEF(utility);

class DirectoryIteratorImpl;

/**
 * @class DirectoryIterator
 * @brief used to enumerate all files in a directory
 */
class DirectoryIterator
{
public:
    DirectoryIterator();
    DirectoryIterator(const std::string& path, bool bRecursive);
    DirectoryIterator(const DirectoryIterator& iterator, bool bRecursive);
    DirectoryIterator(const File& file, bool bRecursive);
    DirectoryIterator(const Path& path, bool bRecursive);
    ~DirectoryIterator();

    DirectoryIterator& operator = (const DirectoryIterator& it);
    DirectoryIterator& operator = (const File& file);
    DirectoryIterator& operator = (const Path& path);
    DirectoryIterator& operator = (const std::string& path);
public:
    /**
     * reset the iterator
     */
    void reset();
    
    /**
     * whether reach to end or not
     * @return true for not, false for yes
     */
    bool hasNext() const;
    
    /**
     * retrieval current element
     * @return current element
     */
    const File& next() ;

    /** Returns the current filename. */
    const std::string& getFileName() const;

    /** Returns the current file. */
    const File& getFile() const;

    /** Returns the current path. */
    const Path& getPath() const;

    /** set recursive flag */
    void setRecursive(bool bRecursive);

    /** get recursive flag */
    bool getRecursive() const;    
private:
    void clear();
private:
    typedef std::stack<DirectoryIteratorImpl*> DirectoryStack;

    DirectoryStack  m_dirStack;
    
    Path m_path;
    File m_file;
    DirectoryIteratorImpl* m_pImpl;
    bool m_bRecursive;
};

DEFINE_TYPED_PTR(DirectoryIterator);

////////////////////////////////////////////////////////////////////////////////
// inlines
//
inline const std::string& DirectoryIterator::getFileName() const
{
    return m_path.getFileName();
}
	
inline const Path& DirectoryIterator::getPath() const
{
    return m_path;
}

inline const File& DirectoryIterator::getFile() const
{
    return m_file;
}

inline void DirectoryIterator::setRecursive(bool bRecursive)
{
    m_bRecursive = bRecursive;
}

inline bool DirectoryIterator::getRecursive() const
{
    return m_bRecursive;
}

FX_NS_END

#endif 
