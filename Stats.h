#ifndef STATS_H
#define STATS_H

#include "Vector.h"

/**
 * @brief Mean of the values.
 * @param values: the values to average
 * @return the mean, or 0 if there are no values
 */
double Mean( const Vector<double> & values );

/**
 * @brief Sample standard deviation (divides by n - 1).
 * @param values: the values to measure
 * @return the standard deviation, or 0 if there are fewer than 2 values
 */
double StdDev( const Vector<double> & values );
/**
 * @brief Mean Absolute Deviation of ungrouped data: the mean of |first - mean|.
 * It works on plain numbers, so it knows nothing about weather data.
 * @param values: the numbers
 * @return the mean absolute deviation, 0 if there are no values
 */
double Mad( const Vector<double> & values );

/**
 * @brief Pearson Correlation Coefficient between two sets of values, matched pair by pair
 * Result is between -1 (perfect opposite) and +1 (perfect match), 0 means no straight-line relationship.
 *
 *   sPCC = sum( (first - mean1)(second - meanY) ) / sqrt( sum( (first - mean1)^2 ) * sum( (second - meanY)^2 ) )
 *
 * It is undefined, so NaN is returned, when
 *   - the two Vectors are not the same length,
 *   - there are fewer than 2 pairs, or
 *   - one of the sets never varies (every value the same), so the formula would divide by zero.
 * A caller tests for that with IsDefined(). (Returning 0 would wrongly claim "no relationship".)
 *
 * @param first: the first set of values
 * @param second: the second set of values, same length as first
 * @return the coefficient, or NaN if it is undefined
 */
double sPCC( const Vector<double> & first, const Vector<double> & second );

/**
 * @brief Tells whether a result from sPCC is a real number.
 * @param coefficient: the value returned by sPCC
 * @return false if sPCC could not calculate it (NaN), true otherwise
 */
bool IsDefined( double coefficient );
// checks  cases such as:
//The two lists have different lengths.
//There are fewer than 2 pairs of readings.
//All values in one list are identical(eg.wind speed constant at 5.0 all month. The formula would then divide by zero).

#endif // STATS_H

