#ifndef __FX_PATH_H
#define __FX_PATH_H

#include "firtex/common/StdHeader.h"
#include <vector>

FX_NS_DEF(utility);

/**
 * @class Path
 * @brief This class represents filesystem paths in a 
 * platform-independent manner.
 */
class Path
{
public:
    enum Style
    {
        PATH_UNIX,    /// Unix-style path
        PATH_WINDOWS, /// Windows-style path
        PATH_NATIVE,  /// The current platform's native style
        PATH_GUESS    /// Guess the style by examining the path
    };
	
    typedef std::vector<std::string> StringVec;

    /**
     * \if EN
     * Creates an empty relative path.
     * \endif
     */
    Path();

    /**
     * \if EN
     * Creates an empty absolute or relative path.
     * @param absolute absolute or relative path.
     * \endif
     */
    explicit Path(bool absolute);

    /**
     * \if EN
     * Creates a path from a string.
     * @param path path in native format
     * \endif
     */
    Path(const char* szPath);

    /**
     * \if EN
     * Creates a path from a string.
     * @param path path in native format
     * \endif
     */
    Path(const std::string& sPath);

    /**
     * \if EN
     * Creates a path from a string.
     * @param path path in native format
     * @param style path style
     * \endif
     */
    Path(const std::string& sPath, Style style);

    /**
     * \if EN
     * Copy constructor
     * \endif
     */
    Path(const Path& path);

    /**
     * \if EN
     * Creates a path from a parent path and a filename.
     * @param parent expected to reference to a directory.
     * @param file file name
     * \endif
     */
    Path(const Path& parent, const std::string& fileName);

    /**
     * \if EN
     * Creates a path from a parent path and a relative path.
     * @param parent expected to reference to a directory.
     * @param relative relative path
     * \endif
     */
    Path(const Path& parent, const Path& relative);

    /**
     * \if EN
     * Destroys the Path.
     * \endif
     */
    ~Path();
    
public:
    /**
     * \if EN
     * Assignment operator.
     * \endif
     */
    Path& operator = (const Path& path);

    /**
     * \if EN
     * Assigns a string containing a path in native format.
     * @param path path in native format
     * \endif
     */
    Path& operator = (const std::string& sPath);

    /**
     * \if EN
     * Assigns a string containing a path in native format.
     * @param path path in native format
     * \endif
     */
    Path& operator = (const char* szPath);

    /**
     * \if EN
     * Swaps the path with another one.
     * \endif
     */
    void swap(Path& path);

    /**
     * \if EN
     * Assigns a string containing a path in native format.
     * @param path path in native format
     * \endif
     */
    Path& assign(const std::string& sPath);

    /**
     * \if EN
     * Assigns a string containing a path.
     * @param path path in native format
     * @param style path style
     * \endif
     */
    Path& assign(const std::string& sPath, Style style);

    /**
     * \if EN
     * Assigns the given path.
     * \endif
     */
    Path& assign(const Path& path);

    /**
     * \if EN
     * Assigns a string containing a path.
     * @param path path in native format
     * \endif
     */
    Path& assign(const char* szPath);

    /**
     * \if EN
     * Return a string containing the path in native format.
     * @return path path in native format
     * \endif
     */
    std::string toString() const;

    /**
     * \if EN
     * Return a string containing the path in the given format.
     * @param style path style
     * @return path in native format
     * \endif
     */
    std::string toString(Style style) const;

    /**
     * \if EN
     * Parse a string containing the path in native format
     * @param path path in native format
     * \endif
     */
    Path& parse(const std::string& sPath);

    /**
     * \if EN
     * Assigns a string containing a path.
     * @param path path in native format
     * @param style path sty
     * \endif
     */
    Path& parse(const std::string& sPath, Style style);

    /**
     * \if EN
     * Tries to interpret the given string as a path
     * in native format.
     * If the path is syntactically valid, assigns the
     * path and return true. Otherwise leaves the 
     * object unchanged and returns false.
     * \endif
     */
    bool tryParse(const std::string& sPath);

