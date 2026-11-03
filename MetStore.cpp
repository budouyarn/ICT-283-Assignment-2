#include "MetStore.h"
#include "Stats.h"
#include <algorithm>
#include <vector>

// ---------------------------------------------------------------------------------------------
// The call-back that collects figures while a tree is traversed.
// A Bst calls a plain function once per reading, and a plain function cannot carry data with it,
// so the collected figures live in static members. That makes this code single-threaded only,
// and Start() must be called before every traversal.
// ---------------------------------------------------------------------------------------------
namespace
{
    const double HOURS_PER_READING = 10.0 / 60.0; // one reading is the average power over 10 minutes

    class Gather
    {
        public:
            static void Start()
            {
                s_speeds = Vector<double>();
                s_temperatures = Vector<double>();
                s_solars = Vector<double>();
                s_stSpeed = s_stTemp = s_srSpeed = s_srSolar = s_trTemp = s_trSolar = Vector<double>();
            }

            // the function whose address is given to the tree
            static void Visit( MetRecType & record )
            {
                // no month test needed: every tree handed to the traversal holds one month only
                bool hasSpeed = !record.IsSpeedMissing();
                bool hasTemp = !record.IsTempMissing();
                bool hasRad = !record.IsSolarMissing();

                if (hasSpeed) s_speeds.Insert( s_speeds.Size(), record.GetSpeed() );
                if (hasTemp) s_temperatures.Insert( s_temperatures.Size(), record.GetTemperature() );
                if (hasRad) s_solars.Insert( s_solars.Size(), record.GetSolarRadiation() );

                // pairs: both values must be valid, so the two lists stay matched
                if (hasSpeed && hasTemp)
                {
                    s_stSpeed.Insert( s_stSpeed.Size(), record.GetSpeed() );
                    s_stTemp.Insert( s_stTemp.Size(), record.GetTemperature() );
                }
                if (hasSpeed && hasRad)
                {
                    s_srSpeed.Insert( s_srSpeed.Size(), record.GetSpeed() );
                    s_srSolar.Insert( s_srSolar.Size(), record.GetSolarRadiation() );
                }
                if (hasTemp && hasRad)
                {
                    s_trTemp.Insert( s_trTemp.Size(), record.GetTemperature() );
                    s_trSolar.Insert( s_trSolar.Size(), record.GetSolarRadiation() );
                }
            }

            static Vector<double> s_speeds, s_temperatures, s_solars;
            static Vector<double> s_stSpeed, s_stTemp, s_srSpeed, s_srSolar, s_trTemp, s_trSolar;
    };

    Vector<double> Gather::s_speeds, Gather::s_temperatures, Gather::s_solars;
    Vector<double> Gather::s_stSpeed, Gather::s_stTemp, Gather::s_srSpeed, Gather::s_srSolar,
                   Gather::s_trTemp, Gather::s_trSolar;

    // Inserts items[low..high] middle-first so the tree stays balanced:
    // the middle goes in first, then the middle of each half, and so on.
    void InsertMiddleFirst( const std::vector<MetRecType> & items, int low, int high, Bst<MetRecType> & tree )
    {
        if (low > high)
            return;
        int middle = low + (high - low) / 2;
        tree.insert( items[middle] );
        InsertMiddleFirst( items, low, middle - 1, tree );
        InsertMiddleFirst( items, middle + 1, high, tree );
    }

    // The one place the cell key is built: March 2015 -> 201503 (year 1 to 9999, month 1 to 12).
    int MakeKey( int year, int month )
    {
        return year * 100 + month;
    }

    // Collects the readings of one month of one year (an empty result if that cell does not exist).
    void GatherCell( const Map<int, Bst<MetRecType> > & cells, int year, int month )
    {
        Gather::Start();
        const Bst<MetRecType> * tree = cells.Find( MakeKey( year, month ) );
        if (tree != nullptr)
            tree->inorderTraversal( Gather::Visit ); // pass the function, not a call
    }
}

MetStore::MetStore() : m_cells()
{
}

