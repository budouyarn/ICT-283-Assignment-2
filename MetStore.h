#ifndef METSTORE_H
#define METSTORE_H

#include "Map.h"
#include "Bst.h"
#include "Metlog.h"
#include "MetRecType.h"

/**
 * The weather readings arranged for Assignment 2: a Map from each YEAR to a Bst holding that
 * year's readings, ordered by date and then time.
 *
 *   Map<int year, Bst<MetRecType>>
 *
 * Why this arrangement (the full reasoning is in the written justification):
 *  - A menu question about one month of one year goes straight to that year's tree (Map::Find)
 *    and never looks at the other years.
 *  - A question about one month across all years visits each year's tree in turn (Map::Keys).
 *  - A Bst keeps its values in order and refuses a second value with the same date and time, so a
 *    file loaded twice does not double the readings.
 *
 * The readings arrive in date order, which is the WORST case for a plain Bst (it becomes a chain).
 * So Load sorts them first and inserts them middle-first, which keeps every tree balanced.
 *
 * The Bst traversal calls a plain function for each reading and a function pointer cannot carry data,
 * so the figures are gathered by a small helper class with static members (see MetStore.cpp).
 *
 * @class MetStore
 * @author  Lim Wen Xin Sherine
 * @version 1.0
 * @date    01/11/2026
 */
class MetStore
{
    public:
        /** The result of Option 3: three correlations for one month, and how many pairs each used. */
        struct Correlations
        {
            double st;          /**< wind Speed and air Temperature (S_T), NaN if undefined */
            double sr;          /**< wind Speed and Solar radiation (S_R), NaN if undefined */
            double tr;          /**< air Temperature and Solar radiation (T_R), NaN if undefined */
            int stPairs;        /**< readings where both S and T are valid */
            int srPairs;        /**< readings where both S and SR are valid */
            int trPairs;        /**< readings where both T and SR are valid */
        };

        /** @brief Default constructor. Creates an empty store. */
        MetStore();

        /**
         * @brief Builds the Map of trees from every record in a Metlog, replacing anything held before.
         * Records are sorted by date and time, then each year's tree is filled middle-first so it stays balanced.
         * Two records with the same date and time count as the same reading; only the first is kept.
         * @param metLog: the loaded readings (read only)
         * @post GetYearCount() is the number of different years in metLog
         */
        void Load( const Metlog & metLog );

        /** @brief How many different years are held. */
        int GetYearCount() const;

        /** @brief How many different readings (distinct date and time) are held, over all years. */
        int GetReadingCount() const;

        /** @brief Number of valid wind speed readings in one month of one year. */
        int GetSpeedCount( int month, int year ) const;
        /** @brief Mean wind speed (km/h) in one month of one year, 0 if none. */
        double GetSpeedMean( int month, int year ) const;
        /** @brief Sample standard deviation of wind speed (km/h) in one month of one year, 0 if fewer than 2. */
        double GetSpeedStdDev( int month, int year ) const;

        /** @brief Number of valid air temperature readings in one month of one year. */
        int GetTemperatureCount( int month, int year ) const;
        /** @brief Mean air temperature (degrees C) in one month of one year, 0 if none. */
        double GetTemperatureMean( int month, int year ) const;
        /** @brief Sample standard deviation of air temperature in one month of one year, 0 if fewer than 2. */
        double GetTemperatureStdDev( int month, int year ) const;

        /**
         * @brief Sample Pearson correlation for S_T, S_R and T_R in one month, over every year loaded.
         * Only readings where BOTH values of a pair are valid are used, so the two lists stay matched.
         * @param month: 1 to 12
         * @return the three coefficients (NaN where undefined) and the pair counts
         */
        Correlations GetCorrelations( int month ) const;

    private:
        Map<int, Bst<MetRecType> > m_years;      /**< year -> that year's readings */
};

#endif // METSTORE_H
