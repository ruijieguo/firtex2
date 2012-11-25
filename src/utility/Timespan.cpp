#include "firtex/utility/Timespan.h"
#include <algorithm>

FX_NS_DEF(utility);

const Timespan::TimeDiff Timespan::MILLISECONDS = 1000;
const Timespan::TimeDiff Timespan::SECONDS      = 1000 * Timespan::MILLISECONDS;
const Timespan::TimeDiff Timespan::MINUTES      = 60 * Timespan::SECONDS;
const Timespan::TimeDiff Timespan::HOURS        = 60 * Timespan::MINUTES;
const Timespan::TimeDiff Timespan::DAYS         = 24 * Timespan::HOURS;

Timespan::Timespan()
    : m_nSpan(0)
{
}

Timespan::Timespan(TimeDiff microseconds)
    : m_nSpan(microseconds)
{
}

Timespan::Timespan(long seconds, long microseconds)
    : m_nSpan(TimeDiff(seconds)*SECONDS + microseconds)
{
}

Timespan::Timespan(int32_t days, int32_t hours, int32_t minutes,
                   int32_t seconds, int32_t microseconds)
    : m_nSpan(TimeDiff(microseconds) + TimeDiff(seconds)*SECONDS + 
              TimeDiff(minutes)*MINUTES + TimeDiff(hours)*HOURS +
              TimeDiff(days)*DAYS)
{
}

Timespan::Timespan(const Timespan& timespan)
    : m_nSpan(timespan.m_nSpan)
{
}

Timespan::~Timespan()
{
}

Timespan& Timespan::operator = (const Timespan& timespan)
{
    m_nSpan = timespan.m_nSpan;
    return *this;
}

Timespan& Timespan::operator = (TimeDiff microseconds)
{
    m_nSpan = microseconds;
    return *this;
}

Timespan& Timespan::assign(int32_t days, int32_t hours, int32_t minutes, 
                           int32_t seconds, int32_t microseconds)
{
    m_nSpan = TimeDiff(microseconds) + TimeDiff(seconds)*SECONDS + 
              TimeDiff(minutes)*MINUTES + TimeDiff(hours)*HOURS + TimeDiff(days)*DAYS;
    return *this;
}

Timespan& Timespan::assign(long seconds, long microseconds)
{
    m_nSpan = TimeDiff(seconds)*SECONDS + TimeDiff(microseconds);
    return *this;
}

Timespan Timespan::operator + (const Timespan& d) const
{
    return Timespan(m_nSpan + d.m_nSpan);
}

Timespan Timespan::operator - (const Timespan& d) const
{
    return Timespan(m_nSpan - d.m_nSpan);
}

Timespan& Timespan::operator += (const Timespan& d)
{
    m_nSpan += d.m_nSpan;
    return *this;
}

Timespan& Timespan::operator -= (const Timespan& d)
{
    m_nSpan -= d.m_nSpan;
    return *this;
}

Timespan Timespan::operator + (TimeDiff microseconds) const
{
    return Timespan(m_nSpan + microseconds);
}

Timespan Timespan::operator - (TimeDiff microseconds) const
{
    return Timespan(m_nSpan - microseconds);
}

Timespan& Timespan::operator += (TimeDiff microseconds)
{
    m_nSpan += microseconds;
    return *this;
}

Timespan& Timespan::operator -= (TimeDiff microseconds)
{
    m_nSpan -= microseconds;
    return *this;
}

FX_NS_END
