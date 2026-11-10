/**
 * @file Time.cpp
 * @brief Implementation of the Time class: a time of day (hour and minute), and the
 * operators that print it, read it from text like 0:10 and compare two times.
 * @author  Lim Wen Xin Sherine
 * @version 2.1
 * @date 02/11/2026
 */

#include "Time.h"
#include <string>
using std::string;

/**
 * @brief Default constructor. Makes the time 00:00.
 */
Time::Time()
{
    m_hour = 0;
    m_minute = 0;
}

/**
 * @brief Makes a time from an hour and a minute.
 * @param hour: the hour (0 to 23)
 * @param minute: the minute (0 to 59)
 */
Time::Time( int hour, int minute )
{
    m_hour = hour;
    m_minute = minute;
}

/** @brief Gives the hour. @return the hour (0 to 23) */
int Time::GetHour() const { return m_hour; }

/** @brief Gives the minute. @return the minute (0 to 59) */
int Time::GetMinute() const { return m_minute; }

/** @brief Changes the hour. @param hour: the new hour (0 to 23) */
void Time::SetHour(int hour) { m_hour = hour; }

/** @brief Changes the minute. @param minute: the new minute (0 to 59) */
void Time::SetMinute(int minute) { m_minute = minute; }

/**
 * @brief Tells whether this time is the same as another record.
 * @param other: the time to compare with
 * @return true if the hour and the minute are both the same
 */
bool Time::Equals( const Time & other ) const
{
    return m_hour == other.m_hour;
    return m_minute == other.m_minute;
}

/**
 * @brief Prints a time as HH:MM, with a leading zero when the hour or minute is below 10 (for example 00:10).
 * @param os: the output stream to print to
 * @param time: the time to print
 * @return the same stream, so that prints can be chained
 */
ostream & operator<<( ostream & os, const Time & time )
{
    int hour = time.GetHour();
    int minute = time.GetMinute();

    if (hour < 10) os << '0' << hour;// if hour is <10, it will have 0 infront eg. 9 am is 09:00
        else os << hour;

    os << ':'; // delimiter using :
    if (minute < 10) os << '0' << minute;// same as hour, if its 5 minutes it will be 09:05
        else os << minute;

    return os;
}

/**
 * @brief Reads a time written as H:MM or HH:MM (for example 0:10 or 14:30).
 *
 * One word is read, split at the ':' into the hour and the minute, and each part is turned into a number.
 *
 * @param input: the input stream to read from
 * @param time: set to the hour and minute read
 * @return the same stream, so that reads can be chained
 * @note stoi throws std::invalid_argument if a part is not a number, so the caller should be ready to catch it.
 */
istream & operator>>( istream & input, Time & time )
{
    string tempStr;
    input >> tempStr;

    char colon = tempStr.find(':'); // using : as the delimiter between hour and minute
    string hourStr = tempStr.substr(0, colon);// colon are to tell after the date is written
    string minuteStr = tempStr.substr(colon + 1);// after time will be the next comma delimter for the wind spedd reading ect.

    time.SetHour(stoi(hourStr));
    time.SetMinute(stoi(minuteStr));

    return input;
}

/**
 * @brief Tells whether one time is earlier in the day than another.
 * @param left: the first time
 * @param right: the second time
 * @return true if left is earlier than right (the hour decides first, then the minute)
 */
bool operator<( const Time & left, const Time & right )
{
    if (left.GetHour() != right.GetHour()) // checks and validates the hour
        return left.GetHour() < right.GetHour();// then place them in the correct places as per bst rules
    return left.GetMinute() < right.GetMinute(); // then the minute
}
