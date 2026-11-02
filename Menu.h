#ifndef MENU_H
#define MENU_H

#include <string>
#include "Metlog.h"
#include "MetStore.h"
#include "Stats.h"          // IsDefined

using std::string;

/**
 * The weather data menu. Options 1 and 2 and the new Option 3 read from the MetStore
 * (the Map of year trees); Option 4 reports on the whole Metlog.
 *
 * @class Menu
 * @author  Lim Wen Xin Sherine
 * @version 2.0
 * @date    01/11/2026
 */
class Menu
{
    public:
        /** @brief Runs the menu until the user chooses Exit or the input ends.
         *  @param metLog the loaded readings, for the whole-file report (read only)
         *  @param store the readings arranged by year, for options 1 to 3 (read only) */
        void Run( const Metlog & metLog, const MetStore & store ) const;

    private:
        static bool ReadInt( const string & prompt, int low, int high, int & value );
        static bool AskMonthYear( int & month, int & year );
        static void PrintMenu();

        void ShowWind( const MetStore & store, int month, int year ) const;
        void ShowTemperature( const MetStore & store, int month, int year ) const;
        void ShowCorrelations( const MetStore & store, int month ) const;
        void ShowWholeFile( const Metlog & metLog ) const;
};

#endif // MENU_H
