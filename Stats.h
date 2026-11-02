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
 * @brief Sample Pearson Correlation Coefficient between two sets of values, matched pair by pair:
 * x[0] goes with y[0], x[1] with y[1], and so on.
 * Result is between -1 (perfect opposite) and +1 (perfect match); 0 means no straight-line relationship.
 *
 *   sPCC = sum( (x - meanX)(y - meanY) ) / sqrt( sum( (x - meanX)^2 ) * sum( (y - meanY)^2 ) )
 *
 * It is undefined, so NaN is returned, when
 *   - the two Vectors are not the same length,
 *   - there are fewer than 2 pairs, or
 *   - one of the sets never varies (every value the same), so the formula would divide by zero.
 * A caller tests for that with IsDefined(). (Returning 0 would wrongly claim "no relationship".)
 *
 * @param x: the first set of values
 * @param y: the second set of values, same length as x
 * @return the coefficient, or NaN if it is undefined
 */
double sPCC( const Vector<double> & x, const Vector<double> & y );

/**
 * @brief Tells whether a result from sPCC is a real number.
 * @param coefficient: the value returned by sPCC
 * @return false if sPCC could not calculate it (NaN), true otherwise
 */
bool IsDefined( double coefficient );

#endif // STATS_H

