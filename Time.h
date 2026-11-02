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
    /** @brief Default Constructor */
    Time();

    /** @brief Parameterized Constructor that initializes all attributes
     * @param hour :Hour of the day
     * @param minute: Minute of the hour
     */
    Time( int hour, int minute );

    /** @brief Retrieves the hour
     *  @return Hour value stored in this Time
     */
    int GetHour() const;

     /** @brief Retrieves the minute
     *  @return Minute value stored in this Time
     */
    int GetMinute() const;

     /** @brief Sets the hour
     *  @param hour: the hour value to store
     */
    void SetHour( int hour );

     /** @brief Sets the minute
     *  @param minute:the minute value to store
     */
    void SetMinute( int minute );

    /** @brief checks for any duplicate of record with same time
     *  @param  other:the Time to compare with this one
     *  @return true if the hour and the minute both match, false otherwise
     */
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
