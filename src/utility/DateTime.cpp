#include "firtex/utility/DateTime.h"
#include "firtex/utility/NumberFormatter.h"
#include "firtex/utility/Exception.h"
#include <assert.h>
#include <time.h>
#include <cmath>
#include <ctime>

#ifdef FX_WINDOWS
#include <windows.h>
#endif


FX_NS_DEF(utility);

const std::string DateTime::ISO8601_FORMAT  = "%Y-%m-%dT%H:%M:%S%z";
const std::string DateTime::RFC822_FORMAT   = "%w, %e %b %y %H:%M:%S %Z";
const std::string DateTime::RFC1123_FORMAT  = "%w, %e %b %Y %H:%M:%S %Z";
const std::string DateTime::HTTP_FORMAT     = "%w, %d %b %Y %H:%M:%S %Z";
const std::string DateTime::RFC850_FORMAT   = "%W, %e-%b-%y %H:%M:%S %Z";
const std::string DateTime::RFC1036_FORMAT  = "%W, %e %b %y %H:%M:%S %Z";
const std::string DateTime::ASCTIME_FORMAT  = "%w %b %f %H:%M:%S %Y";
const std::string DateTime::SORTABLE_FORMAT = "%Y-%m-%d %H:%M:%S";

const std::string DateTime::WEEKDAY_NAMES[] =
{
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
};

const std::string DateTime::MONTH_NAMES[] =
{
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
};

DateTime::DateTime()
{
    Timestamp now;
    m_utcTime = now.utcTime();
    makeLocal(tzd());
}

DateTime::DateTime(const Timestamp& timestamp)
    : m_utcTime(timestamp.utcTime())
{
    makeLocal(tzd());
}

DateTime::DateTime(int year, int month, int day, int hour,
                   int minute, int second, int millisecond, int microsecond)
    : m_year(year)
    , m_month(month)
    , m_day(day)
    , m_hour(hour)
    , m_minute(minute)
    , m_second(second)
    , m_millisecond(millisecond)
    , m_microsecond(microsecond)
{
    FIRTEX_ASSERT2(year >= 0 && year <= 9999);
    FIRTEX_ASSERT2(month >= 1 && month <= 12);
    FIRTEX_ASSERT2(day >= 1 && day <= daysOfMonth(year, month));
    FIRTEX_ASSERT2(hour >= 0 && hour <= 23);
    FIRTEX_ASSERT2(minute >= 0 && minute <= 59);
    FIRTEX_ASSERT2(second >= 0 && second <= 59);
    FIRTEX_ASSERT2(millisecond >= 0 && millisecond <= 999);
    FIRTEX_ASSERT2(microsecond >= 0 && microsecond <= 999);

    m_utcTime = toUtcTime(toJulianDay(year, month, day)) + 
                10*(hour*Timespan::HOURS + minute*Timespan::MINUTES + 
                    second*Timespan::SECONDS + 
                    millisecond*Timespan::MILLISECONDS + microsecond);
}

DateTime::DateTime(Timestamp::UtcTimeVal utcTime, Timestamp::TimeDiff diff)
    : m_utcTime(utcTime + diff*10)
{
    computeGregorian(julianDay());
    computeDaytime();
}

DateTime::DateTime(const DateTime& dateTime)
    : m_utcTime(dateTime.m_utcTime)
    , m_year(dateTime.m_year)
    , m_month(dateTime.m_month)
    , m_day(dateTime.m_day)
    , m_hour(dateTime.m_hour)
    , m_minute(dateTime.m_minute)
    , m_second(dateTime.m_second)
    , m_millisecond(dateTime.m_millisecond)
    , m_microsecond(dateTime.m_microsecond)
{
}

DateTime::~DateTime()
{
}

DateTime& DateTime::operator = (const DateTime& dateTime)
{
    if (&dateTime != this)
    {
        m_utcTime     = dateTime.m_utcTime;
        m_year        = dateTime.m_year;
        m_month       = dateTime.m_month;
        m_day         = dateTime.m_day;
        m_hour        = dateTime.m_hour;
        m_minute      = dateTime.m_minute;
        m_second      = dateTime.m_second;
        m_millisecond = dateTime.m_millisecond;
        m_microsecond = dateTime.m_microsecond;
    }
    return *this;
}

