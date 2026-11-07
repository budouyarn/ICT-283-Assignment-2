/**
 * @file TestStats.cpp
 * @brief Simple tests for Mean, StdDev, Mad, sPCC and IsDefined.
 * Expected values are worked out by hand, except the Excel PEARSON documentation example.
 */
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include "Stats.h"

using std::cout;
using std::endl;
using std::string;
using std::ostringstream;

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
 * @brief Runs one test on a decimal result: PASS when it is within 0.000001 of the expected value, otherwise FAIL.
 * @param what: description of the test
 * @param actual: the value the programme gave
 * @param expected: the value worked out by hand
 */
static void checkNear( const string & what, double actual, double expected )
{
    ostringstream detail;
    detail << "expected " << expected << ", found " << actual;
    check( std::fabs( actual - expected ) <= 0.000001, what, detail.str() );
}

/**
 * @brief Builds a Vector<double> from a plain array.
 * @param values: the array
 * @param n: how many values to copy
 * @return a Vector<double> holding those n values
 */
static Vector<double> make( const double * values, int n )
{
    Vector<double> vector;
    for (int counter = 0; counter < n; counter++)
        vector.Insert( vector.Size(), values[counter] );
    return vector;
}

/**
 * @brief Runs the checks.
 * @return 0 if every test passed (the run stops at the first failure)
 */
int main()
{
    const double one[] = { 1, 2, 3, 4, 5 };

    // Mean: 15 / 5 = 3
    checkNear( "Mean {1..5} = 3", Mean( make( one, 5 ) ), 3.0 );
    // StdDev (sample, n-1): sqrt( 10 / 4 ) = 1.5811388
    checkNear( "StdDev {1..5} = 1.5811388", StdDev( make( one, 5 ) ), 1.5811388 );
    // Mad: distances from the mean 2 1 0 1 2 = 6, and 6 / 5 = 1.2
    checkNear( "Mad {1..5} = 1.2", Mad( make( one, 5 ) ), 1.2 );
    // nothing to measure gives 0
    checkNear( "Mean of empty = 0", Mean( Vector<double>() ), 0.0 );

    // sPCC: the example in Excel's PEARSON documentation
    const double x[] = { 9, 7, 5, 3, 1 };
    const double y[] = { 10, 6, 1, 5, 3 };
    checkNear( "Excel example = 0.699379", sPCC( make( x, 5 ), make( y, 5 ) ), 0.699379 );

    // sPCC: a perfect straight line up is +1, and down is -1
    const double up[] = { 2, 4, 6, 8, 10 };
    const double down[] = { 10, 8, 6, 4, 2 };
    checkNear( "line up = +1", sPCC( make( one, 5 ), make( up, 5 ) ), 1.0 );
    checkNear( "line down = -1", sPCC( make( one, 5 ), make( down, 5 ) ), -1.0 );

    // sPCC: no answer (NaN) when one set never varies, or there is only one pair
    const double flat[] = { 3, 3, 3, 3, 3 };
    check( !IsDefined( sPCC( make( one, 5 ), make( flat, 5 ) ) ), "a flat set is undefined (NaN)" );
    check( !IsDefined( sPCC( make( one, 1 ), make( one, 1 ) ) ), "one pair is undefined (NaN)" );

    cout << "All " << testNumber << " tests passed" << endl;
    return 0;
}