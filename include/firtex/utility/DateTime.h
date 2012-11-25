#ifndef __FX_DATETIME_H
#define __FX_DATETIME_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "firtex/common/StdHeader.h"
#include "firtex/utility/Exception.h"
#include "firtex/utility/Timestamp.h"
#include "firtex/utility/Timespan.h"
#include <ostream>
#include <istream>
#include <string>

FX_NS_DEF(utility);

class DateTime
{
public:
    enum
    {
        UTC = 0xFFFF /// Special value for timeZoneDifferential denoting UTC. 
    };

    enum Months
    {
        JANUARY = 1,
        FEBRUARY,
        MARCH,
        APRIL,
        MAY,
        JUNE,
        JULY,
        AUGUST,
        SEPTEMBER,
        OCTOBER,
        NOVEMBER,
        DECEMBER
    };
    
    enum DaysOfWeek
    {
        SUNDAY = 0,
        MONDAY,
        TUESDAY,
        WEDNESDAY,
        THURSDAY,
        FRIDAY,
        SATURDAY
    };

public:
    DateTime();
    DateTime(const Timestamp& timestamp);

    /**
     * Create a DateTime for the given Gregorian date and time.
     * @param year is from 0 to 9999.
     * @param month is from 1 to 12.
     * @param day is from 1 to 31.
     * @param hour is from 0 to 23.
     * @param minute is from 0 to 59.
     * @param second is from 0 to 59.
     * @param millisecond is from 0 to 999.
     * @param microsecond is from 0 to 999.
     */
    DateTime(int year, int month, int day, int hour = 0, int minute = 0, 
             int second = 0, int millisecond = 0, int microsecond = 0);

    /** Creates a DateTime from an UtcTimeVal and a TimeDiff.*/
    DateTime(Timestamp::UtcTimeVal utcTime, Timestamp::TimeDiff diff);
    DateTime(const DateTime& dateTime);
    ~DateTime();

public:
    DateTime& operator = (const DateTime& dateTime);
    DateTime& operator = (const Timestamp& timestamp);
    /**
     * Assigns a Gregorian date and time.
     * @param year is from 0 to 9999.
     * @param month is from 1 to 12.
     * @param day is from 1 to 31.
     * @param hour is from 0 to 23.
     * @param minute is from 0 to 59.
     * @param second is from 0 to 59.
     * @param millisecond is from 0 to 999.
     * @param microsecond is from 0 to 999.
     */
    DateTime& assign(int year, int month, int day, int hour = 0, int minute = 0,
                     int second = 0, int millisecond = 0, int microseconds = 0);

    void swap(DateTime& dateTime);

    int year() const;
    int month() const;
    int day() const;
    /**
     * Return the weekday (0 to 6, where
     * 0 = Sunday, 1 = Monday, ..., 6 = Saturday).
     */
    int dayOfWeek() const;

    int hour() const;
    int hourAMPM() const;
    bool isAM() const;
    bool isPM() const;
    int minute() const;
    int second() const;
    int millisecond() const;
    int microsecond() const;

    Timestamp timestamp() const;

    /** 
     * Return the date and time expressed in UTC-based
     * time. 
     */
    Timestamp::UtcTimeVal utcTime() const;
    double julianDay() const;

public:
    bool operator == (const DateTime& dateTime) const;	
    bool operator != (const DateTime& dateTime) const;	
    bool operator <  (const DateTime& dateTime) const;	
    bool operator <= (const DateTime& dateTime) const;	
    bool operator >  (const DateTime& dateTime) const;	
    bool operator >= (const DateTime& dateTime) const;	

