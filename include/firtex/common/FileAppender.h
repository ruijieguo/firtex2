#ifndef __FX_SIMPLEFILECHANNEL_H
#define __FX_SIMPLEFILECHANNEL_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/LayoutAppender.h"
#include "firtex/utility/Timestamp.h"
#include "firtex/utility/TextFile.h"
#include "firtex/config/Configurator.h"
#include "firtex/thread/Mutex.h"
#include "firtex/common/Logger.h"

FX_NS_DEF(common);

/**
 * @class FileAppender
 * @brief A Appender that writes to a file. This class only
 * supports simple log file rotation.
 */
class FileAppender: public LayoutAppender
{
public:
    FileAppender();
    FileAppender(const std::string& path);

    DECLARE_APPENDER_CREATOR(FileAppender, "file");

public:
    /** Open the FileAppender and creates the log file if necessary.*/
    void open();

    /** Closes the FileAppender.*/
    void close();

    virtual void append(const LoggingEvent& event);

    /** flush log message to device */
    virtual void flush();

    /** 
     * Configure this appender
     * Following configure are supported:
     *   - path:          The primary log file's path.
     *   - secondary_path: The secondary log file's path.
     *   - rotation:      The log file's rotation mode, which supports the following values:
     *
     *     * never:         no log rotation
     *     * <n>:           the file is rotated when its size exceeds
     *                      <n> bytes.
     *     * <n> K:         the file is rotated when its size exceeds
     *                      <n> Kilobytes.
     *     * <n> M:         the file is rotated when its size exceeds
     *                      <n> Megabytes.
     */
    void configure(FX_NS(config)::Configurator& conf);
		
    /** Returns the log file's creation date.*/
    FX_NS(utility)::Timestamp creationDate() const;

    /** Return the log file's current size in bytes.*/
    uint64_t getSize() const;

    /** Returns the log file's primary path.*/
    const std::string& getPath() const;

    /** Returns the log file's secondary path.*/
    const std::string& getSecondaryPath() const;

public:
    static const std::string CONF_PATH;
    static const std::string CONF_SECONDARYPATH;
    static const std::string CONF_ROTATION;

public:
    ~FileAppender();

protected:
    void setRotation(const std::string& rotation);
    void rotate();

private:
    std::string m_sPath;
    std::string m_sSecondaryPath;
    std::string m_sRotation;
    uint64_t m_nLimit;
    bool m_bAutoFlush;
    FX_NS(utility)::TextFile* m_pFile;
    FX_NS(thread)::FastMutex m_mutex;
};

DEFINE_TYPED_PTR(FileAppender);

FX_NS_END

#endif // __FX_FILEAPPENDER_H
