#ifndef MENU_H
#define MENU_H

#include <string>
#include "Metlog.h"
#include "MetStore.h"
#include "Stats.h"

using std::string;

/**
 * @class Menu
 * @brief The weather data menu: asks the user what to see, then prints the answer.
 *
 * Menu does the keyboard and screen work only. It never calculates anything itself:
 * it asks MetStore (or Metlog) through their public methods and prints what comes back.
 *
 * Options:
 *  Option 1: average wind speed and standard deviation for a month and year
 *  Option 2: average air temperature and standard deviation for a month and year
 *  Option 3: sample Pearson correlation coefficients (S_T, S_R, T_R) for a month, over all years
 *  Option 4: for each month of a year, wind speed and temperature with (stdev, mad), and total solar radiation, written to WindTempSolar.csv
 *  Option 0: exit
 *
 * @author  Lim Wen Xin Sherine
 * @version 2.0
 * @date 28/10/2026
 */
class Menu
{
    public:
        /**
         * @brief Runs the menu until the user chooses Exit or the input ends.
         * @param metLog the loaded readings, for the whole-file report (read only)
         * @param store the readings arranged by year and month, for options 1 to 4 (read only)
         */
        void Run( const Metlog & metLog, const MetStore & store ) const;

    private:
        /**
         * @brief Asks until the user types a whole number from low(1) to high(4).
         *
         * A whole line is read and then parsed, so letters or half-typed input such as "3abc"
         * cannot leave the keyboard stream in a failed state.
         *
         * @param prompt the question to show
         * @param low: the smallest number accepted in rsnge 1-4
         * @param high: the largest number accepted in rsnge 1-4
         * @param value: set to the number typed (only changed when a valid number is read)
         * @return true if a valid number was read, false only if the input has ended
         */
        static bool ReadInt( const string & prompt, int low, int high, int & value );

        /**
         * @brief Asks for a month (1 to 12) and then a year.
         * @param month: set to the month typed
         * @param year: set to the year typed (only asked if the month was read)
         * @return true if both were read, false if the input ended
         */
        static bool AskMonthYear( int & month, int & year );

        /** @brief Prints the list of menu options. */
        static void PrintMenu();

        /**
         * @brief Option 1: prints the number of valid readings, the average wind speed and the standard
         * deviation for one month and year.
         * @param store: the readings to ask
         * @param month: the month, 1 to 12
         * @param year: the year
         */
        void ShowWind( const MetStore & store, int month, int year ) const;

        /**
         * @brief Option 2: prints the number of valid readings, the average air temperature and the
         * standard deviation for one month and year.
         * @param store: the readings to ask
         * @param month: the month, 1 to 12
         * @param year: the year
         */
        void ShowTemperature( const MetStore & store, int month, int year ) const;

        /**
         * @brief Option 3: prints the sample Pearson correlation coefficients S_T, S_R and T_R for one
         * month, using the readings of every year that was loaded. A coefficient that cannot be
         * calculated is shown as "not available".
         * @param store: the readings to ask
         * @param month: the month, 1 to 12
         */
        void ShowCorrelations( const MetStore & store, int month ) const;

        /**
         * @brief Option 4: writes WindTempSolar.csv with the year on the first line, then one line for each
         * month: wind speed and temperature, each as mean(stdev, mad), and the total solar radiation.
         * A month with no valid readings of a kind shows N/A.
         * @param store: the readings to ask
         * @param year: the year to report
         */
        void ShowMonthlyTable( const MetStore & store, int year ) const;

};

#endif // MENU_H
