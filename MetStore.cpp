#ifndef METRECTYPE_H
#define METRECTYPE_H
/**
 * contains  meteorological record:
 * -the date and time a reading was taken,
 * -wind speed (km/h)
 * - ambient air temperature (degrees C) and
 * -solar radiation (W/m2) recorded at that moment.
 * Any of the three
 * measurements can be flagged as missing when the sensor gave no value.
 *
 * @class MetRecType
 * @author  Lim Wen Xin Sherine
 * @version 1.0
 * @date 05/10/2026
 *
 *
 *
 *
 *
 */
#include "Date.h"
#include "Time.h"

const float MISSING_SPEED = -1.0f;
//mark a missing/offline-sensor reading:
// acts as the marker for "sensor offline".
//A blank or N/A speed is stored as -1 rather than 0, because 0 km/h is a real reading (calm air) and would be counted wrongly in the mean.
const float MISSING_TEMP  = -999.0f; //Temperature can  be -1 °C, so -1 value could become a  real cold reading  and not a missed value
const float MISSING_SOLAR = -1.0f;// same as MISSING_SPEED

class MetRecType
{
    public:
        /**
         * @brief Default constructor. Required because Vector<T>'s own
         * default constructor does `new T[m_capacity]`, which needs T
         * to be default-constructed.
         */
        MetRecType();

        /**
         * @brief Builds a complete record with the required groups.
         * @param date:the date of this reading
         * @param time:the time of this reading
         * @param speed: the wind speed in km/h, or MISSING_SPEED if the
         * sensor was offline for this reading.
         */
        MetRecType( const Date & date, const Time & time, float speed );

        /** @brief Retrieves the date of this reading
         *  @return the Date stored in this record
         */
        Date GetDate() const;

        /** @brief Retrieves the time of this reading
         *  @return the Time stored in this record
         */
        Time GetTime() const;

        /** @brief Retrieves the wind speed of this reading
         *  @return the speed in km/h, or MISSING_SPEED if the sensor
         *  was offline for this reading
         */
        float GetSpeed() const;

         /** @brief Retrieves the ambient air temperature of this reading
         *  @return the temperature in degrees C, returns MISSING_TEMP if the
         *   sensor was offline.
         */
        float GetTemperature() const;

         /** @brief Retrieves the solar radiation of this reading
         *  @return the solar radiation in W/m2,returns MISSING_SOLAR if the
         *   sensor was offline.
         */
        float GetSolarRadiation() const;

        /** @brief Sets the date of this reading
         *  @param date the new date
         */
        void SetDate( const Date & date );

        /** @brief Sets the time of this reading
         *  @param time the new time
         */
        void SetTime( const Time & time );

        /** @brief Sets the wind speed of this reading
         *  @param speed the speed in km/h, or MISSING_SPEED if the sensor was offline
         */
        void SetSpeed( float speed );

        /** @brief Sets the ambient air temperature of this reading
         *  @param temperature the temperature in degrees C, or MISSING_TEMP
         *   if the sensor was offline
         */
        void SetTemperature( float temperature );

        /** @brief Sets the solar radiation of this reading
         *  @param solarRadiation the solar radiation in W/m2, or MISSING_SOLAR
         *   if the sensor was offline
         */
        void SetSolarRadiation( float solarRadiation );

        /** @brief Reports whether this reading's sensor was offline
         *  @return true, if GetSpeed() is a tolerance value
         */
        // behaviour method to find anomalities if speed is missing
        bool IsSpeedMissing() const;

         /** @brief Reports whether this reading's temperature is missing
         *  @return true, if GetTemperature() is a tolerance value
         */
        bool IsTempMissing() const;

         /** @brief Reports whether this reading's solar radiation is missing
         *  @return true, if GetSolarRadiation() is a tolerance value
         */
        bool IsSolarMissing() const;


    private:
        Date m_date;// composition from Date Class
        Time m_time; //composition from Time Class
        float m_speed;
        float m_temperature;
        float m_solar;

};

/** @brief Orders two records by date, then by time, so records can be kept in a binary search tree.
 *  Two records with the same date and time count as the same reading (neither is smaller).
 *  @return true if left was taken before right */
bool operator<( const MetRecType & left, const MetRecType & right );

#endif
