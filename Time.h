#ifndef TIME_H
#define TIME_H
/**
 * Application to represent a time of day (hour:minute)
 * @class Time
 * @author  Lim Wen Xin Sherine
 * @version 1.1
 * @date 02/10/2026
 */

#include <iostream>

using std::istream;
using std::ostream;

class Time
{
public:
    Time();
    Time( int hour, int minute );
    int GetHour() const;
    int GetMinute() const;
    void SetHour( int hour );
    void SetMinute( int minute );
    bool Equals( const Time & other ) const;

private:
    int m_hour;//stores hour value
    int m_minute;// store minute value
};

ostream & operator<<( ostream & os, const Time & time );
istream & operator>>( istream & input, Time & time );

/** @brief Compares two times so a record can be placed in a binary search tree.
 *  The hour is compared first, then the minute.
 *  @return true if left is earlier than right (false when they are equal) */
bool operator<( const Time & left, const Time & right );

#endif
