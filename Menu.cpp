/**
 * @brief Implementation of the Menu class: the weather data menu and the
 * functions that ask the user for input and print the results.
 * @author  Lim Wen Xin Sherine
 * @version 2.0
 * @date 01/11/2026
 */

#include "Menu.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::fixed;
using std::getline;
using std::istringstream;
using std::setprecision;
using std::string;

/** @brief How close (km/h) a wind speed must be to the mean to "match" it (half of the last printed digit). */
const double MATCH_TOLERANCE = 0.005;

/** @brief The names of the months, so MONTH_NAMES[month - 1] gives the name of month 1-12. */
const string MONTH_NAMES[12] ={"January", "February", "March", "April", "May", "June","July", "August", "September", "October", "November", "December"};

// Menu choices
const int MENU_WIND = 1;
const int MENU_TEMPERATURE = 2;
const int MENU_CORRELATION = 3;     // replaces the old solar radiation total
const int MENU_WHOLE_FILE = 4;
const int MENU_EXIT = 0;

void Menu::Run( const Metlog & metLog, const MetStore & store ) const
{
    // setprecision (2) is the same way of printing as %.2f in C
    cout << fixed << setprecision(2);

    int choice = MENU_EXIT;
    do
    {
        PrintMenu();
        if (!ReadInt( "Enter your choice: ", 0, 4, choice ))
            break;// input ended

        if (choice == MENU_WHOLE_FILE)
            ShowWholeFile( metLog );
        else if (choice == MENU_CORRELATION)
        {
            int month = 0;
            if (!ReadInt( "Enter the month (1-12): ", 1, 12, month ))   // Option 3 asks for the month only
                break;
            ShowCorrelations( store, month );
        }
        else if (choice != MENU_EXIT)
        {
            int month = 0, year = 0;
            if (!AskMonthYear( month, year ))
                break;

            if (choice == MENU_WIND)
                ShowWind( store, month, year );
            else if (choice == MENU_TEMPERATURE)
                ShowTemperature( store, month, year );
        }
    } while (choice != MENU_EXIT);// loop again until the user chooses Exit
}

// Asks until the user types a whole number from low to high. Reading a whole
// line and parsing it with an istringstream means letters or half-typed input
// can't leave the keyboard stream in a failed state. Returns false only if the
// input has ended, stop instead of looping.
bool Menu::ReadInt( const string & prompt, int low, int high, int & value )
{
    string line;
    while (true)// keeps asking until a valid number is typed
    {
        cout << prompt;
        if (!getline( cin, line ))
            return false;

        istringstream lineStream( line );
        int number = 0;
        char extra; // catches anything typed after the number, eg. "3abc"
        if (lineStream >> number && !(lineStream >> extra) &&
            number >= low && number <= high)
        {
            value = number;
            return true;
        }
        cout << "Please enter a whole number from " << low << " to " << high << "." << endl;
    }
}

bool Menu::AskMonthYear( int & month, int & year )
{// && stops at the first false, so the year is only asked if the month was read
    return ReadInt( "Enter the month (1-12): ", 1, 12, month ) &&
           ReadInt( "Enter the year (e.g. 2014): ", 1, 9999, year );
}

void Menu::PrintMenu()
{
    cout << endl
         << "========== Weather data menu ==========" << endl
         << MENU_WIND        << ". Average wind speed and standard deviation for a month and year" << endl
         << MENU_TEMPERATURE << ". Average air temperature and standard deviation for a month and year" << endl
         << MENU_CORRELATION << ". Sample Pearson correlation (S_T, S_R, T_R) for a month, over all years" << endl
         << MENU_WHOLE_FILE  << ". Wind speed summary of the whole file, and matches to the average" << endl
         << MENU_EXIT << ". Exit" << endl;
}

