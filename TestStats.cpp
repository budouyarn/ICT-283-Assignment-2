/**
 * @file TestStats.cpp
 * @brief Simple tests for Mean, StdDev, Mad, sPCC and IsDefined.
 * Expected values are worked out by hand, except the Excel PEARSON documentation example.
 */
#include <iostream>
#include <cmath>
#include "Stats.h"

using std::cout;
using std::endl;

static int failed = 0;//number of checks that did not hold

/**
 * @brief Prints FAIL when two values are not close enough (doubles are never compared with ==).
 * @param what: description of the check
 * @param actual: the value the programme gave
 * @param expected: the value worked out by hand
 */
static void checkNear( const char * what, double actual, double expected )
{
    if (std::fabs( actual - expected ) > 0.000001)
    {
        failed++;
        cout << "FAIL: " << what << endl;
    }
}

/**
 * @brief Builds a Vector<double> from a plain array.
 * @param values: the array
 * @param element: how many values to copy
 * @return a Vector<double> holding those element values
 */
static Vector<double> make( const double * values, int element )
{
    Vector<double> vector;
    for (int counter = 0; counter < element; counter++)
        vector.Insert( vector.Size(), values[counter] );
    return vector;
}

/**
 * @brief Runs the checks.
 * @return 0 if every check held, 1 otherwise
 */
int main()
{
    const double one[] = { 1, 2, 3, 4, 5 };

    //Check near checks for tolerance. if it does goes beyond value, then it will fail and return fail after all the run

    // Mean: 15 / 5 = 3
    checkNear( "Mean {1-5} = 3", Mean( make( one, 5 ) ), 3.0 );
    // StdDev (sample, element-1): sqrt( 10 / 4 ) = 1.5811388
    checkNear( "StdDev {1-5} = 1.5811388", StdDev( make( one, 5 ) ), 1.5811388 );
    // Mad: distances from the mean 2 1 0 1 2 = 6, and 6 / 5 = 1.2
    checkNear( "Mad {1-5} = 1.2", Mad( make( one, 5 ) ), 1.2 );
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
    if (IsDefined( sPCC( make( one, 5 ), make( flat, 5 ) ) ))
    {
        failed++;
        cout << "FAIL: a flat set should be undefined" << endl;
    }
    if (IsDefined( sPCC( make( one, 1 ), make( one, 1 ) ) ))
    {
        failed++;
        cout << "FAIL: one pair should be undefined" << endl;
    }

      if (failed == 0)
    {
        cout << "All checks passed" << endl;
        return 0;
    }
    cout << failed << " check(s) failed" << endl;
    return 1;
}
