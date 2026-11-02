#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <string>
#include <vector>
#include "Date.h"

using std::string;
using std::vector;

class DataSource
{
    public:

        DataSource();// default constructor

        bool ReadSource(); // returns false if data_source.txt is missing or holds no file names
        int GetFileCount() const; // how many data files were listed in data_source.txt
        string GetDataFilePath( int index ) const; // path of one data file found by ReadSource
        string GetSourceFilePath() const; // where data_source.txt is

        static bool ParseDate( const string & line, Date & date ); // turns a line like 3/01/2014 into a Date; false if it is not DD/MM/YYYY

    private:
        string m_sourceFilePath;// where data_source.txt is
        string m_dataFolder;// folder that holds the data files
        vector <string> m_dataFilePaths;// path of every data file, set by ReadSource
};

#endif // DATASOURCE_H

