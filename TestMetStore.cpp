/**
 * @file TestMetStore.cpp
 * @brief Simple test of MetStore: loading, duplicate filtering, missing values and one month's statistics.
 * Needs MetStore, Metlog, MetRecType, Date, Time, Stats (and their headers) in the same build.
 */
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include "MetStore.h"
#include "Metlog.h"

using std::cout;
using std::endl;
using std::string;

static int testNumber = 0;///< which test is running

/**
 * @brief Runs one test: prints PASS and lets the next test run, or prints FAIL and stops the whole run.
 * Stopping at the first failure means a later line never uses a result that is already known to be wrong
 * (for example a pointer that Find returned as nullptr).
 * @param condition: true if the test holds
 * @param what: description of the test
 * @param detail: what was expected and what was found (printed only on FAIL)
 */
static void check( bool condition, const string & what, const string & detail = "" )
{
    testNumber++;
    if (condition)
    {
        cout << "Test " << testNumber << ": " << what << " ... PASS" << endl;
        return;
    }
    cout << "Test " << testNumber << ": " << what << " ... FAIL" << endl;
    if (!detail.empty())
        cout << "    " << detail << endl;
    cout << "Stopped at test " << testNumber << ", the later tests were not run" << endl;
    exit( 1 );
}

/**
 * @brief Adds one reading to a Metlog.
 * @param log: the Metlog to add to
 * @param day, month, year: the date
 * @param hour, minute: the time
 * @param speed, temperature, radiation: the measurements (use MISSING_SPEED / MISSING_SOLAR when absent)
 */
static void add( Metlog & log, int day, int month, int year, int hour, int minute,
                 float speed, float temperature, float radiation )
{
    MetRecType record;
    record.SetDate( Date( day, month, year ) );
    record.SetTime( Time( hour, minute ) );
    record.SetSpeed( speed );
    record.SetTemperature( temperature );
    record.SetSolarRadiation( radiation );
    log.AddRecord( record );
}

/**
 * @brief Runs the checks.
 * @return 0 if every test passed (the run stops at the first failure)
 */
int main()
{
    // an empty store holds nothing
    MetStore empty;
    check( empty.GetReadingCount() == 0, "new store has no readings" );
    check( empty.GetSpeedCount( 1, 2014 ) == 0, "new store has no speeds" );

    // 7 records added out of date order: 2015 before 2014, one missing speed, one missing solar, one exact duplicate
    Metlog log;
    add( log, 1, 1, 2015, 0, 10, 60, 6, MISSING_SOLAR );  // Jan 2015, solar missing
    add( log, 1, 1, 2014, 0, 20, 30, 3, 300 ); // Jan 2014
    add( log, 1, 1, 2015, 0, 20, 70, 8, 650 ); // Jan 2015
    add( log, 1, 1, 2014, 0, 0, 10, 1, 100 ); // Jan 2014
    add( log, 2, 1, 2014, 0, 0, MISSING_SPEED, 5, 500 );// Jan 2014, speed missing
    add( log, 1, 1, 2014, 0, 10, 20, 2, 200 );// Jan 2014
    add( log, 1, 1, 2014, 0, 0, 10, 1, 100 );// exact copy of the 4th record

    MetStore store;
    store.Load( log );

    check( store.GetYearCount() == 2, "2 years (2014 and 2015)" );
    check( store.GetReadingCount() == 6, "7 records, 1 duplicate dropped, so 6 readings" );

    // Jan 2014 holds 4 readings: speeds 30, 10, missing, 20 and solars 300, 100, 500, 200
    check( store.GetSpeedCount( 1, 2014 ) == 3, "Jan 2014: 3 valid speeds (one missing)" );
    check( std::fabs( store.GetSpeedMean( 1, 2014 ) - 20.0 ) < 0.000001, "Jan 2014: speed mean (30+10+20)/3 = 20" );
    check( store.GetSolarCount( 1, 2014 ) == 4, "Jan 2014: 4 valid solar readings" );

    // a month with no readings
    check( store.GetSpeedCount( 2, 2014 ) == 0, "Feb 2014: no readings" );

    cout << "All " << testNumber << " tests passed" << endl;
    return 0;
}

 