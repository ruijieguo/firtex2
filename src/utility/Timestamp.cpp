#include "firtex/utility/Timestamp.h"
#include "firtex/utility/Exception.h"
#include <algorithm>

#if defined(FX_WINDOWS)
#include <windows.h>
#else
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/times.h>
#endif


FX_NS_DEF(utility)

Timestamp::Timestamp()
{
    update();
}

Timestamp::Timestamp(TimeVal tv)
{
    m_ts = tv;
}

Timestamp::Timestamp(const Timestamp& other)
{
    m_ts = other.m_ts;
}

Timestamp::~Timestamp()
{
}

Timestamp& Timestamp::operator = (const Timestamp& other)
{
    m_ts = other.m_ts;
    return *this;
}

Timestamp& Timestamp::operator = (TimeVal tv)
{
    m_ts = tv;
    return *this;
}

void Timestamp::swap(Timestamp& timestamp)
{
    std::swap(m_ts, timestamp.m_ts);
}

Timestamp Timestamp::fromTime(std::time_t t)
{
    return Timestamp(TimeVal(t)*resolution());
}

Timestamp Timestamp::fromUtcTime(UtcTimeVal val)
{
    val -= (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
    val /= 10;
    return Timestamp(val);
}

void Timestamp::update()
{
#if defined(FX_WINDOWS)

    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
    epoch.LowPart  = 0xD53E8000;
    epoch.HighPart = 0x019DB1DE;

    ULARGE_INTEGER ts;
    ts.LowPart  = ft.dwLowDateTime;
    ts.HighPart = ft.dwHighDateTime;
    ts.QuadPart -= epoch.QuadPart;
    m_ts = ts.QuadPart/10;

#else

    struct timeval tv;
    if (gettimeofday(&tv, NULL))
        FIRTEX_THROW(RuntimeException, "cannot get time of day");
    m_ts = TimeVal(tv.tv_sec)*resolution() + tv.tv_usec;
#endif
}


#if defined(FX_WINDOWS)

Timestamp Timestamp::fromFileTimeNP(uint32_t fileTimeLow, uint32_t fileTimeHigh)
{
    ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
    epoch.LowPart  = 0xD53E8000;
    epoch.HighPart = 0x019DB1DE;

    ULARGE_INTEGER ts;
    ts.LowPart  = fileTimeLow;
    ts.HighPart = fileTimeHigh;
    ts.QuadPart -= epoch.QuadPart;

    return Timestamp(ts.QuadPart/10);
}

void Timestamp::toFileTimeNP(uint32_t& fileTimeLow, uint32_t& fileTimeHigh) const
{
    ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
    epoch.LowPart  = 0xD53E8000;
    epoch.HighPart = 0x019DB1DE;

    ULARGE_INTEGER ts;
    ts.QuadPart  = m_ts * 10;
    ts.QuadPart += epoch.QuadPart;
    fileTimeLow  = ts.LowPart;
    fileTimeHigh = ts.HighPart;
}
#endif

FX_NS_END

