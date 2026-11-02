/**
 * @file Metlog.cpp
 * @brief Implementation of the Metlog class: loads a weather CSV file into a
 *  Vector of MetRecType records and calculates wind speed, air
 *  temperature and solar radiation figures from them.
 * @class Metlog
 * @author  Lim Wen Xin Sherine
 * @version 1.0
 * @date 08/10/2026
 */
#include "Metlog.h"
#include <fstream> // ofstream
#include <sstream> // istringstream, to split and read the text of each line
#include <cmath> // sqrt
#include <stdexcept> // runtime_error
#include <string>

#include "Date.h"
#include "Time.h"

using std::ofstream;
using std::istringstream; // reading of WAST
using std::getline;
using std::runtime_error;

/** @brief How many column headers the program needs: WAST, S, T and SR. */
const int REQUIRED_COUNT = 4;// declaring size of Array to hold number of columns

/** @name Month and year filters
 *  The Collect functions treat 0 as "don't filter on this". Real months are 1 to 12.
 *  ANY_MONTH: matches every month
 *  ANY_YEAR :matches every year
 */
const int ANY_MONTH = 0;
const int ANY_YEAR  = 0;


/** @brief The length of one reading in hours. A reading is the average over 10 minutes*/
const double HOURS_PER_READING = 10.0 / 60.0;

/**
 * @brief Splits one CSV line into its comma-separated fields.
 * @param line: the line to split, eg. "1/03/2014 9:00,3.2,17.5,0"
 * @return a Vector holding each field as a string, in the order they appear
 */
static Vector<string> CommaDelim( const string & line )
{
    Vector<string> fields; // stores the fields into the string vector

    istringstream lineStream(line);// reads the entire line in a row
    string field;// declare and initialise field
    int counts = 0;                // how many fields have been added so far

    while (getline(lineStream, field, ',')) // line which is already in the line stream, reads up to the next comma each time
    {
        fields.Insert(counts, field);   // counts is the first free position
        counts++;
    }
    if (!line.empty() && line[line.size() - 1] == ',') // checks whether the line ends with a comma. The last character is at position size() - 1.
        fields.Insert(counts, ""); // a trailing comma means one more, empty field that getline would not return
    return fields;
}

/**
 * @brief Converts text such as "29.56" to a float.
 * @param text the text to convert
 * @param value: receives the number; left untouched if the text is not a number
 * @return true, if the text was a number, false otherwise, so the caller can store the MISSING_ value instead
 */
static bool TryParseFloat( const string & text, float & value )
{
    istringstream numberStream(text); // lets us read the text as if it were typed input
    float number;

    numberStream >> number;
    if (numberStream.fail()) // "N/A", "" and other non-numbers fail here
        return false;

    value = number;
    return true;
}

/**
 * @brief Splits a WAST field such as "1/03/2014 9:00" into a Date and a Time.
 * @param wast: the text of the WAST field
 * @param date: receives the day, month and year; untouched if false is returned
 * @param time: receives the hour and minute; untouched if false is returned
 * @return true, if the text had the shape D/M/YYYY H:MM, false if it was the wrong shape
 */
static bool ParseWast( const string & wast, Date & date, Time & time )
{
    istringstream wastStream(wast); // reads the wast into the object stream

    int day, month, year, hour, minute;
    char slash1, slash2, colon;     // catch the separators so they can be checked

    // >> skips the spaces for us, and the char variables catch the separators
    wastStream >> day >> slash1 >> month >> slash2 >> year >> hour >> colon >> minute;

    if (wastStream.fail() || slash1 != '/' || slash2 != '/' || colon != ':')
        return false; // Determine what is the Failed Stream, where the delimiter was not present at these positions

    // When the delimiters are in the right format, it will parse as intended
    date.SetDay(day);
    date.SetMonth(month);
    date.SetYear(year);
    time.SetHour(hour);
    time.SetMinute(minute);
    return true;
}

/**
 * @brief The headers of the excel this programme looks for.
 * They can be in any order in the file and the file may have other columns too
 * (which are ignored), but each of these has to be present.
 */
