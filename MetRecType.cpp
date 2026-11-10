/**
 * @file MetRecType.cpp
 * @brief Implementation of MetRecType: one weather reading (date, time, wind speed, ambient air
 * temperature and solar radiation), and the operator that puts two readings in date-and-time order.
 * @author  Lim Wen Xin Sherine
 * @version 2.0
 * @date    09/11/2026
 */

#include "MetRecType.h"

/**
 * @brief Default constructor. Makes a reading with the default date and time and every measurement
 * marked as missing (MISSING_SPEED, MISSING_TEMP, MISSING_SOLAR).
 */

MetRecType::MetRecType()
{
    m_date;
    m_time;
    m_speed= MISSING_SPEED;
    m_temperature= MISSING_TEMP;
    m_solar= MISSING_SOLAR;
}

/**
 * @brief Makes a reading from a date, a time and a wind speed.
 *
 * The temperature and solar radiation are set to missing too, so a record built this way never
 * holds random values.
 *
 * @param date: the date of the reading
 * @param time: the time of the reading
 * @param speed: the wind speed (km/h)
 */
// temperature and solar are now set too, so manualOne record built this way never holds random values
MetRecType::MetRecType( const Date & date, const Time & time, float speed )
{
    m_date= date;
    m_time= time;
    m_speed= speed;
    m_temperature = MISSING_TEMP;
    m_solar= MISSING_SOLAR;

}

/** @brief Gives the date of the reading.
 *  @return the date
 */
Date MetRecType::GetDate() const
{
    return m_date;
}

/** @brief Gives the time of the reading.
 *  @return the time
 */
Time MetRecType::GetTime() const
{
    return m_time;
}

/** @brief Gives the wind speed.
*   @return the speed in km/h (a negative value means missing)
*/
float MetRecType::GetSpeed() const
{
    return m_speed;
}

/** @brief Gives the ambient air temperature.
 *  @return the temperature in degrees C (MISSING_TEMP or lower means missing)
 */
float MetRecType::GetTemperature() const
{
    return m_temperature;
}

/** @brief Gives the solar radiation.
  * @return the solar radiation in W/m2 (a negative value means missing)
  */
float MetRecType::GetSolarRadiation() const
{
    return m_solar;
}

/** @brief Changes the date of the reading.
 *  @param date: the new date
 */
void MetRecType::SetDate( const Date & date )
{
    m_date = date;
}

/** @brief Changes the time of the reading.
 *  @param time: the new time
 */
void MetRecType::SetTime( const Time & time )
{
    m_time = time;
}

/** @brief Changes the wind speed.
  * @param speed: the new speed in km/h
  */
void MetRecType::SetSpeed( float speed )
{
     m_speed = speed;
}

/** @brief Changes the ambient air temperature.
 *  @param temperature: the new temperature in degrees C
 */
void MetRecType::SetTemperature( float temperature )
{
    m_temperature = temperature;
}

/** @brief Changes the solar radiation.
*   @param solarRadiation: the new solar radiation in W/m2
*/
void MetRecType::SetSolarRadiation( float solarRadiation )
{
    m_solar = solarRadiation;
}

/** @brief Tells whether the wind speed is missing (N/A in the file).
 *  @return true if the speed is negative
 */
bool MetRecType::IsSpeedMissing() const
{
    return m_speed < 0;
}

/** @brief Tells whether the temperature is missing (N/A in the file).
 *  @return true if the temperature is MISSING_TEMP or lower
 */
bool MetRecType::IsTempMissing() const
{
    return m_temperature <= MISSING_TEMP;
}

/** @brief Tells whether the solar radiation is missing (N/A in the file).
  * @return true if the solar radiation is negative
  */
bool MetRecType::IsSolarMissing() const
{
    return m_solar < 0;
}

/**
 * @brief Tells whether one reading is earlier than another.
 *
 * The date checks first, then if the dates are the same, the time checks. This is the only comparison
 *  Bst uses for the sort, so two readings with the same date and time count as equal.
 *
 * @param left: the first reading
 * @param right: the second reading
 * @return true if left is earlier than right
 */
bool operator<( const MetRecType & left, const MetRecType & right )
{
    if (left.GetDate() < right.GetDate())
        return true;
    if (right.GetDate() < left.GetDate())
        return false;
    return left.GetTime() < right.GetTime();
}
