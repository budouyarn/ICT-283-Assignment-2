// Implementation of MetStore: the weather readings arranged by year and month, and the
// statistics asked of one month (counts, means, standard deviations, mad, solar total, correlations).

#include "MetStore.h"
#include "Stats.h"
#include <vector>

namespace
{
    const double HOURS_PER_READING = 10.0 / 60.0; // one reading is the average power over 10 minutes

    // The valid values taken from some readings. A local variable of this type carries the figures,
    // so nothing is static and nothing is shared between calls.
    struct Lists
    {
        Vector<double> speeds, temperatures, solars;
        Vector<double> stSpeed, stTemp, srSpeed, srSolar, trTemp, trSolar; // matched pairs
    };

    // Adds every valid value of one month's readings to lists.
    void AddReadings( const Vector<MetRecType> & readings, Lists & lists )
    {
        for (int counter = 0; counter < readings.Size(); counter++)
        {
            const MetRecType & record = readings[counter];
            bool hasSpeed = !record.IsSpeedMissing();
            bool hasTemp = !record.IsTempMissing();
            bool hasRad = !record.IsSolarMissing();

            if (hasSpeed) lists.speeds.Insert( lists.speeds.Size(), record.GetSpeed() );
            if (hasTemp) lists.temperatures.Insert( lists.temperatures.Size(), record.GetTemperature() );
            if (hasRad) lists.solars.Insert( lists.solars.Size(), record.GetSolarRadiation() );

            // pairs: both values must be valid, so the two lists stay matched
            if (hasSpeed && hasTemp)
            {
                lists.stSpeed.Insert( lists.stSpeed.Size(), record.GetSpeed() );
                lists.stTemp.Insert( lists.stTemp.Size(), record.GetTemperature() );
            }
            if (hasSpeed && hasRad)
            {
                lists.srSpeed.Insert( lists.srSpeed.Size(), record.GetSpeed() );
                lists.srSolar.Insert( lists.srSolar.Size(), record.GetSolarRadiation() );
            }
            if (hasTemp && hasRad)
            {
                lists.trTemp.Insert( lists.trTemp.Size(), record.GetTemperature() );
                lists.trSolar.Insert( lists.trSolar.Size(), record.GetSolarRadiation() );
            }
        }
    }

    // Puts the readings in date-and-time order with a merge sort (written here, so <algorithm> is not needed). The sorted runs are
    // merged in pairs (width 1, 2, 4, ...) until the whole Vector is one run. Taking from the left
    // run when two readings are equal keeps equal readings in their original order (stable).
    void SortReadings( std::vector<MetRecType> & items )
    {
        int total = static_cast<int>( items.size() );
        for (int width = 1; width < total; width *= 2)
        {
            std::vector<MetRecType> merged;
            merged.reserve( items.size() );
            for (int low = 0; low < total; low += 2 * width)
            {
                int middle = (low + width < total) ? low + width : total;
                int high = (low + 2 * width < total) ? low + 2 * width : total;
                int left = low, right = middle;
                while (left < middle && right < high)
                {
                    if (items[right] < items[left])
                        merged.push_back( items[right++] );
                    else
                        merged.push_back( items[left++] );
                }
                while (left < middle)
                    merged.push_back( items[left++] );
                while (right < high)
                    merged.push_back( items[right++] );
            }
            items = merged;
        }
    }

    // The readings of one month of one year, or nullptr if there are none.
    const MonthData * FindMonth( const Map<int, Bst<MonthData> > & years, int year, int month )
    {
        const Bst<MonthData> * months = years.Find( year );
        if (months == nullptr)
            return nullptr;
        return months->find( MonthData( month ) );
    }

    // The valid values of one month of one year (empty lists if that month is not held).
    Lists Collect( const Map<int, Bst<MonthData> > & years, int year, int month )
    {
        Lists lists;
        const MonthData * found = FindMonth( years, year, month );
        if (found != nullptr)
            AddReadings( found->readings, lists );
        return lists;
    }
}

MetStore::MetStore() : m_years()
{
}