static const string REQUIRED_HEADERS[REQUIRED_COUNT] ={"WAST", "S", "T", "SR"};

/**
 * @brief Checks that every required header is in the header row.
 * @param headerFields: the fields of the file's header row
 * @throws std::runtime_error naming the first required header the file doesn't have them
 */
static void CheckHeaders( const Vector<string> & headerFields )
{
    for (int headercounter = 0; headercounter < REQUIRED_COUNT; headercounter++)
    {
        bool found = false;//initialised

        for (int counter = 0; counter < headerFields.Size(); counter++)
        {   // check down the loop to check
            if (headerFields[counter] == REQUIRED_HEADERS[headercounter])
                found = true;
        }
        if (!found) // if still not found, then throw error
            throw runtime_error("Missing column header: " + REQUIRED_HEADERS[headercounter]);
    }
}

/**
 * @brief Searches the header row for a column name.
 * @param headerFields: the fields of the file's header row
 * @param name:the column name to look for, e.g. "SR"
 * @return the position of the column (0 = first column), or -1 if not found
 */
static int FindColumn( const Vector<string> & headerFields, const string & name )
{
    for (int counter = 0; counter < headerFields.Size(); counter++)
    {
        if (headerFields[counter] == name)
            return counter;
    }
    return -1;
}

/**
 * @brief Default constructor. Starts with no records and no skipped rows.
 */
Metlog::Metlog()
{
    m_badRowCount = 0; // initialising the skipped-row count
}

/**
 * @brief Empties the log: removes every record and resets the skipped-row count.
 */
void Metlog::Clear()
{
    m_records = Vector<MetRecType>();//creates a brand new vector
    m_badRowCount = 0;
}

/**
 * @brief Adds one record to the end of the log.
 * @param record: the record to add
 */
void Metlog::AddRecord( const MetRecType & record )
{
    m_records.Insert(m_records.Size(), record);// insert at the end, so the file order is kept
}

/**
 * @brief Counts one more row as skipped because it was malformed.
 */
void Metlog::AddBadRow()
{
    m_badRowCount++;
}

void Metlog::Merge( const Metlog & other )
{
    for (int counter = 0; counter < other.GetRecordCount(); counter++)
        AddRecord( other.GetRecord( counter ) );   // each record goes on the end, so file order is kept
    m_badRowCount += other.m_badRowCount;  // so GetSkipCount() covers every file read
}

/**
 * @brief Reads a whole CSV file from the stream into the Metlog.
 * The columns can be in any order and extra columns are ignored. Missing or
 * non-numeric readings are stored as the MISSING_ values.
 *
 * Wind speed is converted from m/s to km/h as it is read.
 * @param input: the stream to read from, positioned at the start of the file
 * @param metlog: the log to fill; anything it held before is replaced
 * @return the same stream, so reads can be chained call
 * @throws std::runtime_error if a required column header is missing
 */
