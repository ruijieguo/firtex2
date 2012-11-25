#ifndef __FXTIMESTAMP_H
#define __FXTIMESTAMP_H

#include "firtex/utility/Timestamp.h"

FX_NS_DEF(utility)

/**
 * @class Timespan
 * A class that represents time spans up to microsecond resolution.
 */
class Timespan
{
public:
    typedef Timestamp::TimeDiff TimeDiff;

    /** Creates a zero Timespan.*/
    Timespan();

    /** Creates a Timespan.*/
    Timespan(TimeDiff microseconds);
    
    /** Creates a Timespan. Useful for creating
     * a Timespan from a struct timeval.
     */
    Timespan(long seconds, long microseconds);

    /** Creates a Timespan.*/
    Timespan(int32_t days, int32_t hours, int32_t minutes,
             int32_t seconds, int32_t microseconds);

    /** Creates a Timespan from another one.*/
    Timespan(const Timespan& timespan);

    ~Timespan();
public:
    Timespan& operator = (const Timespan& timespan);
    Timespan& operator = (TimeDiff microseconds);

    /** Assigns a new span.*/
    Timespan& assign(int32_t days, int32_t hours, int32_t minutes, int32_t seconds, int32_t microseconds);
    Timespan& assign(long seconds, long microseconds);

    bool operator == (const Timespan& ts) const;
    bool operator != (const Timespan& ts) const;
    bool operator >  (const Timespan& ts) const;
    bool operator >= (const Timespan& ts) const;
    bool operator <  (const Timespan& ts) const;
    bool operator <= (const Timespan& ts) const;

    bool operator == (TimeDiff microseconds) const;
    bool operator != (TimeDiff microseconds) const;
    bool operator >  (TimeDiff microseconds) const;
    bool operator >= (TimeDiff microseconds) const;
    bool operator <  (TimeDiff microseconds) const;
    bool operator <= (TimeDiff microseconds) const;
    
    Timespan operator + (const Timespan& d) const;
    Timespan operator - (const Timespan& d) const;
    Timespan& operator += (const Timespan& d);
    Timespan& operator -= (const Timespan& d);

    Timespan operator + (TimeDiff microseconds) const;
    Timespan operator - (TimeDiff microseconds) const;
    Timespan& operator += (TimeDiff microseconds);
    Timespan& operator -= (TimeDiff microseconds);

    /** Returns the number of days.*/
    int32_t days() const;

    /** Returns the number of hours (0 to 23).*/
    int32_t hours() const;

    /** Returns the total number of hours.*/
    int32_t totalHours() const;

    /** Returns the number of minutes (0 to 59).*/
    int32_t minutes() const;

    /** Returns the total number of minutes.*/
    int32_t totalMinutes() const;

    /** Returns the number of seconds (0 to 59).*/
    int32_t seconds() const;

    /** Returns the total number of seconds.*/
    int32_t totalSeconds() const;

    /** Returns the number of milliseconds (0 to 999).*/
    int32_t milliseconds() const;

    /** Returns the total number of milliseconds.*/
    TimeDiff totalMilliseconds() const;

    /**
     * Returns the fractions of a millisecond
     * in microseconds (0 to 999).
     */
    int32_t microseconds() const;

    /**
     * Returns the fractions of a second
     * in microseconds (0 to 999999).
     */
    int32_t useconds() const;

    /** Returns the total number of microseconds. */
    TimeDiff totalMicroseconds() const;

public:
    static const TimeDiff MILLISECONDS; /// The number of microseconds in a millisecond.
    static const TimeDiff SECONDS;      /// The number of microseconds in a second.
    static const TimeDiff MINUTES;      /// The number of microseconds in a minute.
    static const TimeDiff HOURS;        /// The number of microseconds in a hour.
    static const TimeDiff DAYS;         /// The number of microseconds in a day.

public:
    static TimeDiff microseconds(TimeDiff msec)
    {
        return msec * MILLISECONDS;
    }

private:
    TimeDiff m_nSpan;
};


/////////////////////////////////////////////////////////
// inlines
//
inline int32_t Timespan::days() const
{
    return int32_t(m_nSpan/DAYS);
}

inline int32_t Timespan::hours() const
{
    return int32_t((m_nSpan/HOURS) % 24);
}

inline int32_t Timespan::totalHours() const
{
    return int32_t(m_nSpan/HOURS);
}

inline int32_t Timespan::minutes() const
{
    return int32_t((m_nSpan/MINUTES) % 60);
}
	
inline int32_t Timespan::totalMinutes() const
{
    return int32_t(m_nSpan/MINUTES);
}

inline int32_t Timespan::seconds() const
{
    return int32_t((m_nSpan/SECONDS) % 60);
}

inline int32_t Timespan::totalSeconds() const
{
    return int32_t(m_nSpan/SECONDS);
}

inline int32_t Timespan::milliseconds() const
{
    return int32_t((m_nSpan/MILLISECONDS) % 1000);
}

inline Timespan::TimeDiff Timespan::totalMilliseconds() const
{
    return m_nSpan/MILLISECONDS;
}

inline int32_t Timespan::microseconds() const
{
    return int32_t(m_nSpan % 1000);
}

inline int32_t Timespan::useconds() const
{
    return int32_t(m_nSpan % 1000000);
}

inline Timespan::TimeDiff Timespan::totalMicroseconds() const
{
    return m_nSpan;
}

inline bool Timespan::operator == (const Timespan& ts) const
{
    return m_nSpan == ts.m_nSpan;
}

inline bool Timespan::operator != (const Timespan& ts) const
{
    return m_nSpan != ts.m_nSpan;
}

inline bool Timespan::operator >  (const Timespan& ts) const
{
    return m_nSpan > ts.m_nSpan;
}

inline bool Timespan::operator >= (const Timespan& ts) const
{
    return m_nSpan >= ts.m_nSpan;
}

inline bool Timespan::operator <  (const Timespan& ts) const
{
    return m_nSpan < ts.m_nSpan;
}

inline bool Timespan::operator <= (const Timespan& ts) const
{
    return m_nSpan <= ts.m_nSpan;
}

inline bool Timespan::operator == (TimeDiff microseconds) const
{
    return m_nSpan == microseconds;
}

inline bool Timespan::operator != (TimeDiff microseconds) const
{
    return m_nSpan != microseconds;
}

inline bool Timespan::operator >  (TimeDiff microseconds) const
{
    return m_nSpan > microseconds;
}

inline bool Timespan::operator >= (TimeDiff microseconds) const
{
    return m_nSpan >= microseconds;
}

inline bool Timespan::operator <  (TimeDiff microseconds) const
{
    return m_nSpan < microseconds;
}

inline bool Timespan::operator <= (TimeDiff microseconds) const
{
    return m_nSpan <= microseconds;
}

FX_NS_END

#endif //