    DateTime  operator +  (const Timespan& span) const;
    DateTime  operator -  (const Timespan& span) const;
    Timespan  operator -  (const DateTime& dateTime) const;
    DateTime& operator += (const Timespan& span);
    DateTime& operator -= (const Timespan& span);

public:
    /**
     * Formats the given timestamp according to the given format.
     * The format string is used as a template to format the date and
     * is copied character by character except for the following special characters,
     * which are replaced by the corresponding value.
     *
     *   * %w - abbreviated weekday (Mon, Tue, ...)
     *   * %W - full weekday (Monday, Tuesday, ...)
     *   * %b - abbreviated month (Jan, Feb, ...)
     *   * %B - full month (January, February, ...)
     *   * %d - zero-padded day of month (01 .. 31)
     *   * %e - day of month (1 .. 31)
     *   * %f - space-padded day of month ( 1 .. 31)
     *   * %m - zero-padded month (01 .. 12)
     *   * %n - month (1 .. 12)
     *   * %o - space-padded month ( 1 .. 12)
     *   * %y - year without century (70)
     *   * %Y - year with century (1970)
     *   * %H - hour (00 .. 23)
     *   * %h - hour (00 .. 12)
     *   * %a - am/pm
     *   * %A - AM/PM
     *   * %M - minute (00 .. 59)
     *   * %S - second (00 .. 59)
     *   * %i - millisecond (000 .. 999)
     *   * %c - centisecond (0 .. 9)
     *   * %F - fractional seconds/microseconds (000000 - 999999)
     *   * %z - time zone differential in ISO 8601 format (Z or +NN.NN).
     *   * %Z - time zone differential in RFC format (GMT or +NNNN)
     *   * %% - percent sign
     */
    void format(std::string& str, const std::string& fmt, int timeZoneDifferential = UTC);

public:
    /** 
     * Converts a local time into UTC,
     * by applying the given time zone differential. 
     */
    void makeUTC(int tzd);

    /**
     * Converts a UTC time into a local time,
     * by applying the given time zone differential.
     */
    void makeLocal(int tzd);

    /**
     * Return true if the given year is a leap year;
     * false otherwise.
     */
    static bool isLeapYear(int year);

    /**
     * Returns the number of days in the given month
     * and year. Month is from 1 to 12.
     */
    static int daysOfMonth(int year, int month);
    
    static bool isValid(int year, int month, int day, int hour = 0, int minute = 0,
                        int second = 0, int millisecond = 0, int microsecond = 0);
    
protected:
    /** Computes the Julian day for an UTC time.*/
    static double toJulianDay(Timestamp::UtcTimeVal utcTime);

    /**
     * Computes the Julian day for a gregorian calendar date and time.
     * See <http://vsg.cape.com/~pbaum/date/jdimp.htm>, section 2.3.1 for the algorithm.
     */	
    static double toJulianDay(int year, int month, int day, int hour = 0, int minute = 0,
                              int second = 0, int millisecond = 0, int microsecond = 0);

    /** Computes the UTC time for a Julian day.*/
    static Timestamp::UtcTimeVal toUtcTime(double julianDay);

    /**
     * Formats the given timezone differential in ISO format
     * and appends it to the given string.
     * If timeZoneDifferential is UTC, "Z" is returned,
     * otherwise, +HH.MM (or -HH.MM) is returned.
     */
    static void tzdISO(std::string& str, int timeZoneDifferential);

    /**
     * Formats the given timezone differential in RFC format
     * and appends it to the given string.
     * If timeZoneDifferential is UTC, "GMT" is returned,
     * otherwise ++HHMM (or -HHMM) is returned.
     */
    static void tzdRFC(std::string& str, int timeZoneDifferential);

    /**
     * Return the time zone differential for the current timezone.
     * The timezone differential is computed as utcOffset() + dst()
     * and is expressed in seconds.
     */
    static int tzd();

    /**
     * Return the offset of local time to UTC, in seconds.
     * local time = UTC + utcOffset() + dst().
     */
    static int utcOffset();

    /**
     * Return the daylight saving time offset in seconds if
     * daylight saving time is in use.
     * local time = UTC + utcOffset() + dst().
     */
    static int dst();

public:
    /**
     * Computes the Gregorian date for the given Julian day.
     * See <http://vsg.cape.com/~pbaum/date/injdimp.htm>, section 3.3.1 for the algorithm.
     */
    void computeGregorian(double julianDay);

    /** Extracts the daytime (hours, minutes, seconds, etc.) from the stored utcTime.*/
    void computeDaytime();

private:
    void checkLimit(short& lower, short& higher, short limit);

    /**utility functions used to correct the overflow in computeGregorian*/
    void normalize();

private:
    Timestamp::UtcTimeVal m_utcTime;
    short  m_year;
    short  m_month;
    short  m_day;
    short  m_hour;
    short  m_minute;
    short  m_second;
    short  m_millisecond;
    short  m_microsecond;

public:
    static const std::string WEEKDAY_NAMES[7];
    static const std::string MONTH_NAMES[12];

    /**
     * The date/time format defined in the ISO 8601 standard.
     * Examples: 
     *   2005-01-01T12:00:00+01:00
     *   2005-01-01T11:00:00Z
     */
    static const std::string ISO8601_FORMAT;