istream & operator>>( istream & input, Metlog & metlog )
{   //Read and check the header row (the first line)
    string headerLine;
    getline(input, headerLine);
    Vector<string> headerFields = CommaDelim(headerLine);

    CheckHeaders(headerFields); // throws if any required header is missing

    //Find which column holds the date/time, speed, temperature and solar
    int wastIndex  = FindColumn(headerFields, "WAST");
    int speedIndex = FindColumn(headerFields, "S");
    int tempIndex  = FindColumn(headerFields, "T");
    int solarIndex = FindColumn(headerFields, "SR");

    metlog.Clear();// replace anything held from an earlier read
    string rowLine;

    while (getline(input, rowLine))// one data row per pass, until the end of the file
    {
        if (rowLine.empty()) continue; // ignore blank lines

        Vector<string> rowFields = CommaDelim(rowLine);
        if (wastIndex  >= rowFields.Size() || speedIndex >= rowFields.Size() ||
            tempIndex  >= rowFields.Size() || solarIndex >= rowFields.Size())
        {
            metlog.AddBadRow();// the row is too short to hold all four columns
            continue;
        }

        Date recDate;
        Time recTime;
        if (!ParseWast(rowFields[wastIndex], recDate, recTime))
        {
            metlog.AddBadRow();// the date/time text is the wrong shape
            continue;
        }

        // each reading starts as missing and is replaced only if its field is a number
        float value = 0;
        float speed = MISSING_SPEED;
        float temperature = MISSING_TEMP;
        float solar = MISSING_SOLAR;

        if (TryParseFloat(rowFields[speedIndex], value))
            speed = value * 3.6f; // m/s -> km/h

        if (TryParseFloat(rowFields[tempIndex], value))
            temperature = value;// degrees C

        if (TryParseFloat(rowFields[solarIndex], value))
            solar = value;// W/m2

        MetRecType record;// one record per row, filled by its setters
        record.SetDate(recDate);
        record.SetTime(recTime);
        record.SetSpeed(speed);
        record.SetTemperature(temperature);
        record.SetSolarRadiation(solar);
        metlog.AddRecord(record);
    }

    input.clear();   // getline hit the end of the file and flagged the stream; reaching the end is normal, so reset it
    return input;
}

/**
 * @brief Writes every record to a CSV file so the loaded data can be checked.
 * Each line is date,time,speed,temperature,solar, with N/A for any missing reading.
 * @param filename: the name of the file to create
 * @return true: if the file was written, false if it could not be opened
 */
bool Metlog::WriteTestOutput( const string & filename ) const
{
    ofstream outFile(filename);
    if (!outFile)
        return false;

    for (int counter = 0; counter < m_records.Size(); counter++)// size() determine how many has been recorded in the records
    {
        outFile << m_records[counter].GetDate() << ','
                << m_records[counter].GetTime() << ',';

        if (m_records[counter].IsSpeedMissing()) // condition to check if any speed is missing/ offline
            outFile << "N/A";
        else
            outFile << m_records[counter].GetSpeed(); // otherwise continues to record the speed in the records
        outFile << ',';

        if (m_records[counter].IsTempMissing())
            outFile << "N/A";
        else
            outFile << m_records[counter].GetTemperature();
        outFile << ',';

        if (m_records[counter].IsSolarMissing())
            outFile << "N/A";
        else
            outFile << m_records[counter].GetSolarRadiation();
        outFile << '\n';  // end of this record's line
    }
    return true;
}

/**
 * @brief Retrieves how many records the log holds.
 * @return the number of records, including ones with missing readings
 */
int Metlog::GetRecordCount() const
{
    return m_records.Size(); //get how many it has been recorded
}

/**
 * @brief Retrieves one record.
 * @param index: the position of the record (0 to GetRecordCount() - 1)
 * @return a copy of the record at that position
 *
 */
MetRecType Metlog::GetRecord( int index ) const
{
    return m_records[index];// get the actual records sitting at each index
}

/**
 * @brief Retrieves how many rows were skipped as malformed while reading the file.
 * @return the number of skipped rows
 */
int Metlog::GetSkipCount() const
{
    return m_badRowCount;
}

/**
 * @brief Counts the readings in the whole file that have a valid wind speed.
 * @return the number of valid wind speed readings
 */
int Metlog::GetValidCount() const
{
    return CollectSpeeds(ANY_MONTH, ANY_YEAR).Size();
}

// private helpers shared by the whole-file and the month/year calculations

/**
 * @brief Tests whether a record falls in the wanted month and year.
 * @param record: the record to test
 * @param month: the month between 1-12
 * @param year: Any year input
 * @return true if the record is in the period, false otherwise
 */
bool Metlog::IsInPeriod( const MetRecType & record, int month, int year )
{
    if (month != ANY_MONTH && record.GetDate().GetMonth() != month)
        return false;  // wrong month
    if (year != ANY_YEAR && record.GetDate().GetYear() != year)
        return false; // wrong year
    return true;
}