DateTime& DateTime::operator = (const Timestamp& timestamp)
{
    m_utcTime = timestamp.utcTime();
    computeGregorian(julianDay());
    computeDaytime();
    return *this;
}

DateTime& DateTime::assign(int year, int month, int day, int hour,
                           int minute, int second, int millisecond,
                           int microsecond)
{
    FIRTEX_ASSERT2(year >= 0 && year <= 9999);
    FIRTEX_ASSERT2(month >= 1 && month <= 12);
    FIRTEX_ASSERT2(day >= 1 && day <= daysOfMonth(year, month));
    FIRTEX_ASSERT2(hour >= 0 && hour <= 23);
    FIRTEX_ASSERT2(minute >= 0 && minute <= 59);
    FIRTEX_ASSERT2(second >= 0 && second <= 59);
    FIRTEX_ASSERT2(millisecond >= 0 && millisecond <= 999);
    FIRTEX_ASSERT2(microsecond >= 0 && microsecond <= 999);

    m_utcTime     = toUtcTime(toJulianDay(year, month, day)) + 
                    10*(hour*Timespan::HOURS + minute*Timespan::MINUTES +
                            second*Timespan::SECONDS +
                            millisecond*Timespan::MILLISECONDS + microsecond);
    m_year        = year;
    m_month       = month;
    m_day         = day;
    m_hour        = hour;
    m_minute      = minute;
    m_second      = second;
    m_millisecond = millisecond;
    m_microsecond = microsecond;

    return *this;
}

void DateTime::swap(DateTime& dateTime)
{
    std::swap(m_utcTime, dateTime.m_utcTime);
    std::swap(m_year, dateTime.m_year);
    std::swap(m_month, dateTime.m_month);
    std::swap(m_day, dateTime.m_day);
    std::swap(m_hour, dateTime.m_hour);
    std::swap(m_minute, dateTime.m_minute);
    std::swap(m_second, dateTime.m_second);
    std::swap(m_millisecond, dateTime.m_millisecond);
    std::swap(m_microsecond, dateTime.m_microsecond);
}

int DateTime::dayOfWeek() const
{
    return int((std::floor(julianDay() + 1.5))) % 7;
}

int DateTime::daysOfMonth(int year, int month)
{
    FIRTEX_ASSERT2(month >= 1 && month <= 12);

    static int daysOfMonthTable[] = {0, 31, 28, 31, 30, 31, 
                                     30, 31, 31, 30, 31, 30, 31};

    if (month == 2 && isLeapYear(year))
        return 29;
    else
        return daysOfMonthTable[month];
}

bool DateTime::isValid(int year, int month, int day, int hour,
                       int minute, int second, int millisecond, int microsecond)
{
    return (year >= 0 && year <= 9999) &&
        (month >= 1 && month <= 12) &&
        (day >= 1 && day <= daysOfMonth(year, month)) &&
        (hour >= 0 && hour <= 23) &&
        (minute >= 0 && minute <= 59) &&
        (second >= 0 && second <= 59) &&
        (millisecond >= 0 && millisecond <= 999) &&
        (microsecond >= 0 && microsecond <= 999);
}

double DateTime::julianDay() const
{
    return toJulianDay(m_utcTime);
}

DateTime DateTime::operator + (const Timespan& span) const
{
    return DateTime(m_utcTime, span.totalMicroseconds());
}

DateTime DateTime::operator - (const Timespan& span) const
{
    return DateTime(m_utcTime, -span.totalMicroseconds());
}

Timespan DateTime::operator - (const DateTime& dateTime) const
{
    return Timespan((m_utcTime - dateTime.m_utcTime)/10);
}

DateTime& DateTime::operator += (const Timespan& span)
{
    m_utcTime += span.totalMicroseconds() * 10;
    computeGregorian(julianDay());
    computeDaytime();
    return *this;
}

DateTime& DateTime::operator -= (const Timespan& span)
{
    m_utcTime -= span.totalMicroseconds()*10;
    computeGregorian(julianDay());
    computeDaytime();
    return *this;
}