    /**
     * \if EN
     * Tries to interpret the given string as a path,
     * according to the given style.
     * If the path is syntactically valid, assigns the
     * path and returns true. Otherwise leaves the
     * object unchanged and returns false.
     * \endif
     */
    bool tryParse(const std::string& sPath, Style style);

    /**
     * \if EN
     * The resulting path always refers to a directory and
     * the filename part is empty.
     * \endif
     */
    Path& parseDirectory(const std::string& sPath);

    /**
     * \if EN
     * The resulting path always refers to a directory and
     * the filename part is empty.
     * \endif
     */
    Path& parseDirectory(const std::string& sPath, Style style);

    /**
     * \if EN
     * If the path contains a filename, the filename is appended
     * to the directory list and cleared. Thus the resulting path
     * always refers to a directory.
     * \endif
     */
    Path& makeDirectory();

    /**
     * \if EN
     * If the path contains no filename, the last directory
     * becomes the filename.
     * \endif
     */
    Path& makeFile();

    /**
     * \if EN
     * Makes the path refer to its parent.
     * \endif
     */
    Path& makeParent();

    /**
     * \if EN
     * Makes the path absolute if it is relative.
     * The current working directory is taken as base directory.
     * \endif
     */
    Path& makeAbsolute();

    /**
     * \if EN
     * Makes the path absolute if it is relative.
     * The given path is taken as base.
     * \endif
     */
    Path& makeAbsolute(const Path& base);

    /**
     * \if EN
     * Appends the given path.
     * \endif
     */
    Path& append(const Path& path);

    /**
     * \if EN
     * Resolves the given path agains the current one.
     *
     * If the given path is absolute, it replaces the current one.
     * Otherwise, the relative path is appended to the current path.
     * \endif
     */
    Path& resolve(const Path& path);

    /**
     * \if EN
     * Return true iff the path is absolute.
     * \endif
     */
    bool isAbsolute() const;

    /**
     * \if EN
     * Return true iff the path is relative.
     * \endif
     */
    bool isRelative() const;

    /**
     * \if EN
     * Return true iff the path references a directory
     * (the filename part is empty).
     * \endif
     */
    bool isDirectory() const;

    /**
     * \if EN
     * Return true iff the path references a file
     * (the filename part is not empty).
     * \endif
     */
    bool isFile() const;

    /**
     * \if EN
     * Set the node name.
     * Setting a non-empty node automatically makes
     * the path an absolute one.
     * \endif
     */
    void setNode(const std::string& node);

    /**
     * \if EN
     * Return the node name.
     * \endif
     */
    const std::string& getNode() const;

    /**
     * \if EN
     * Set the device name.
     * Setting a non-empty device automatically makes
     * the path an absolute one.
     * \endif
     */
    void setDevice(const std::string& device);

    /**
     * \if EN
     * Return the device name.
     * \endif
     */
    const std::string& getDevice() const;

    /**
     * \if EN
     * Return the number of directories in the directory list.
     * \endif
     */
    size_t depth() const;

    /**
     * \if EN
     * Return the n'th directory in the directory list.
     * If n == depth(), returns the filename.
     * \endif
     */
    const std::string& directory(size_t n) const;

    /**
     * \if EN
     * Return the n'th directory in the directory list.
     * If n == depth(), returns the filename.
     * \endif
     */
    const std::string& operator [] (size_t n) const;

    /**
     * \if EN
     * Adds a directory to the directory list.
     * \endif
     */
    void pushDirectory(const std::string& dir);

    /**
     * \if EN
     * Removes the last directory from the directory list.
     * \endif
     */
    void popDirectory();

    /**
     * \if EN
     * Set the filename.
     * \endif
     */
    void setFileName(const std::string& name);

    /**
     * \if EN
     * Return the filename.
     * \endif
     */
    const std::string& getFileName() const;

    /**
     * \if EN
     * Set the basename part of the filename and
     * does not change the extension.
     * \endif
     */
    void setBaseName(const std::string& name);