/**
 * @brief Gathers the valid wind speeds (km/h) in a month and year.
 * The three Collect functions differ only in which reading they pick up.
 * Records whose sensor was offline are left out, so they can't drag a result
 * toward the missing placeholder values.
 * @param  month: the month between 1-12
 * @param year: Any year input
 * @return the speeds, in file order
 */
Vector<double> Metlog::CollectSpeeds( int month, int year ) const
{
    Vector<double> values;
    for (int counter = 0; counter < m_records.Size(); counter++)
    {
        if (IsInPeriod(m_records[counter], month, year) && !m_records[counter].IsSpeedMissing())
            values.Insert(values.Size(), m_records[counter].GetSpeed());
    }
    return values;
}

/**
 * @brief Gathers the valid ambient air temperatures (degrees C) in a month and year.
 * @param  month: the month between 1-12
 * @param year: Any year input
 * @return the temperatures, in file order
 */
Vector<double> Metlog::CollectTemperatures( int month, int year ) const
{
    Vector<double> values;
    for (int counter = 0; counter < m_records.Size(); counter++)
    {
        if (IsInPeriod(m_records[counter], month, year) && !m_records[counter].IsTempMissing())
            values.Insert(values.Size(), m_records[counter].GetTemperature());
    }
    return values;
}

/**
 * @brief Gathers the valid solar radiation readings (W/m2) in a month and year.
 * @param month: the month between 1-12
 * @param year: Any year input
 * @return the solar radiation readings, in file order
 */
Vector<double> Metlog::CollectSolar( int month, int year ) const
{
    Vector<double> values;
    for (int counter = 0; counter < m_records.Size(); counter++)
    {
        if (IsInPeriod(m_records[counter], month, year) && !m_records[counter].IsSolarMissing())
            values.Insert(values.Size(), m_records[counter].GetSolarRadiation());
    }
    return values;
}

/**
 * @brief Calculates the mean of a set of values.
 * Sums in double: over tens of thousands of readings a float running total
 * would lose precision as it grows.
 * @param values:the values to average
 * @return the mean, or 0 if there are no values
 */
double Metlog::MeanOf( const Vector<double> & values )
{
    if (values.Size() == 0)
        return 0;// avoids dividing by zero

    double sum = 0;
    for (int counter = 0; counter < values.Size(); counter++)
        sum += values[counter];
    return sum / values.Size();
}

/**
 * @brief Calculates the standard deviation of a set of values.
 * Divides by n - 1.
 * @param values: the values to measure
 * @return the  standard deviation, or 0 if there are fewer than 2 values
 */
double Metlog::StdDevOf( const Vector<double> & values )
{
    if (values.Size() < 2)
        return 0;                           // n - 1 would be 0 or negative

    double mean = MeanOf(values);
    double squaredDiffs = 0;                // running total of (value - mean) squared
    for (int counter = 0; counter < values.Size(); counter++)
    {
        double diff = values[counter] - mean;
        squaredDiffs += diff * diff;
    }
    return sqrt(squaredDiffs / (values.Size() - 1));
}

/**
 * @brief Calculates the mean wind speed (km/h) over the whole file.
 * @return the mean, or 0 if there are no valid readings
 */
double Metlog::CalculateMean() const
{
    return MeanOf(CollectSpeeds(ANY_MONTH, ANY_YEAR));
}

/**
 * @brief Calculates the sample standard deviation of wind speed (km/h) over the whole file.
 * @return the standard deviation, or 0 if there are fewer than 2 valid readings
 */
double Metlog::CalculateStdDev() const
{
    return StdDevOf(CollectSpeeds(ANY_MONTH, ANY_YEAR));
}


/**
 * @brief Counts the valid wind speed readings in a month and year.
 * @param month: the month between 1-12
 * @param month: the month between 1-12
 * @return the number of valid readings
 */
int Metlog::GetSpeedCount( int month, int year ) const
{
    return CollectSpeeds(month, year).Size();
}

/**
 * @brief Calculates the mean wind speed (km/h) in a month and year.
 * @param month: the month between 1-12
 * @param year: the year wanted, or ANY_YEAR
 * @return the mean, or 0 if there are no valid readings
 */