    /**
     * The date/time format defined in RFC 822 (obsoleted by RFC 1123).
     * Examples: 
     *   Sat, 1 Jan 05 12:00:00 +0100
     *   Sat, 1 Jan 05 11:00:00 GMT
     */
    static const std::string RFC822_FORMAT;

    /**
     * The date/time format defined in RFC 1123 (obsoletes RFC 822).
     * Examples: 
     *   Sat, 1 Jan 2005 12:00:00 +0100
     *   Sat, 1 Jan 2005 11:00:00 GMT
     */
    static const std::string RFC1123_FORMAT;

    /**
     * The date/time format defined in the HTTP specification (RFC 2616),
     *
     * Examples: 
     *   Sat, 01 Jan 2005 12:00:00 +0100
     */
    static const std::string HTTP_FORMAT;

    /**
     * The date/time format defined in RFC 850 (obsoleted by RFC 1036).
     * Examples: 
     * Saturday, 1-Jan-05 12:00:00 +0100
     * Saturday, 1-Jan-05 11:00:00 GMT
     */
    static const std::string RFC850_FORMAT;

    /**
     * The date/time format defined in RFC 1036 (obsoletes RFC 850).
     * Examples: 
     *   Saturday, 1 Jan 05 12:00:00 +0100
     *   Saturday, 1 Jan 05 11:00:00 GMT
     */
    static const std::string RFC1036_FORMAT;

    /**
     * The date/time format produced by the ANSI C asctime() function.
     * Example: Sat Jan  1 12:00:00 2005
     */
    static const std::string ASCTIME_FORMAT;

    /**
     * A simple, sortable date/time format.
     * Example:   2005-01-01 12:00:00
     */
    static const std::string SORTABLE_FORMAT;
};


////////////////////////////////////////////////////////////////
// inlines
//
inline Timestamp DateTime::timestamp() const
{
    return Timestamp::fromUtcTime(m_utcTime);
}

inline Timestamp::UtcTimeVal DateTime::utcTime() const
{
    return m_utcTime;
}

inline int DateTime::year() const
{
    return m_year;
}

inline int DateTime::month() const
{
    return m_month;
}

inline int DateTime::day() const
{
    return m_day;
}

inline int DateTime::hour() const
{
    return m_hour;
}

inline int DateTime::hourAMPM() const
{
    if (m_hour < 1)
        return 12;
    else if (m_hour > 12)
        return m_hour - 12;
    else
        return m_hour;
}

inline bool DateTime::isAM() const
{
    return m_hour < 12;
}

inline bool DateTime::isPM() const
{
    return m_hour >= 12;
}

inline int DateTime::minute() const
{
    return m_minute;
}

inline int DateTime::second() const
{
    return m_second;
}

inline int DateTime::millisecond() const
{
    return m_millisecond;
}

inline int DateTime::microsecond() const
{
    return m_microsecond;
}

inline bool DateTime::operator == (const DateTime& dateTime) const
{
    return m_utcTime == dateTime.m_utcTime;
}

inline bool DateTime::operator != (const DateTime& dateTime) const	
{
    return m_utcTime != dateTime.m_utcTime;
}

inline bool DateTime::operator <  (const DateTime& dateTime) const	
{
    return m_utcTime < dateTime.m_utcTime;
}

inline bool DateTime::operator <= (const DateTime& dateTime) const
{
    return m_utcTime <= dateTime.m_utcTime;
}

inline bool DateTime::operator >  (const DateTime& dateTime) const
{
    return m_utcTime > dateTime.m_utcTime;
}

inline bool DateTime::operator >= (const DateTime& dateTime) const	
{
    return m_utcTime >= dateTime.m_utcTime;
}

inline bool DateTime::isLeapYear(int year)
{
    return (year % 4) == 0 && ((year % 100) != 0 || (year % 400) == 0);
}

inline void swap(DateTime& d1, DateTime& d2)
{
    d1.swap(d2);
}

inline double DateTime::toJulianDay(Timestamp::UtcTimeVal utcTime)
{
    double utcDays = double(utcTime)/864000000000.0;
    return utcDays + 2299160.5; // first day of Gregorian reform (Oct 15 1582)
}

inline Timestamp::UtcTimeVal DateTime::toUtcTime(double julianDay)
{
    return Timestamp::UtcTimeVal((julianDay - 2299160.5)*864000000000.0);
}

FX_NS_END

#endif
