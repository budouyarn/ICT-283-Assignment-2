/**
 * @file DataSource.cpp
 * @brief Implementation of the DataSource class: reads data_source.txt to find the names of the
 * weather data files, and turns a D/M/YYYY line into a Date.
 * @author  Lim Wen Xin Sherine
 * @version 2.0
 * @date    09/11/2026
 */

#include "DataSource.h"
#include "Date.h"
#include <fstream>
#include <sstream>
#include <vector>

using std::ifstream;
using std::istringstream;

/**
 * @brief Constructor for DataSource that knows where data_source.txt is but has not read it yet.
 *
 * The source file is "data/data_source.txt" and the data files are expected in "data/".
 * No data file paths are known until ReadSource() runs.
 */
DataSource::DataSource()
{
    m_sourceFilePath= "data/data_source.txt";// the file that holds the data files' names
    m_dataFolder="data/";// where the data files live
    m_dataFilePaths = vector<string>(); // none known until ReadSource() runs

}

/**
 * @brief Turns one line such as "3/01/2014" into a Date.
 *
 * The Date is only changed once the whole line has been read correctly.
 *
 * @param line: the text to read, in the form D/M/YYYY
 * @param date: set to the day, month and year read (left untouched if the line is not D/M/YYYY)
 * @return true if the line was read correctly, false if a number is missing or the separators are not '/'
 */
bool DataSource::ParseDate( const string & line, Date & date )
{
    istringstream lineStream( line );  // the line becomes its own small stream
    int day, month, year;
    char slash1, slash2;// the two separators, which should both be '/'

    lineStream >> day >> slash1 >> month >> slash2 >> year;

    if (lineStream.fail() || slash1 != '/' || slash2 != '/')  // a number was missing, or the separators are wrong
        return false;

    // only touch the Date once the whole line has been read correctly
    date.SetDay( day );
    date.SetMonth( month );
    date.SetYear( year );
    return true;
}

/**
 * @brief Reads data_source.txt and remembers the path of every data file listed in it.
 *
 * Each name is read one at a time, so blank lines and spaces are skipped. A name that already
 * contains a '/' or '\' already says which folder the file is in and is used as it is; any other
 * name has the data folder added in front. Anything read by an earlier call is cleared first.
 *
 * @return true if at least one file name was found, false if data_source.txt is missing,cannot be opened, or holds no names
 */
bool DataSource::ReadSource()
{
    m_dataFilePaths.clear(); //clear off garbage before new read

    ifstream sourceFile( m_sourceFilePath );
    if (!sourceFile) // data_source.txt is missing or can't be opened
        return false;

    string name;
    while (sourceFile >> name) // one name at a time, skipping blank lines and spaces
    {
        // a / or \ in the name means it already says which folder the file is in
        bool hasFolder = false;
        for (int counter = 0; counter < static_cast<int>(name.size()); counter++)
            if (name[counter] == '/' || name[counter] == '\\')
                hasFolder = true;

        if (hasFolder)
            m_dataFilePaths.push_back( name ); // already has a folder: use it as it is
        else
            m_dataFilePaths.push_back( m_dataFolder + name ); // add the data folder in front
    }

    return !m_dataFilePaths.empty(); // false if the file held no names
}

/**
 * @brief Tells how many data files ReadSource() found.
 * @return the number of data file paths held (0 before ReadSource() has run)
 */
int DataSource::GetFileCount() const
{
    return m_dataFilePaths.size(); // retrieve how many data file names ReadSource found
}

/**
 * @brief Gives the path of one data file found by ReadSource().
 * @pre index is from 0 to GetFileCount() - 1 (it is not checked)
 * @param index: which file, counting from 0
 * @return the path of that data file, for example "data/MetData-2014.csv"
 */
string DataSource::GetDataFilePath( int index ) const
{
    return m_dataFilePaths[index];// retrieve path of one data file ReadSource() found.
}

/**
 * @brief Gives the location of data_source.txt.
 * @return the path of the source file, "data/data_source.txt" unless it was changed
 */
string DataSource::GetSourceFilePath() const
{
    return m_sourceFilePath;//retrieve location of data_source.txt
}