void Menu::ShowWind( const MetStore & store, int month, int year ) const
{
    int count = store.GetSpeedCount( month, year );
    cout << endl << "Wind speed, " << MONTH_NAMES[month - 1] << " " << year << endl;
    if (count == 0)
    {
        cout << "No valid wind speed readings for that month." << endl;
        return;
    }
    cout << "Valid readings (N): " << count << endl;
    cout << "Average wind speed (km/h): " << store.GetSpeedMean( month, year ) << endl;

    if (count < 2) // a sample standard deviation needs at least 2 readings
        cout << "Standard deviation: not available (needs at least 2 readings)" << endl;
    else
        cout << "Sample standard deviation (km/h): " << store.GetSpeedStdDev( month, year ) << endl;
}

void Menu::ShowTemperature( const MetStore & store, int month, int year ) const
{
    int count = store.GetTemperatureCount( month, year );// how many valid temperatures that month
    cout << endl << "Ambient air temperature, " << MONTH_NAMES[month - 1] << " " << year << endl;
    if (count == 0)
    {
        cout << "No valid temperature readings for that month." << endl;
        return;
    }
    cout << "Valid readings (N): " << count << endl;
    cout << "Average temperature (degrees C): " << store.GetTemperatureMean( month, year ) << endl;

    if (count < 2)
        cout << "Standard deviation: not available (needs at least 2 readings)" << endl;
    else
        cout << "Sample standard deviation (degrees C): " << store.GetTemperatureStdDev( month, year ) << endl;
}

/**
 * @brief Option 3: prints the sample Pearson correlation coefficient for S_T, S_R and T_R
 * for one month, using the readings of every year that was loaded.
 */
void Menu::ShowCorrelations( const MetStore & store, int month ) const
{
    MetStore::Correlations result = store.GetCorrelations( month );

    cout << endl << "Sample Pearson Correlation Coefficient for " << MONTH_NAMES[month - 1] << endl;

    if (IsDefined( result.SpeedTemp ))
        cout << "S_T: " << result.SpeedTemp << endl;
    else
        cout << "S_T: not available (needs at least 2 matching readings that vary)" << endl;

    if (IsDefined( result.SpeedRad ))
        cout << "S_R: " << result.SpeedRad << endl;
    else
        cout << "S_R: not available (needs at least 2 matching readings that vary)" << endl;

    if (IsDefined( result.TempRad ))
        cout << "T_R: " << result.TempRad << endl;
    else
        cout << "T_R: not available (needs at least 2 matching readings that vary)" << endl;
}

// The Lab 5 report: whole-file wind speed figures and the matching readings
void Menu::ShowWholeFile( const Metlog & metLog ) const
{
    // List every reading whose speed was missing (N/A), by its date and time
    int missingCount = metLog.GetRecordCount() - metLog.GetValidCount();
    cout << endl << "Readings with a missing wind speed: " << missingCount << endl;
    for (int counter = 0; counter < metLog.GetRecordCount(); counter++)
    {
        MetRecType record = metLog.GetRecord(counter);
        if (record.IsSpeedMissing())
            cout << "  Missing: " << record.GetDate() << " " << record.GetTime() << endl;
    }
    cout << "Rows skipped as malformed: " << metLog.GetSkipCount() << endl;

    cout << "The number of valid wind speed readings(N): " << metLog.GetValidCount() << endl;
    cout << "The mean wind speed (km/h): " << metLog.GetMean() << endl;
    cout << "The sample standard deviation(S) (km/h): " << metLog.GetStdDev() << endl;

    Vector<MetRecType> matches = metLog.FindMatchesToMean( MATCH_TOLERANCE );
    if (matches.Size() == 0)
        cout << "No reading matches the average speed." << endl;
    else
    {   // Print the date and time of every reading that matches the average, one per line
        cout << "Readings matching the average speed:" << endl;
        for (int counter = 0; counter < matches.Size(); counter++)// readings within MATCH_TOLERANCE of the mean
            cout << matches[counter].GetDate() << " " << matches[counter].GetTime() << endl;
    }
}
