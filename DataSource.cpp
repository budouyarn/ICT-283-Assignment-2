#include "DataSource.h"
#include "Date.h"
#include <fstream>
#include <sstream>
#include<vector>

using std::ifstream;
using std::istringstream;

DataSource::DataSource()
{
    m_sourceFilePath= "data/data_source.txt";// the file that holds the data files' names
    m_dataFolder="data/";// where the data files live
    m_dataFilePaths = vector<string>(); // none known until ReadSource() runs

}

// Turns one line such as "3/01/2014" into a Date. Returns false (and leaves date untouched) if the line is not D/M/YYYY.
bool DataSource::ParseDate( const string & line, Date & date )
{
    istringstream lineStream( line );  // the line becomes its own small stream
    int day, month, year;
    char slash1, slash2;// initiate which are the seperators

    lineStream >> day >> slash1 >> month >> slash2 >> year;

    if (lineStream.fail() || slash1 != '/' || slash2 != '/')  // a number was missing, or the separators are wrong
        return false;

    // only touch the Date once the whole line has been read correctly
    date.SetDay( day );
    date.SetMonth( month );
    date.SetYear( year );
    return true;
}

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


int DataSource::GetFileCount() const
{
    return m_dataFilePaths.size(); // retrieve how many data file name readsource found
}


string DataSource::GetDataFilePath( int index ) const
{
    return m_dataFilePaths[index];// retrieve path of one data file ReadSource() found.
}


string DataSource::GetSourceFilePath() const
{
    return m_sourceFilePath;//retrieve location of data_source.txt
}
