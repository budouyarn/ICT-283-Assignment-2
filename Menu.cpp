/**
 * @brief Implementation of the Menu class: the weather data menu and the
 * functions that ask the user for input and print the results.
 * @author  Lim Wen Xin Sherine
 * @version 2.0
 * @date 01/11/2026
 */

#include "Menu.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::istringstream;
using std::ofstream;
using std::string;

/** @brief How close (km/h) a wind speed must be to the mean to "match" it (half of the last printed digit). */
const double MATCH_TOLERANCE = 0.005;

/** @brief The names of the months, so MONTH_NAMES[month - 1] gives the name of month 1-12. */
const string MONTH_NAMES[12] ={"January", "February", "March", "April", "May", "June","July", "August", "September", "October", "November", "December"};

// Menu choices
const int MENU_WIND = 1;
const int MENU_TEMPERATURE = 2;
const int MENU_CORRELATION = 3;// replaces the old solar radiation total
const int MENU_MONTHLY_TABLE = 4;// the (stdev, mad) table written to WindTempSolar.csv
const int MENU_WHOLE_FILE = 5;
const int MENU_EXIT = 0;
/**
 * @brief Rounds a number to 2 decimal places, e.g. 3.456 -> 3.46.
 * @param value: the number to round
 * @return the number rounded to hundredths
 */
static double RoundToTwo( double value )
{
    return std::round( value * 100.0 ) / 100.0;
}
void Menu::Run( const Metlog & metLog, const MetStore & store ) const
{
    int choice = MENU_EXIT;
    do
    {
        PrintMenu();
        if (!ReadInt( "Enter your choice: ", 0, 5, choice ))
            break;// input ended

        if (choice == MENU_WHOLE_FILE)
            ShowWholeFile( metLog );
        else if (choice == MENU_MONTHLY_TABLE)
        {
            int year = 0;
            if (!ReadInt( "Enter the year (e.g. 2014): ", 1, 9999, year ))
                break;
            ShowMonthlyTable( store, year );
        }
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
         << MENU_WIND << ". Average wind speed and standard deviation for one month and year" << endl
         << MENU_TEMPERATURE << ". Average air temperature and standard deviation for one month and year" << endl
         << MENU_CORRELATION << ". Sample Pearson Correlation Coefficient(S_T, S_R, T_R) for one month, over all years" << endl
         << MENU_MONTHLY_TABLE << ". Average wind speed and temperature (each with stdev, mad) and total solar radiation for each month of a year, written to WindTempSolar.csv" << endl
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
    cout << "Average wind speed (km/h): " << RoundToTwo( store.GetSpeedMean( month, year ) ) << endl;

    if (count < 2) // a sample standard deviation needs at least 2 readings
        cout << "Standard deviation: not available (needs at least 2 readings)" << endl;
    else
        cout << "Standard deviation (km/h): " << RoundToTwo( store.GetSpeedStdDev( month, year ) ) << endl;
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
    cout << "Average temperature (degrees C): " << RoundToTwo( store.GetTemperatureMean( month, year ) ) << endl;

    if (count < 2)
        cout << "Standard deviation: not available (needs at least 2 readings)" << endl;
    else
        cout << "Sample standard deviation (degrees C): " << RoundToTwo( store.GetTemperatureStdDev( month, year ) ) << endl;
}

/**
 * @brief Option 3: prints the Sample Pearson correlation coefficient for S_T, S_R and T_R
 * for one month, using the readings of every year that was loaded.
 */
void Menu::ShowCorrelations( const MetStore & store, int month ) const
{
    MetStore::Correlations result = store.CalculateCorrelations( month );

    cout << endl << "Sample Pearson Correlation Coefficient for " << MONTH_NAMES[month - 1] << endl;

    if (IsDefined( result.st ))
    {
        cout << "S_T: " << RoundToTwo( result.st ) << endl;
    }
    else
        cout << "S_T: not available (needs at least 2 matching readings that vary)" << endl;

    if (IsDefined( result.sr ))
    {
        cout << "S_R: " << RoundToTwo( result.sr ) << endl;
    }
    else
        cout << "S_R: not available (needs at least 2 matching readings that vary)" << endl;

    if (IsDefined( result.tr ))
    {
        cout << "T_R: " << RoundToTwo( result.tr ) << endl;
    }
    else
        cout << "T_R: not available (needs at least 2 matching readings that vary)" << endl;
}

/**
 * @brief Option 4: writes, for every month of one year, the average wind speed, the average air
 * temperature (each with (stdev, mad) next to it) and the total solar radiation to WindTempSolar.csv.
 * A month with no valid readings of a kind shows N/A; a standard deviation needs at least 2 readings.
 */
void Menu::ShowMonthlyTable( const MetStore & store, int year ) const
{
    ofstream printCSV( "WindTempSolar.csv" );
    if (!printCSV)
    {
        cout << "Could not create WindTempSolar.csv" << endl;
        return;
    }
    printCSV << year << endl;
    for (int month = 1; month <= 12; month++)
    {
        printCSV << MONTH_NAMES[month - 1] << ",";

        int speedCount = store.GetSpeedCount( month, year );
        if (speedCount == 0)
            printCSV << "N/A";
        else
        {
            printCSV << RoundToTwo( store.GetSpeedMean( month, year ) ) << "(";
            if (speedCount < 2) printCSV << "N/A"; else printCSV << RoundToTwo( store.GetSpeedStdDev( month, year ) );
            printCSV << ", " << RoundToTwo( store.GetSpeedMad( month, year ) ) << ")";
        }
        printCSV << ",";

        int tempCount = store.GetTemperatureCount( month, year );
        if (tempCount == 0)
            printCSV << "N/A";
        else
        {
            printCSV << RoundToTwo( store.GetTemperatureMean( month, year ) ) << "(";
            if (tempCount < 2) printCSV << "N/A"; else printCSV << RoundToTwo( store.GetTemperatureStdDev( month, year ) );
            printCSV << ", " << RoundToTwo( store.GetTemperatureMad( month, year ) ) << ")";
        }
        printCSV << ",";

        if (store.GetSolarCount( month, year ) == 0)
            printCSV << "N/A";
        else
            printCSV << RoundToTwo( store.GetSolarTotal( month, year ) );
        printCSV << endl;
    }
    cout << endl << "Monthly table for " << year << " written to WindTempSolar.csv" << endl;
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
    cout << "The mean wind speed (km/h): " << RoundToTwo( metLog.GetMean() ) << endl;
    cout << "The sample standard deviation(S) (km/h): " << RoundToTwo( metLog.GetStdDev() ) << endl;

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