void MetStore::Load( const Metlog & metLog )
{
    m_cells = Map<int, Bst<MetRecType> >();

    std::vector<MetRecType> sorted; // sorted needs random access, so a std::vector is used here
    sorted.reserve( metLog.GetRecordCount() );
    for (int counter = 0; counter < metLog.GetRecordCount(); counter++)
        sorted.push_back( metLog.GetRecord( counter ) );
    std::sort( sorted.begin(), sorted.end() );// by date, then time (operator< of MetRecType)

    int total = static_cast<int>( sorted.size() );
    int start = 0;
    while (start < total) // after sorting, each month is one run of records
    {
        int key = MakeKey( sorted[start].GetDate().GetYear(), sorted[start].GetDate().GetMonth() );
        int end = start;
        while (end < total && MakeKey( sorted[end].GetDate().GetYear(), sorted[end].GetDate().GetMonth() ) == key)
            end++;

        m_cells.Insert( key, Bst<MetRecType>() );
        InsertMiddleFirst( sorted, start, end - 1, *m_cells.Find( key ) );
        start = end;
    }
}

int MetStore::GetYearCount() const
{
    Vector<int> keys = m_cells.Keys();
    int years = 0;
    for (int counter = 0; counter < keys.Size(); counter++)
    {
        bool seen = false;// count a year only the first time one of its cells is met
        for (int earlier = 0; earlier < counter; earlier++)
            if (keys[earlier] / 100 == keys[counter] / 100)
                seen = true;
        if (!seen)
            years++;
    }
    return years;
}

int MetStore::GetReadingCount() const
{
    int total = 0;
    Vector<int> keys = m_cells.Keys();
    for (int counter = 0; counter < keys.Size(); counter++)
        total += m_cells.Find( keys[counter] )->treeNodeCount();
    return total;
}

int MetStore::GetSpeedCount( int month, int year ) const
{
    GatherCell( m_cells, year, month );
    return Gather::s_speeds.Size();
}

double MetStore::GetSpeedMean( int month, int year ) const
{
    GatherCell( m_cells, year, month );
    return Mean( Gather::s_speeds );
}

double MetStore::GetSpeedStdDev( int month, int year ) const
{
    GatherCell( m_cells, year, month );
    return StdDev( Gather::s_speeds );
}

int MetStore::GetTemperatureCount( int month, int year ) const
{
    GatherCell( m_cells, year, month );
    return Gather::s_temperatures.Size();
}

double MetStore::GetTemperatureMean( int month, int year ) const
{
    GatherCell( m_cells, year, month );
    return Mean( Gather::s_temperatures );
}

double MetStore::GetTemperatureStdDev( int month, int year ) const
{
    GatherCell( m_cells, year, month );
    return StdDev( Gather::s_temperatures );
}

double MetStore::GetSpeedMad( int month, int year ) const
{
    GatherCell( m_cells, year, month );
    return Mad( Gather::s_speeds );
}

double MetStore::GetTemperatureMad( int month, int year ) const
{
    GatherCell( m_cells, year, month );
    return Mad( Gather::s_temperatures );
}

int MetStore::GetSolarCount( int month, int year ) const
{
    GatherCell( m_cells, year, month );
    return Gather::s_solars.Size();
}

double MetStore::GetSolarTotal( int month, int year ) const
{
    GatherCell( m_cells, year, month );
    double sum = 0;                                                     // W/m2, summed over the valid readings
    for (int counter = 0; counter < Gather::s_solars.Size(); counter++)
        sum += Gather::s_solars[counter];
    return sum * HOURS_PER_READING / 1000;                              // W/m2 x hours / 1000 = kWh/m2
}

MetStore::Correlations MetStore::GetCorrelations( int month ) const
{
    Gather::Start();
    Vector<int> keys = m_cells.Keys();
    for (int counter = 0; counter < keys.Size(); counter++)             // every cell; only those of this month are used
        if (keys[counter] % 100 == month)
            m_cells.Find( keys[counter] )->inorderTraversal( Gather::Visit );

    Correlations result;
    result.st = sPCC( Gather::s_stSpeed, Gather::s_stTemp );
    result.sr = sPCC( Gather::s_srSpeed, Gather::s_srSolar );
    result.tr = sPCC( Gather::s_trTemp, Gather::s_trSolar );
    result.stPairs = Gather::s_stSpeed.Size();
    result.srPairs = Gather::s_srSpeed.Size();
    result.trPairs = Gather::s_trTemp.Size();
    return result;
}
