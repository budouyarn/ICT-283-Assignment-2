#include "Stats.h"
#include <cmath>
#include <limits>

double Mean( const Vector<double> & values )
{
    if (values.Size() == 0)
        return 0;                                  // avoids dividing by zero

    double sum = 0;
    for (int counter = 0; counter < values.Size(); counter++)
        sum += values[counter];
    return sum / values.Size();
}

double StdDev( const Vector<double> & values )
{
    if (values.Size() < 2)
        return 0;                                  // n - 1 would be 0 or negative

    double mean = Mean( values );
    double squaredDiffs = 0;
    for (int counter = 0; counter < values.Size(); counter++)
    {
        double diff = values[counter] - mean;
        squaredDiffs += diff * diff;
    }
    return std::sqrt( squaredDiffs / (values.Size() - 1) );
}

double sPCC( const Vector<double> & x, const Vector<double> & y )
{
    const double UNDEFINED = std::numeric_limits<double>::quiet_NaN();

    if (x.Size() != y.Size() || x.Size() < 2)
        return UNDEFINED;

    double meanX = Mean( x );
    double meanY = Mean( y );

    double sumXY = 0;                              // sum of (x - meanX)(y - meanY)
    double sumXX = 0;                              // sum of (x - meanX)^2
    double sumYY = 0;                              // sum of (y - meanY)^2
    for (int counter = 0; counter < x.Size(); counter++)
    {
        double dx = x[counter] - meanX;
        double dy = y[counter] - meanY;
        sumXY += dx * dy;
        sumXX += dx * dx;
        sumYY += dy * dy;
    }

    if (sumXX <= 0 || sumYY <= 0)                  // one set never varies: nothing to correlate
        return UNDEFINED;

    return sumXY / std::sqrt( sumXX * sumYY );
}

bool IsDefined( double coefficient )
{
    return !std::isnan( coefficient );
}
