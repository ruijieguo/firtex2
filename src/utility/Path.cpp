#include "firtex/utility/Path.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Exception.h"
#include <algorithm>

#if defined(FX_WINDOWS)
#include "firtex/utility/Path_WIN32.h"
#else
#include "firtex/utility/Path_LINUX.h"
#endif

FX_NS_DEF(utility);

Path::Path(): m_bAbsolute(false)
{
}

Path::Path(bool absolute): m_bAbsolute(absolute)
{
}

Path::Path(const char* szPath)
{
    assign(szPath);
}

Path::Path(const std::string& sPath)
{
    assign(sPath);
}

Path::Path(const std::string& sPath, Style style)
{
    assign(sPath, style);
}

Path::Path(const Path& path)
    : m_sNode(path.m_sNode)
    , m_sDevice(path.m_sDevice)
    , m_sName(path.m_sName)
    , m_vDirs(path.m_vDirs)
    , m_bAbsolute(path.m_bAbsolute)
{	
}

Path::Path(const Path& parent, const std::string& fileName)
    : m_sNode(parent.m_sNode)
    , m_sDevice(parent.m_sDevice)
    , m_sName(parent.m_sName)
    , m_vDirs(parent.m_vDirs)
    , m_bAbsolute(parent.m_bAbsolute)
{	
    makeDirectory();
    m_sName = fileName;
}

Path::Path(const Path& parent, const Path& relative)
    : m_sNode(parent.m_sNode)
    , m_sDevice(parent.m_sDevice)
    , m_sName(parent.m_sName)
    , m_vDirs(parent.m_vDirs)
    , m_bAbsolute(parent.m_bAbsolute)
{	
    resolve(relative);
}

Path::~Path()
{
}

Path& Path::operator = (const Path& path)
{
    return assign(path);
}

Path& Path::operator = (const std::string& sPath)
{
    return assign(sPath);
}

Path& Path::operator = (const char* szPath)
{
    FIRTEX_ASSERT2(szPath);
    return assign(szPath);
}

void Path::swap(Path& path)
{
    std::swap(m_sNode, path.m_sNode);
    std::swap(m_sDevice, path.m_sDevice);
    std::swap(m_sName, path.m_sName);
    std::swap(m_vDirs, path.m_vDirs);
    std::swap(m_bAbsolute, path.m_bAbsolute);
}

Path& Path::assign(const Path& path)
{
    if (&path != this)
    {
        m_sNode     = path.m_sNode;
        m_sDevice   = path.m_sDevice;
        m_sName     = path.m_sName;
        m_vDirs     = path.m_vDirs;
        m_bAbsolute = path.m_bAbsolute;
    }
    return *this;
}

Path& Path::assign(const std::string& sPath)
{
#if defined(FX_WINDOWS)
    parseWindows(sPath);
#else
    parseUnix(sPath);
#endif
    return *this;
}

Path& Path::assign(const std::string& sPath, Style style)
{
    switch (style)
    {
    case PATH_UNIX:
        parseUnix(sPath);
        break;
    case PATH_WINDOWS:
        parseWindows(sPath);
        break;
    case PATH_NATIVE:
        assign(sPath);
        break;
    case PATH_GUESS:
        parseGuess(sPath);
        break;
    default:
        throw BadParameterException();
    }
    return *this;
}

Path& Path::assign(const char* szPath)
{
    return assign(std::string(szPath));
}

std::string Path::toString() const
{
#if defined(FX_WINDOWS)
    return buildWindows();
#else
    return buildUnix();
#endif
}

std::string Path::toString(Style style) const
{
    switch (style)
    {
    case PATH_UNIX:
        return buildUnix();
    case PATH_WINDOWS:
        return buildWindows();
    case PATH_NATIVE:
    case PATH_GUESS:
        return toString();
    default:
        throw BadParameterException();
    }
    return std::string();
}

