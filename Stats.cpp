#include "Stats.h"
#include <cmath>
#include <limits>

using namespace std;

double Mean( const Vector<double> & values )
{
    if (values.Size() == 0)
        return 0;  // avoids dividing by zero

    double sum = 0;
    for (int counter = 0; counter < values.Size(); counter++)
        sum += values[counter];
    return sum / values.Size();
}

double StdDev( const Vector<double> & values )
{
    if (values.Size() < 2)
        return 0; // n - 1 would be 0 or negative

    double mean = Mean( values );
    double squaredDiffs = 0;
    for (int counter = 0; counter < values.Size(); counter++)
    {
        double diff = values[counter] - mean;
        squaredDiffs += diff * diff;
    }
    return std::sqrt( squaredDiffs / (values.Size() - 1) );
}

double Mad( const Vector<double> & values )
{
    if (values.Size() == 0)
        return 0;// avoids dividing by zero

    double mean = Mean( values );
    double sumOfDistances = 0;
    for (int counter = 0; counter < values.Size(); counter++)
        sumOfDistances += std::fabs( values[counter] - mean ); // distance from the mean, never negative
    return sumOfDistances / values.Size();
}

double sPCC( const Vector<double> & first, const Vector<double> & second )
{
    const double UNDEFINED = std::numeric_limits<double>::quiet_NaN();

    if (first.Size() != second.Size() || first.Size() < 2)
        return UNDEFINED;

    double meanX = Mean( first );
    double meanY = Mean( second );

    double sumXY = 0; // sum of (first - meanX)(second - meanY)
    double sumXX = 0; // sum of (first - meanX)^2
    double sumYY = 0; // sum of (second - meanY)^2
    for (int counter = 0; counter < first.Size(); counter++)
    {
        double deviation = first[counter] - meanX;
        double deviation2 = second[counter] - meanY;
        sumXY += deviation * deviation2;
        sumXX += deviation * deviation;
        sumYY += deviation2 * deviation2;
    }

    if (sumXX <= 0 || sumYY <= 0)  // one set never varies: nothing to correlate
        return UNDEFINED;

    return sumXY / std::sqrt( sumXX * sumYY );
}

bool IsDefined( double coefficient )
{
    return !std::isnan( coefficient );
}
