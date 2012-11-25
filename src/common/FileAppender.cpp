#include "firtex/common/FileAppender.h"
#include "firtex/common/LoggingLayout.h"
#include "firtex/utility/File.h"
#include "firtex/utility/Exception.h"
#include <cctype>

using namespace std;
FX_NS_USE(config);
FX_NS_USE(thread);
FX_NS_USE(utility);
FX_NS_DEF(common);

const string FileAppender::CONF_PATH          = "path";
const string FileAppender::CONF_SECONDARYPATH = "secondary_path";
const string FileAppender::CONF_ROTATION      = "rotation";

FileAppender::FileAppender()
    : m_nLimit(0)
    , m_bAutoFlush(false)
    , m_pFile(NULL)
{
}

FileAppender::FileAppender(const string& path)
    : m_sPath(path)
    , m_sSecondaryPath(path + ".0")
    , m_nLimit(0)
    , m_bAutoFlush(false)
    , m_pFile(NULL)
{
}

FileAppender::~FileAppender()
{
    close();
}

void FileAppender::open()
{
    FastMutex::Guard lock(m_mutex);
    
    if (!m_pFile)
    {
        File primary(m_sPath);
        File secondary(m_sSecondaryPath);
        Timestamp pt = primary.exists() ? primary.lastModified() : 0;
        Timestamp st = secondary.exists() ? secondary.lastModified() : 0;
        string path;
        if (pt >= st)
            path = m_sPath;
        else
            path = m_sSecondaryPath;
        m_pFile = new TextFile;
        m_pFile->open(path, "w");
    }
}

void FileAppender::close()
{
    FastMutex::Guard lock(m_mutex);
    
    if (m_pFile)
    {
        m_pFile->close();
        delete m_pFile;
        m_pFile = NULL;
    }
}

void FileAppender::append(const LoggingEvent& event)
{
    if (!m_pFile)
    { 
        open();
    }

    FastMutex::Guard lock(m_mutex);

    if (m_nLimit > 0 && m_pFile->getSizeInBytes() >= (int64_t)m_nLimit)
    {
        rotate();
    }
    
    std::ostringstream out;
    LoggingLayout& layout = getLayout();
    layout.format(out, event);

    m_pFile->write(out.str().c_str());

    if (m_bAutoFlush || event.level <= LoggingLevel::LEVEL_WARN)
    {
        m_pFile->flush();
    }
}

void FileAppender::flush()
{
    FastMutex::Guard lock(m_mutex);
    if(m_pFile)
    {
        m_pFile->flush();
    }
}

void FileAppender::configure(Configurator& conf)
{
    FastMutex::Guard lock(m_mutex);

    if (conf.getMode() == Configurator::FROM_CONF)
    {
        if (!conf.findConf(CONF_PATH).hasNext())
        {
            FIRTEX_THROW(InvalidConfigException, "Missing [%s] in config file.", 
                         CONF_PATH.c_str());
        }
        conf.configure(CONF_PATH, m_sPath);

        conf.configure(CONF_SECONDARYPATH, m_sSecondaryPath, m_sPath + ".0");

        if (conf.findConf(CONF_ROTATION).hasNext())
        {
            conf.configure(CONF_ROTATION, m_sRotation);
            setRotation(m_sRotation);
        }
        
        conf.configure("flush", m_bAutoFlush, false);
    }
    else 
    {
        conf.configure(CONF_PATH, m_sPath);
        conf.configure(CONF_SECONDARYPATH, m_sSecondaryPath);
        conf.configure(CONF_ROTATION, m_sRotation);
        conf.configure("flush", m_bAutoFlush);
    }
}

Timestamp FileAppender::creationDate() const
{
    if (m_pFile)
    {
        File f(m_pFile->getFileName());
        return f.lastModified();
    }
    else
    {
        return 0;
    }
}

uint64_t FileAppender::getSize() const
{
    if (m_pFile)
        return m_pFile->getSizeInBytes();
    else
        return 0;
}

const string& FileAppender::getPath() const
{
    return m_sPath;
}

const string& FileAppender::getSecondaryPath() const
{
    return m_sSecondaryPath;
}

void FileAppender::setRotation(const string& rotation)
{
    string::const_iterator it  = rotation.begin();
    string::const_iterator end = rotation.end();
    uint64_t n = 0;
    while (it != end && std::isspace(*it)) ++it;
    while (it != end && std::isdigit(*it)) { n *= 10; n += *it++ - '0'; }
    while (it != end && std::isspace(*it)) ++it;
    string unit;
    while (it != end && std::isalpha(*it)) unit += *it++;

    if (unit == "K")
        m_nLimit = n * 1024;
    else if (unit == "M")
        m_nLimit = n * 1024 * 1024;
    else if (unit.empty())
        m_nLimit = n;
    else if (unit == "never")
        m_nLimit = 0;
    else
    {
        FIRTEX_THROW(BadParameterException, "rotation: [%s]", rotation.c_str());
    }
    m_sRotation = rotation;
}

void FileAppender::rotate()
{
    string newPath;
    if (m_pFile->getFileName() == m_sPath)
        newPath = m_sSecondaryPath;
    else
        newPath = m_sPath;
    File f(newPath);
    if (f.exists())
    {
        try
        {
            f.remove();
        }
        catch (...)
        {
        }
    }
    delete m_pFile;
    m_pFile = new TextFile();
    m_pFile->open(newPath, "w");
}

FX_NS_END