bool Path::tryParse(const std::string& sPath)
{
    try
    {
        Path p;
        p.parse(sPath);
        assign(p);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool Path::tryParse(const std::string& sPath, Style style)
{
    try
    {
        Path p;
        p.parse(sPath, style);
        assign(p);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

Path& Path::parseDirectory(const std::string& sPath)
{
    assign(sPath);
    return makeDirectory();
}

Path& Path::parseDirectory(const std::string& sPath, Style style)
{
    assign(sPath, style);
    return makeDirectory();
}

Path& Path::makeDirectory()
{
    pushDirectory(m_sName);
    m_sName.clear();
    return *this;
}

Path& Path::makeFile()
{
    if (!m_vDirs.empty() && m_sName.empty())
    {
        m_sName = m_vDirs.back();
        m_vDirs.pop_back();
    }
    return *this;
}

Path& Path::makeAbsolute()
{
    return makeAbsolute(current());
}

Path& Path::makeAbsolute(const Path& base)
{
    if (!m_bAbsolute)
    {
        Path tmp = base;
        tmp.makeDirectory();
        for (StringVec::const_iterator it = m_vDirs.begin(); it != m_vDirs.end(); ++it)
        {
            tmp.pushDirectory(*it);
        }
        m_sNode     = tmp.m_sNode;
        m_sDevice   = tmp.m_sDevice;
        m_vDirs     = tmp.m_vDirs;
        m_bAbsolute = base.m_bAbsolute;
    }
    return *this;
}

Path Path::absolute() const
{
    Path result(*this);
    if (!result.m_bAbsolute)
    {
        result.makeAbsolute();
    }
    return result;
}

Path Path::absolute(const Path& base) const
{
    Path result(*this);
    if (!result.m_bAbsolute)
    {
        result.makeAbsolute(base);
    }
    return result;
}

Path Path::parent() const
{
    Path p(*this);
    return p.makeParent();
}

Path& Path::makeParent()
{
    if (m_sName.empty())
    {
        if (m_vDirs.empty())
        {
            if (!m_bAbsolute)
                m_vDirs.push_back("..");
        }
        else
        {
            if (m_vDirs.back() == "..")
                m_vDirs.push_back("..");
            else
                m_vDirs.pop_back();
        }
    }
    else
    {
        m_sName.clear();
    }
    return *this;
}

Path& Path::append(const Path& path)
{
    makeDirectory();
    m_vDirs.insert(m_vDirs.end(), path.m_vDirs.begin(), path.m_vDirs.end());
    m_sName = path.m_sName;
    return *this;
}

Path& Path::resolve(const Path& path)
{
    if (path.isAbsolute())
    {
        assign(path);
    }
    else
    {
        for (size_t i = 0; i < path.depth(); ++i)
            pushDirectory(path[i]);
        m_sName = path.m_sName;
    }
    return *this;
}

void Path::setNode(const std::string& node)
{
    m_sNode     = node;
    m_bAbsolute = m_bAbsolute || !node.empty();
}

void Path::setDevice(const std::string& device)
{
    m_sDevice   = device;
    m_bAbsolute = m_bAbsolute || !device.empty();
}

const std::string& Path::directory(size_t n) const
{
    FIRTEX_ASSERT2 (0 <= n && n <= m_vDirs.size());
	
    if (n < m_vDirs.size())
        return m_vDirs[n];
    else
        return m_sName;	
}

const std::string& Path::operator [] (size_t n) const
{
    FIRTEX_ASSERT2 (0 <= n && n <= m_vDirs.size());
	
    if (n < m_vDirs.size())
        return m_vDirs[n];
    else
        return m_sName;	
}

void Path::pushDirectory(const std::string& dir)
{
    if (!dir.empty() && dir != ".")
    {
        if (dir == "..")
        {
            if (!m_vDirs.empty() && m_vDirs.back() != "..")
                m_vDirs.pop_back();
            else if (!m_bAbsolute)
                m_vDirs.push_back(dir);
        }
        else m_vDirs.push_back(dir);
    }
}

void Path::popDirectory()
{
    FIRTEX_ASSERT2 (!m_vDirs.empty());
	
    m_vDirs.pop_back();
}

void Path::setFileName(const std::string& name)
{
    m_sName = name;
}

void Path::setBaseName(const std::string& name)
{
    std::string ext = getExtension();
    m_sName = name;
    if (!ext.empty())
    {
        m_sName.append(".");
        m_sName.append(ext);
    }
}

std::string Path::getBaseName() const
{
    std::string::size_type pos = m_sName.rfind('.');
    if (pos != std::string::npos)
        return m_sName.substr(0, pos);
    else
        return m_sName;
}

void Path::setExtension(const std::string& extension)
{
    m_sName = getBaseName();
    if (!extension.empty())
    {
        m_sName.append(".");
        m_sName.append(extension);
    }
}

std::string Path::getExtension() const
{
    std::string::size_type pos = m_sName.rfind('.');
    if (pos != std::string::npos)
        return m_sName.substr(pos + 1);
    else
        return std::string();
}

void Path::clear()
{
    m_sNode.clear();
    m_sDevice.clear();
    m_sName.clear();
    m_vDirs.clear();
    m_bAbsolute = false;
}

std::string Path::current()
{
    return PathImpl::currentImpl();
}

std::string Path::home()
{
    return PathImpl::homeImpl();
}

std::string Path::temp()
{
    return PathImpl::tempImpl();
}

std::string Path::expand(const std::string& sPath)
{
    return PathImpl::expandImpl(sPath);
}

void Path::parseUnix(const std::string& sPath)
{
    clear();

    std::string::const_iterator it  = sPath.begin();
    std::string::const_iterator end = sPath.end();

    if (it != end)
    {
        if (*it == '/') 
        {
            m_bAbsolute = true; ++it;
        }
        else if (*it == '~')
        {
            ++it;
            if (it == end || *it == '/')
            {
                Path cwd(home());
                m_vDirs = cwd.m_vDirs;
                m_bAbsolute = true;
            }
            else --it;
        }

        while (it != end)
        {
            std::string name;
            while (it != end && *it != '/') name += *it++;
            if (it != end)
            {
                if (m_vDirs.empty())
                {
                    if (!name.empty() && *(name.rbegin()) == ':')
                        m_sDevice.assign(name, 0, name.length() - 1);
                    else
                        pushDirectory(name);
                }
                else pushDirectory(name);
            }
            else m_sName = name;
            if (it != end) ++it;
        }
    }
}

void Path::parseWindows(const std::string& sPath)
{
    clear();

    std::string::const_iterator it  = sPath.begin();
    std::string::const_iterator end = sPath.end();

    if (it != end)
    {
        if (*it == '\\' || *it == '/') { m_bAbsolute = true; ++it; }
        if (m_bAbsolute && it != end && (*it == '\\' || *it == '/')) // UNC
        {
            ++it;
            while (it != end && *it != '\\' && *it != '/') m_sNode += *it++;
            if (it != end) ++it;
        }
        else if (it != end)
        {
            char d = *it++;
            if (it != end && *it == ':') // drive letter
            {
                if (m_bAbsolute || !((d >= 'a' && d <= 'z') || (d >= 'A' && d <= 'Z')))
                {
                    FIRTEX_THROW(BadParameterException, "Bad parameter: [%s]", sPath.c_str());
                }
                m_bAbsolute = true;
                m_sDevice += d;
                ++it;
                if (it == end || (*it != '\\' && *it != '/'))
                {
                    FIRTEX_THROW(BadParameterException, "Bad parameter: [%s]", sPath.c_str());
                }
                ++it;
            }
            else --it;
        }
        while (it != end)
        {
            std::string name;
            while (it != end && *it != '\\' && *it != '/') name += *it++;
            if (it != end)
                pushDirectory(name);
            else
                m_sName = name;
            if (it != end) ++it;
        }
    }
    if (!m_sNode.empty() && m_vDirs.empty() && !m_sName.empty())
        makeDirectory();
}

void Path::parseGuess(const std::string& sPath)
{
    bool hasBackslash   = false;
    bool hasSlash       = false;
//    bool hasOpenBracket = false;
//    bool hasClosBracket = false;
    bool isWindows      = sPath.length() > 2 && sPath[1] == ':' && (sPath[2] == '/' || sPath[2] == '\\');
    std::string::const_iterator end    = sPath.end();
    std::string::const_iterator semiIt = end;
    if (!isWindows)
    {
        for (std::string::const_iterator it = sPath.begin(); it != end; ++it)
        {
            switch (*it)
            {
            case '\\': hasBackslash = true; break;
            case '/':  hasSlash = true; break;
//            case '[':  hasOpenBracket = true;
//            case ']':  hasClosBracket = hasOpenBracket; 
            case ';':  semiIt = it; break;
            }
        }
    }
    if (hasBackslash || isWindows)
    {
        parseWindows(sPath);
    }
    else if (hasSlash)
    {
        parseUnix(sPath);
    }
    else
    {
        parseUnix(sPath);
    }
}

std::string Path::buildUnix() const
{
    std::string result;
    if (!m_sDevice.empty())
    {
        result.append("/");
        result.append(m_sDevice);
        result.append(":/");
    }
    else if (m_bAbsolute)
    {
        result.append("/");
    }
    for (StringVec::const_iterator it = m_vDirs.begin(); 
         it != m_vDirs.end(); ++it)
    {
        result.append(*it);
        result.append("/");
    }
    if (!m_sName.empty())
    {
        result.append(m_sName);
    }
    else if (result.size() > 0)
    {
        result.erase(result.size() - 1, 1);
    }
    return result;
}

std::string Path::buildWindows() const
{
    std::string result;
    if (!m_sNode.empty())
    {
        result.append("\\\\");
        result.append(m_sNode);
        result.append("\\");
    }
    else if (!m_sDevice.empty())
    {
        result.append(m_sDevice);
        result.append(":\\");
    }
    else if (m_bAbsolute)
    {
        result.append("\\");
    }
    for (StringVec::const_iterator it = m_vDirs.begin(); it != m_vDirs.end(); ++it)
    {
        result.append(*it);
        result.append("\\");
    }
    if (!m_sName.empty())
    {
        result.append(m_sName);
    }
    else
    {
        result.erase(result.size() - 1, 1);
    }
    return result;
}

FX_NS_END
