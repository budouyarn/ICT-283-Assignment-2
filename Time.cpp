#include "Time.h"
#include <string>
using std::string;

Time::Time()
{
    m_hour = 0;
    m_minute = 0;
}

Time::Time( int hour, int minute )
{
    m_hour = hour;
    m_minute = minute;
}
int Time::GetHour() const { return m_hour; }
int Time::GetMinute() const { return m_minute; }
void Time::SetHour(int hour) { m_hour = hour; }
void Time::SetMinute(int minute) { m_minute = minute; }

bool Time::Equals( const Time & other ) const
{
    return m_hour == other.m_hour && m_minute == other.m_minute;
}

ostream & operator<<( ostream & os, const Time & time )
{
    int hour = time.GetHour();
    int minute = time.GetMinute();

    if (hour < 10) os << '0' << hour;
        else os << hour;

    os << ':';
    if (minute < 10) os << '0' << minute;
        else os << minute;

    return os;
}

istream & operator>>( istream & input, Time & time )
{
    string tempStr;
    input >> tempStr;

    char colon = tempStr.find(':');
    string hourStr = tempStr.substr(0, colon);
    string minuteStr = tempStr.substr(colon + 1);

    time.SetHour(stoi(hourStr));
    time.SetMinute(stoi(minuteStr));

    return input;
}

bool operator<( const Time & left, const Time & right )
{
    if (left.GetHour() != right.GetHour())          // the hour decides first
        return left.GetHour() < right.GetHour();
    return left.GetMinute() < right.GetMinute();    // then the minute
}
