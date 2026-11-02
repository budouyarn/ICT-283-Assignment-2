#include "MetRecType.h"

MetRecType::MetRecType() : m_date(), m_time(), m_speed( MISSING_SPEED ), m_temperature( MISSING_TEMP ), m_solar( MISSING_SOLAR )
{
}

// FIX: temperature and solar are now set too, so a record built this way never holds random values
MetRecType::MetRecType( const Date & date, const Time & time, float speed )
    : m_date( date ), m_time( time ), m_speed( speed ), m_temperature( MISSING_TEMP ), m_solar( MISSING_SOLAR )
{
}

Date MetRecType::GetDate() const { return m_date; }
Time MetRecType::GetTime() const { return m_time; }
float MetRecType::GetSpeed() const { return m_speed; }
float MetRecType::GetTemperature() const { return m_temperature; }
float MetRecType::GetSolarRadiation() const { return m_solar; }
void MetRecType::SetDate( const Date & date ) { m_date = date; }
void MetRecType::SetTime( const Time & time ) { m_time = time; }
void MetRecType::SetSpeed( float speed ) { m_speed = speed; }
void MetRecType::SetTemperature( float temperature ) { m_temperature = temperature; }
void MetRecType::SetSolarRadiation( float solarRadiation ) { m_solar = solarRadiation; }

bool MetRecType::IsSpeedMissing() const
{
    return m_speed < 0;                    // FIX: a real wind speed is never negative (no float == needed)
}

bool MetRecType::IsTempMissing() const
{
    return m_temperature <= MISSING_TEMP;
}

bool MetRecType::IsSolarMissing() const
{
    return m_solar < 0;
}

bool operator<( const MetRecType & left, const MetRecType & right )
{
    if (left.GetDate() < right.GetDate())
        return true;
    if (right.GetDate() < left.GetDate())
        return false;
    return left.GetTime() < right.GetTime();    // same date: the time decides
}