void DateTime::makeUTC(int tzd)
{
    operator -= (Timespan(((Timestamp::TimeDiff) tzd) * Timespan::SECONDS));
}

void DateTime::makeLocal(int tzd)
{
    operator += (Timespan(((Timestamp::TimeDiff) tzd) * Timespan::SECONDS));
}

double DateTime::toJulianDay(int year, int month, int day, int hour,
                             int minute, int second, int millisecond,
                             int microsecond)
{
    // lookup table for (153*month - 457)/5 - note that 3 <= month <= 14.
    static int lookup[] = {-91, -60, -30, 0, 31, 61, 92, 122,
                           153, 184, 214, 245, 275, 306, 337};
 
    // day to double
    double dday = double(day) + ((double((hour*60 + minute)*60 + second)*1000 +
                                  millisecond)*1000 + microsecond)/86400000000.0;
    if (month < 3)
    {
        month += 12;
        --year;
    }
    double dyear = double(year);
    return dday + lookup[month] + 365*year + std::floor(dyear/4) -
        std::floor(dyear/100) + std::floor(dyear/400) + 1721118.5;
}

void DateTime::checkLimit(short& lower, short& higher, short limit)
{
    if (lower >= limit)
    {
        higher += short(lower / limit);
        lower   = short(lower % limit);
    }
}

void DateTime::normalize()
{
    checkLimit(m_microsecond, m_millisecond, 1000);
    checkLimit(m_millisecond, m_second, 1000);
    checkLimit(m_second, m_minute, 60);
    checkLimit(m_minute, m_hour, 60);
    checkLimit(m_hour, m_day, 24);

    if (m_day > daysOfMonth(m_year, m_month))
    {
        m_day -= daysOfMonth(m_year, m_month);
        if (++m_month > 12)
        {
            ++m_year;
            m_month -= 12;
        }
    }
}

void DateTime::computeGregorian(double julianDay)
{
    double z    = std::floor(julianDay - 1721118.5);
    double r    = julianDay - 1721118.5 - z;
    double g    = z - 0.25;
    double a    = std::floor(g / 36524.25);
    double b    = a - std::floor(a/4);
    m_year       = short(std::floor((b + g)/365.25));
    double c    = b + z - std::floor(365.25*m_year);
    m_month      = short(std::floor((5*c + 456)/153));
    double dday = c - std::floor((153.0*m_month - 457)/5) + r;
    m_day        = short(dday);
    if (m_month > 12)
    {
        ++m_year;
        m_month -= 12;
    }
    r      *= 24;
    m_hour   = short(std::floor(r));
    r      -= std::floor(r);
    r      *= 60;
    m_minute = short(std::floor(r));
    r      -= std::floor(r);
    r      *= 60;
    m_second = short(std::floor(r));
    r      -= std::floor(r);
    r      *= 1000;
    m_millisecond = short(std::floor(r));
    r      -= std::floor(r);
    r      *= 1000;
    m_microsecond = short(r + 0.5);

    normalize();
}

void DateTime::computeDaytime()
{
    Timespan span(m_utcTime/10);
    m_hour        = (short)span.hours();
    m_minute      = (short)span.minutes();
    m_second      = (short)span.seconds();
    m_millisecond = (short)span.milliseconds();
    m_microsecond = (short)span.microseconds();
}