double Metlog::GetSpeedMean( int month, int year ) const
{
    return MeanOf(CollectSpeeds(month, year));
}

/**
 * @brief Calculates the standard deviation of wind speed (km/h) in a month and year.
 * @param month: the month between 1-12
 * @param year: the year wanted, or ANY_YEAR
 * @return the standard deviation, or 0 if there are fewer than 2 valid readings
 */
double Metlog::GetSpeedStdDev( int month, int year ) const
{
    return StdDevOf(CollectSpeeds(month, year));
}

/**
 * @brief Counts the valid ambient air temperature readings in a month and year.
 * @param month: the month between 1-12
 * @param year: the year wanted, or ANY_YEAR
 * @return the number of valid readings
 */
int Metlog::GetTemperatureCount( int month, int year ) const
{
    return CollectTemperatures(month, year).Size();
}

/**
 * @brief Calculates the mean ambient air temperature (degrees C) in a month and year.
 * @param month:the month wanted (1-12), or ANY_MONTH
 * @param year:the year wanted, or ANY_YEAR
 * @return the mean, or 0 if there are no valid readings
 */
double Metlog::GetTemperatureMean( int month, int year ) const
{
    return MeanOf(CollectTemperatures(month, year));
}

/**
 * @brief Calculates the sample standard deviation of air temperature (degrees C) in a month and year.
 * @param month: the month between 1-12
 * @param year:the year wanted, or ANY_YEAR
 * @return the standard deviation, or 0 if there are fewer than 2 valid readings
 */
double Metlog::GetTemperatureStdDev( int month, int year ) const
{
    return StdDevOf(CollectTemperatures(month, year));
}

/**
 * @brief Counts the valid solar radiation readings in a month and year.
 * @param month: the month between 1-12
 * @param year: the year wanted, or ANY_YEAR
 * @return number of valid readings
 */
int Metlog::GetSolarCount( int month, int year ) const
{
    return CollectSolar(month, year).Size();
}

/**
 * @brief Calculates the total solar radiation (kWh/m2) in a month and year.
 * Each reading is a W/m2 average over 10 minutes, so the total is the sum of
 * the readings times the hours per reading, divided by 1000.
 * @param month: the month between 1-12
 * @param year:the year wanted, or ANY_YEAR
 * @return the total solar radiation, or 0 if there are no valid readings
 */
double Metlog::GetSolarTotal( int month, int year ) const
{
    Vector<double> readings = CollectSolar(month, year);

    double sum = 0; // W/m2, summed over the readings
    for (int counter = 0; counter < readings.Size(); counter++)
        sum += readings[counter];

    return sum * HOURS_PER_READING / 1000;// W/m2 x hours / 1000 = kWh/m2
}

/**
 * @brief Finds every reading whose wind speed matches the whole-file mean.
 * @param tolerance:how far (km/h) a speed may be from the mean and still match
 * @return the matching records, in file order (empty if none match)
 */
Vector<MetRecType> Metlog::FindMatchesToMean( double tolerance ) const
{
    Vector<MetRecType> matches;// starts empty; filled below
    double mean = CalculateMean();

    for (int counter = 0; counter < m_records.Size(); counter++)
    {
        if (m_records[counter].IsSpeedMissing())
            continue; // an offline sensor can't match anything

        // "matches" means close enough: the gap between the two values is
        // within the tolerance, never an exact == between two floating point numbers
        if (fabs(m_records[counter].GetSpeed() - mean) <= tolerance)
            matches.Insert(matches.Size(), m_records[counter]);
    }
    return matches;
}

/**
 * @brief Retrieves the mean wind speed (km/h) over the whole file.
 * @return the mean, or 0 if there are no valid readings
 */
double Metlog::GetMean() const
{
    return CalculateMean();
}

/**
 * @brief Retrieves the sample standard deviation of wind speed (km/h) over the whole file.
 * @return the standard deviation, or 0 if there are fewer than 2 valid readings
 */
double Metlog::GetStdDev() const
{
    return CalculateStdDev();
}
