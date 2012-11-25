#ifndef __FX_TIMESTAMP_H
#define __FX_TIMESTAMP_H

#include "firtex/common/StdHeader.h"
#include <ctime>

FX_NS_DEF(utility);

/**
 * @class Timestamp	
 * A Timestamp stores a monotonic time value
 * with (theoretical) microseconds resolution.
 * Timestamps can be compared with each other
 * and simple arithmetics are supported.
 * Timestamps are UTC (Coordinated Universal Time)
 * based and thus independent of the timezone
 * in effect on the system.
 */
class Timestamp	
{
public:
    typedef int64_t TimeVal;    /// monotonic UTC time value in microsecond resolution
    typedef int64_t UtcTimeVal; /// monotonic UTC time value in 100 nanosecond resolution
    typedef int64_t TimeDiff;   /// difference between two timestamps in microseconds

public:
    /** Creates a timestamp with the current time.*/
    Timestamp();
    
    /** Creates a timestamp from the given time value.*/
    Timestamp(TimeVal tv);

    /** Destroys the timestamp */
    Timestamp(const Timestamp& other);
    ~Timestamp();

    Timestamp& operator = (const Timestamp& other);
    Timestamp& operator = (TimeVal tv);

public:
    /** Swaps the Timestamp with another one. */
    void swap(Timestamp& timestamp);

    /** Updates the Timestamp with the current time. */
    void update();

    bool operator == (const Timestamp& ts) const;
    bool operator != (const Timestamp& ts) const;
    bool operator >  (const Timestamp& ts) const;
    bool operator >= (const Timestamp& ts) const;
    bool operator <  (const Timestamp& ts) const;
    bool operator <= (const Timestamp& ts) const;
 
    Timestamp  operator +  (TimeDiff d) const;
    Timestamp  operator -  (TimeDiff d) const;
    TimeDiff   operator -  (const Timestamp& ts) const;
    Timestamp& operator += (TimeDiff d);
    Timestamp& operator -= (TimeDiff d);

    /**
     * Returns the timestamp expressed in time_t.
     * time_t base time is midnight, January 1, 1970.
     * Resolution is one second.
     */
    std::time_t time() const;

    /**
     * Returns the timestamp expressed in UTC-based
     * time. UTC base time is midnight, October 15, 1582.
     * Resolution is 100 nanoseconds. 
     */
    UtcTimeVal utcTime() const;
    
    /**
     * Returns the timestamp expressed in microseconds
     * since the Unix epoch, midnight, January 1, 1970.
     */
    TimeVal microseconds() const;

    /**
     * Returns the time elapsed since the time denoted by
     * the timestamp. Equivalent to Timestamp() - *this.
     */
    TimeDiff elapsed() const;

    /**
     * Returns true iff the given interval has passed
     * since the time denoted by the timestamp.
     */
    bool isElapsed(TimeDiff interval) const;

    /** Creates a timestamp from a std::time_t.*/
    static Timestamp fromTime(std::time_t t);

    /** Creates a timestamp from a UTC time value.*/
    static Timestamp fromUtcTime(UtcTimeVal val);

    /**
     * Returns the resolution in units per second.
     * Since the timestamp has microsecond resolution,
     * the returned value is always 1000000.
     */
    static TimeVal resolution();

#if defined(_WIN32)
    static Timestamp fromFileTimeNP(uint32_t fileTimeLow, uint32_t fileTimeHigh);
    void toFileTimeNP(uint32_t& fileTimeLow, uint32_t& fileTimeHigh) const;
#endif

private:
    TimeVal m_ts;
};


/////////////////////////////////////////////////////////////
// inlines
//
inline bool Timestamp::operator == (const Timestamp& ts) const
{
    return m_ts == ts.m_ts;
}


inline bool Timestamp::operator != (const Timestamp& ts) const
{
    return m_ts != ts.m_ts;
}


inline bool Timestamp::operator >  (const Timestamp& ts) const
{
    return m_ts > ts.m_ts;
}


inline bool Timestamp::operator >= (const Timestamp& ts) const
{
    return m_ts >= ts.m_ts;
}

inline bool Timestamp::operator <  (const Timestamp& ts) const
{
    return m_ts < ts.m_ts;
}

inline bool Timestamp::operator <= (const Timestamp& ts) const
{
    return m_ts <= ts.m_ts;
}

inline Timestamp Timestamp::operator + (Timestamp::TimeDiff d) const
{
    return Timestamp(m_ts + d);
}

inline Timestamp Timestamp::operator - (Timestamp::TimeDiff d) const
{
    return Timestamp(m_ts - d);
}

inline Timestamp::TimeDiff Timestamp::operator - (const Timestamp& ts) const
{
    return m_ts - ts.m_ts;
}

inline Timestamp& Timestamp::operator += (Timestamp::TimeDiff d)
{
    m_ts += d;
    return *this;
}

inline Timestamp& Timestamp::operator -= (Timestamp::TimeDiff d)
{
    m_ts -= d;
    return *this;
}

inline std::time_t Timestamp::time() const
{
    return std::time_t(m_ts/resolution());
}

inline Timestamp::UtcTimeVal Timestamp::utcTime() const
{
    return m_ts*10 + (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
}

inline Timestamp::TimeVal Timestamp::microseconds() const
{
    return m_ts;
}

inline Timestamp::TimeDiff Timestamp::elapsed() const
{
    Timestamp now;
    return now - *this;
}

inline bool Timestamp::isElapsed(Timestamp::TimeDiff interval) const
{
    Timestamp now;
    Timestamp::TimeDiff diff = now - *this;
    return diff >= interval;
}

inline Timestamp::TimeVal Timestamp::resolution()
{
    return 1000000;
}

inline void swap(Timestamp& s1, Timestamp& s2)
{
    s1.swap(s2);
}

FX_NS_END

#endif