void DateTime::format(std::string& str, const std::string& fmt, int timeZoneDifferential)
{
    std::string::const_iterator it  = fmt.begin();
    std::string::const_iterator end = fmt.end();
    while (it != end)
    {
        if (*it == '%')
        {
            if (++it != end)
            {
                switch (*it)
                {
                case 'w': str.append(WEEKDAY_NAMES[dayOfWeek()], 0, 3); break;
                case 'W': str.append(WEEKDAY_NAMES[dayOfWeek()]); break;
                case 'b': str.append(MONTH_NAMES[month() - 1], 0, 3); break;
                case 'B': str.append(MONTH_NAMES[month() - 1]); break;
                case 'd': NumberFormatter::append0(str, (int32_t)day(), 2); break;
                case 'e': NumberFormatter::append(str, (int32_t)day()); break;
                case 'f': NumberFormatter::append(str, (int32_t)day(), 2); break;
                case 'm': NumberFormatter::append0(str, (int32_t)month(), 2); break;
                case 'n': NumberFormatter::append(str, (int32_t)month()); break;
                case 'o': NumberFormatter::append(str, (int32_t)month(), 2); break;
                case 'y': NumberFormatter::append0(str, (int32_t)year() % 100, 2); break;
                case 'Y': NumberFormatter::append0(str, (int32_t)year(), 4); break;
                case 'H': NumberFormatter::append0(str, (int32_t)hour(), 2); break;
                case 'h': NumberFormatter::append0(str, (int32_t)hourAMPM(), 2); break;
                case 'a': str.append(isAM() ? "am" : "pm"); break;
                case 'A': str.append(isAM() ? "AM" : "PM"); break;
                case 'M': NumberFormatter::append0(str, (int32_t)minute(), 2); break;
                case 'S': NumberFormatter::append0(str, (int32_t)second(), 2); break;
                case 'i': NumberFormatter::append0(str, (int32_t)millisecond(), 3); break;
                case 'c': NumberFormatter::append(str, (int32_t)millisecond()/100); break;
                case 'F': NumberFormatter::append0(str, (int32_t)millisecond()*1000 + microsecond(), 6); break;
                case 'z': tzdISO(str, timeZoneDifferential); break;
                case 'Z': tzdRFC(str, timeZoneDifferential); break;
                default:  str += *it;
                }
                ++it;
            }
        }
        else str += *it++;
    }
}

void DateTime::tzdISO(std::string& str, int timeZoneDifferential)
{
    if (timeZoneDifferential != UTC)
    {
        if (timeZoneDifferential >= 0)
        {
            str += '+';
            NumberFormatter::append0(str, (int32_t)timeZoneDifferential/3600, 2);
            str += ':';
            NumberFormatter::append0(str, (int32_t)(timeZoneDifferential%3600)/60, 2);
        }
        else
        {
            str += '-';
            NumberFormatter::append0(str, (int32_t)-timeZoneDifferential/3600, 2);
            str += ':';
            NumberFormatter::append0(str, (int32_t)(-timeZoneDifferential%3600)/60, 2);
        }
    }
    else str += 'Z';
}


void DateTime::tzdRFC(std::string& str, int timeZoneDifferential)
{
    if (timeZoneDifferential != UTC)
    {
        if (timeZoneDifferential >= 0)
        {
            str += '+';
            NumberFormatter::append0(str, (int32_t)timeZoneDifferential/3600, 2);
            NumberFormatter::append0(str, (int32_t)(timeZoneDifferential%3600)/60, 2);
        }
        else
        {
            str += '-';
            NumberFormatter::append0(str, (int32_t)-timeZoneDifferential/3600, 2);
            NumberFormatter::append0(str, (int32_t)(-timeZoneDifferential%3600)/60, 2);
        }
    }
    else str += "GMT";
}

int DateTime::tzd()
{
    return utcOffset() + dst();
}

int DateTime::utcOffset()
{
#ifndef FX_WINDOWS

#if defined(__APPLE__)  || defined(__FreeBSD__) // no timezone global var
    std::time_t now = std::time(NULL);
    struct std::tm t;
    gmtime_r(&now, &t);
    std::time_t utc = std::mktime(&t);
    return now - utc;
#elif defined(__CYGWIN__)
    return -_timezone;
#else
    return -timezone;
#endif

#else //FX_WINDOWS
    TIME_ZONE_INFORMATION tzInfo;
    DWORD dstFlag = GetTimeZoneInformation(&tzInfo);
    return -tzInfo.Bias*60;
#endif

}

int DateTime::dst()
{
#ifndef FX_WINDOWS
    std::time_t now = std::time(NULL);
    struct std::tm t;
    if (!localtime_r(&now, &t))
        FIRTEX_THROW(SystemException, "cannot get local time DST offset");
    return t.tm_isdst == 1 ? 3600 : 0;

#else //FX_WINDOWS
    TIME_ZONE_INFORMATION tzInfo;
    DWORD dstFlag = GetTimeZoneInformation(&tzInfo);
    return dstFlag == TIME_ZONE_ID_DAYLIGHT ? -tzInfo.DaylightBias*60 : 0;
#endif
}

FX_NS_END