    /**
     * \if EN
     * Return the basename (the filename and extension) of the path.
     * \endif
     */
    std::string getBaseName() const;

    /**
     * \if EN
     * Set the filename extension.
     * \endif
     */
    void setExtension(const std::string& extension);

    /**
     * \if EN
     * Return the filename extension.
     * \endif
     */
    std::string getExtension() const;

    /**
     * \if EN
     * Clears all components.
     * \endif
     */
    void clear();

    /**
     * \if EN
     * Return a path referring to the path's
     * directory.
     * \endif
     */
    Path parent() const;

    /**
     * \if EN
     * Return an absolute variant of the path,
     * taking the current working directory as base.
     * \endif
     */
    Path absolute() const;

    /**
     * \if EN
     * Return an absolute variant of the path,
     * taking the given path as base.
     * \endif
     */
    Path absolute(const Path& base) const;

public:
    /**
     * \if EN
     * Creates a path referring to a directory.
     * \endif
     */
    static Path forDirectory(const std::string& sPath);

    /**
     * \if EN
     * Creates a path referring to a directory.
     * \endif
     */
    static Path forDirectory(const std::string& sPath, Style style);

    /**
     * \if EN
     * Return the platform's path name separator, which separates
     * the components (names) in a path. 
     * On Unix systems, this is the slash '/'. On Windows systems, 
     * this is the backslash '\'.
     * \endif
     */
    static char separator();

    /**
     * \if EN
     * Return the platform's path separator, which separates
     * single paths in a list of paths.
     * @return On Unix systems, return colon ':'. On Windows systems,
     * return semicolon ';'.
     * \endif
     */
    static char pathSeparator();

    /**
     * \if EN
     * Return the current working directory.
     * \endif
     */
    static std::string current();

    /**
     * \if EN
     * Return the user's home directory.
     * \endif
     */
    static std::string home();

    /**
     * \if EN
     * Return the temporary directory.
     * \endif
     */
    static std::string temp();

    /**
     * \if EN
     * Expands all environment variables contained in the path.
     *
     * On Unix, a tilde as first character in the path is
     * replaced with the path to user's home directory.

     * \endif
     */
    static std::string expand(const std::string& sPath);

protected:
    void parseUnix(const std::string& sPath);
    void parseWindows(const std::string& sPath);
    void parseGuess(const std::string& sPath);
    std::string buildUnix() const;
    std::string buildWindows() const;

private:
    std::string m_sNode;
    std::string m_sDevice;
    std::string m_sName;
    StringVec m_vDirs;
    bool m_bAbsolute;
};

//
// inlines
//
inline bool Path::isAbsolute() const
{
    return m_bAbsolute;
}

inline bool Path::isRelative() const
{
    return !m_bAbsolute;
}

inline bool Path::isDirectory() const
{
    return m_sName.empty();
}

inline bool Path::isFile() const
{
    return !m_sName.empty();
}

inline Path& Path::parse(const std::string& sPath)
{
    return assign(sPath);
}

inline Path& Path::parse(const std::string& sPath, Style style)
{
    return assign(sPath, style);
}

inline const std::string& Path::getNode() const
{
    return m_sNode;
}

inline const std::string& Path::getDevice() const
{
    return m_sDevice;
}

inline const std::string& Path::getFileName() const
{
    return m_sName;
}

inline size_t Path::depth() const
{
    return size_t(m_vDirs.size());
}

inline Path Path::forDirectory(const std::string& sPath)
{
    Path p;
    return p.parseDirectory(sPath);
}

inline Path Path::forDirectory(const std::string& sPath, Style style)
{
    Path p;
    return p.parseDirectory(sPath, style);
}

inline char Path::separator()
{
#if defined(FX_WINDOWS)
    return _T('\\');
#else
    return _T('/');
#endif
}

inline char Path::pathSeparator()
{
#if defined(FX_WINDOWS)
    return ';';
#else
    return ':';
#endif
}

inline void swap(Path& p1, Path& p2)
{
    p1.swap(p2);
}

FX_NS_END

#endif
