#ifndef METLOG_H
#define METLOG_H
/**
 *  Acts as the Encapsulated class of the Vector where it holds MetRecType records, reads them
 * from a CSV file, writes them back out for checking, and reports the mean
 * and sample standard deviation of the speed readings.
 *
 * @class Metlog
 * @author Lim Wen Xin Sherine
 * @version 2.0
 * @date 06/10/2026
 *
 */
#include <iostream> // iostream STL that includes almost everything, while able to use std::istream
#include <string>
#include "Vector.h" // uses the Vector.h as the template class
#include "MetRecType.h"

using std::string;
using std::istream;

class Metlog
{
    public:
        /** @brief Default constructor. Starts with an empty log. */
        Metlog();

        /**
         * @brief Writes every record as "date,time,speed", one per line, so
         * the output can be checked against the original file. A
         * missing speed is written as N/A.
         * @param filename: path of the file to write
         * @return false if the file cannot be created
         */
        bool WriteTestOutput( const string & filename ) const;

        /** @brief Number of records held (including ones with a missing speed)
         *  @return the record count
         */
        int GetRecordCount() const;

        /** @brief Number of data rows operator>> skipped because they were malformed
         *  @return the skipped-row count
         */
        int GetSkipCount() const;

        /** @brief Number of records with a real (non-missing) speed
         *  @return the valid-reading count
         */
        int GetValidCount() const;

         /**
         * @brief Gives read-only access to one stored record, so the caller can
         * look at its date, time and speed
         * @param index: position of the record, from 0 to GetRecordCount() - 1
         * @return the record in that position
         */
        MetRecType GetRecord( int index ) const; //Composition from MetRecType as it holds the records

        /**
         * @brief Mean wind speed in km/h over the valid readings.
         * @return the mean speed
         */
        double GetMean() const;

        /**
         * @brief  standard deviation (n - 1) of wind speed in km/h
         *
         * @return the standard deviation
         */
        double GetStdDev() const;

        /** @brief Empties the log, removing every record and resets the bad-row count */
        void Clear(); // clear garbage

        /** @brief Appends one record to the log
         *  @param record: the record to add
         */
        void AddRecord( const MetRecType & record );

        /** @brief Adds to count when it sees an abnormal row that was skipped while reading */
        void AddBadRow();

        void Merge( const Metlog & other );

        /**
         * @brief Number of wind speed readings in one month of one year that
         * are not missing.
         * @param month: integers 1 to 12
         * @param year: the four digit year
         * @return the reading count, return 0 if the month has no valid readings
         */
        int GetSpeedCount( int month, int year ) const;

        /**
         * @brief Mean wind speed in km/h for one month of one year, over the
         * valid readings only. Call only when GetSpeedCount() is at least 1.
         * @param month: Ranged 1 to 12
         * @param year: the four digit year
         * @return the mean speed, or 0 if there are no valid readings
         */
        double GetSpeedMean( int month, int year ) const;

        /**
         * @brief Sample standard deviation (n - 1) of wind speed in km/h for
         * one month of one year. Call only when GetSpeedCount() is at least 2.
         * @param month: Ranged 1 to 12
         * @param year: the four digit year
         * @return the sample standard deviation, or 0 if fewer than 2 valid readings
         */
        double GetSpeedStdDev( int month, int year ) const;

        /**
         * @brief Number of ambient air temperature readings in one month of
         * one year that are real (not missing).
         * @param month: 1 to 12
         * @param year: the four digit year
         * @return the reading count, return 0 if the month has no valid readings
         */
        int GetTemperatureCount( int month, int year ) const;

        /**
         * @brief Mean ambient air temperature in degrees C for one month of
         * one year, over the valid readings only. Call only when
         * GetTemperatureCount() is at least 1.
         * @param month 1 to 12
         * @param year  the four digit year
         * @return the mean temperature, or 0 if there are no valid readings
         */
        double GetTemperatureMean( int month, int year ) const;

        /**
         * @brief  standard deviation (n - 1) of ambient air temperature
         * in degrees C for one month of one year. Call only when
         * GetTemperatureCount() is at least 2.
         * @param month: 1 to 12
         * @param year:  the four digit year
         * @return the standard deviation, or 0 if fewer than 2 valid readings
         */
        double GetTemperatureStdDev( int month, int year ) const;

        /**
         * @brief Number of solar radiation readings in one month of one year
         * that are real values.
         * @param month: 1 to 12
         * @param year:the four digit year
         * @return the reading count, return 0 if the month has no valid readings
         */
        int GetSolarCount( int month, int year ) const;

        /**
         * @brief Total solar radiation for one month of one year in kWh/m2.
         * Each reading logs the average power (W/m2) over a 10 minute interval,
         * so the energy of one reading is W/m2 x (10/60) hours, and dividing
         * by 1000 gives kWh/m2. Missing readings are left out.
         * @param month: 1 to 12
         * @param year:the four digit year
         * @return the total in kWh/m2, return 0 if there are no valid readings
         */
        double GetSolarTotal( int month, int year ) const;
        /**
    * @brief Finds the readings whose speed "matches" the mean speed. Two
    * floating point values are almost never exactly equal, so a reading
    * matches when it is within tolerance of the mean.
    * Readings with a missing speed never matches.
    * @param tolerance: the largest difference (km/h) still counted as a match
    * @return the matching records, in file order.
    */
    Vector<MetRecType> FindMatchesToMean( double tolerance ) const;

    private:

        Vector<MetRecType> m_records;
        int m_badRowCount;

        /**
         * @brief True return, if the record was taken in the given month and year.
         * Used by the three Collect functions so the date test is written once.
         * @param record: the record to test
         * @param month: 1 to 12
         * @param year: the four digit year, or 0 for every year
         */
        static bool IsInPeriod( const MetRecType & record, int month, int year );

        /**
         * @brief Gathers the valid wind speeds (km/h) of one
         * month of one year, in file order.
         * @param month: 1 to 12
         * @param year: the four digit year, or 0 for every year
         */
        Vector<double> CollectSpeeds( int month, int year ) const;

        /**
         * @brief Gathers the real (non-missing) temperatures (degrees C) of
         * one month of one year, in file order.
         * @param month 1 to 12, or 0 for every month
         * @param year  the four digit year, or 0 for every year
         */
        Vector<double> CollectTemperatures( int month, int year ) const;

        /**
         * @brief Gathers the valid solar radiation values (W/m2)
         * of one month of one year, in file order.
         * @param month: 1 to 12
         * @param year :the four digit year, or 0 for every year
         */
        Vector<double> CollectSolar( int month, int year ) const;

        /** @brief Mean of the values (0 if there are none) */
        static double MeanOf( const Vector<double> & values );

        /** @brief Sample standard deviation, n - 1 (0 if fewer than 2 values) */
        static double StdDevOf( const Vector<double> & values );

        /** @brief Does the actual mean calculation for GetMean() */
        double CalculateMean() const;

        /** @brief Does the actual standard deviation calculation for GetStdDev() */
        double CalculateStdDev() const;

};

/**
 * @brief Reads a whole CSV file from the stream into the log, replacing any
 * records already held.
 *
 * Speeds are converted from m/s to km/h. A speed that can'time be read
 * is stored as MISSING_SPEED
 *
 * rows that can'time be parsed
 * are skipped and counted.
 * @param input the open file stream
 * @param windLog the log to fill
 * @return the stream
 * @throws std::runtime_error if the header line is missing any of the 18
 *
 */
istream & operator>>( istream & input, Metlog & metlog );

#endif