void MetStore::Load( const Metlog & metLog )
{
    m_years = Map<int, Bst<MonthData> >();

    std::vector<MetRecType> sorted; // sorting needs random access, so a std::vector is used here
    sorted.reserve( metLog.GetRecordCount() );
    for (int counter = 0; counter < metLog.GetRecordCount(); counter++)
        sorted.push_back( metLog.GetRecord( counter ) );
    SortReadings( sorted );// by date, then time (operator< of MetRecType)

    int total = static_cast<int>( sorted.size() );
    int start = 0;
    while (start < total) // after sorting, each month is one run of records
    {
        int year = sorted[start].GetDate().GetYear();
        int month = sorted[start].GetDate().GetMonth();
        MonthData data( month );
        int end = start;
        while (end < total && sorted[end].GetDate().GetYear() == year && sorted[end].GetDate().GetMonth() == month)
        {
            // equal records are neighbours after the sort; the same date and time counts as one reading
            bool repeat = end > start && !(sorted[end - 1] < sorted[end]);
            if (!repeat)
                data.readings.Insert( data.readings.Size(), sorted[end] );
            end++;
        }

        m_years.Insert( year, Bst<MonthData>() );// refused (false) if the year is already there
        m_years.Find( year )->insert( data );
        start = end;
    }
}

int MetStore::GetYearCount() const
{
    return m_years.Keys().Size(); // one key per year
}

int MetStore::GetReadingCount() const
{
    int total = 0;
    Vector<int> keys = m_years.Keys();
    for (int counter = 0; counter < keys.Size(); counter++)
        for (int month = 1; month <= 12; month++)
        {
            const MonthData * found = FindMonth( m_years, keys[counter], month );
            if (found != nullptr)
                total += found->readings.Size();
        }
    return total;
}

int MetStore::GetSpeedCount( int month, int year ) const
{
    Lists lists = Collect( m_years, year, month );
    return lists.speeds.Size();
}

double MetStore::GetSpeedMean( int month, int year ) const
{
    Lists lists = Collect( m_years, year, month );
    return Mean( lists.speeds );
}

double MetStore::GetSpeedStdDev( int month, int year ) const
{
    Lists lists = Collect( m_years, year, month );
    return StdDev( lists.speeds );
}

int MetStore::GetTemperatureCount( int month, int year ) const
{
    Lists lists = Collect( m_years, year, month );
    return lists.temperatures.Size();
}

double MetStore::GetTemperatureMean( int month, int year ) const
{
    Lists lists = Collect( m_years, year, month );
    return Mean( lists.temperatures );
}

double MetStore::GetTemperatureStdDev( int month, int year ) const
{
    Lists lists = Collect( m_years, year, month );
    return StdDev( lists.temperatures );
}

double MetStore::GetSpeedMad( int month, int year ) const
{
    Lists lists = Collect( m_years, year, month );
    return Mad( lists.speeds );
}

double MetStore::GetTemperatureMad( int month, int year ) const
{
    Lists lists = Collect( m_years, year, month );
    return Mad( lists.temperatures );
}

int MetStore::GetSolarCount( int month, int year ) const
{
    Lists lists = Collect( m_years, year, month );
    return lists.solars.Size();
}

double MetStore::GetSolarTotal( int month, int year ) const
{
    Lists lists = Collect( m_years, year, month );
    double sum = 0; // W/m2, summed over the valid readings
    for (int counter = 0; counter < lists.solars.Size(); counter++)
        sum += lists.solars[counter];
    return sum * HOURS_PER_READING / 1000; // W/m2 x hours / 1000 = kWh/m2
}

MetStore::Correlations MetStore::CalculateCorrelations( int month ) const
{
    Lists lists;
    Vector<int> keys = m_years.Keys();
    for (int counter = 0; counter < keys.Size(); counter++)// every year; the month is asked of each
    {
        const MonthData * found = FindMonth( m_years, keys[counter], month );
        if (found != nullptr)
            AddReadings( found->readings, lists );
    }

    Correlations result;
    result.st = sPCC( lists.stSpeed, lists.stTemp );
    result.sr = sPCC( lists.srSpeed, lists.srSolar );
    result.tr = sPCC( lists.trTemp, lists.trSolar );
    result.stPairs = lists.stSpeed.Size();
    result.srPairs = lists.srSpeed.Size();
    result.trPairs = lists.trTemp.Size();
    return result;
}
