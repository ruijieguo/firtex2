#ifndef __FX_APPENDER_H
#define __FX_APPENDER_H

#include "firtex/common/StdHeader.h"
#include "firtex/common/SharedPtr.h"
#include "firtex/config/Configurable.h"
#include "firtex/utility/Singleton.h"

FX_NS_DEF(common);

#define DECLARE_APPENDER_CREATOR(AppenderClass, AppenderName)           \
    class Creator : public Appender::Creator                            \
                  , public FX_NS(utility)::Singleton<AppenderClass::Creator> \
    {                                                                   \
    protected:                                                          \
        Creator()                                                       \
        {                                                               \
            m_identifier += _T(".");                                    \
            m_identifier += AppenderName;                               \
        }                                                               \
        virtual ~Creator(){}                                            \
    public:                                                             \
        virtual Appender* create() const {return new AppenderClass;}    \
        DECLARE_LAZY_SINGLETON(AppenderClass::Creator);                 \
    };                                                                  \
public:                                                                 \
std::string getIdentifier() const {return AppenderClass::Creator::instance()->getIdentifier();}

struct LoggingEvent;

class Appender : public FX_NS(config)::Configurable
{
public:
    class Creator
    {
    protected:
        Creator() : m_identifier(_T("appender")) {}
        virtual ~Creator(){}

    public:
        virtual Appender* create() const = 0;		
        virtual const std::string& getIdentifier() const {return m_identifier;}

    protected:
        std::string m_identifier;
    };
public:
    Appender() {} 
    virtual ~Appender() {}

public:
    virtual std::string getIdentifier() const = 0;
    virtual void append(const LoggingEvent& event) = 0;
    virtual void flush() = 0;
};

DEFINE_TYPED_PTR(Appender);

FX_NS_END

#endif
