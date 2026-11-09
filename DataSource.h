#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <string>
#include <vector>
#include "Date.h"

using std::string;
using std::vector;

/**
 * @class DataSource
 * @brief Finds the weather data files: reads data_source.txt, which lists the file names,
 * and remembers the path of each data file.
 *
 * The source file is "data/data_source.txt" and the data files are expected in "data/".
 * It also turns a date line such as "3/01/2014" into a Date.
 *
 * @author  Lim Wen Xin Sherine
 * @version 2.0
 * @date 01/11/2026
 */
class DataSource
{
    public:

        /**
         * @brief Default constructor. Knows where data_source.txt is but has not read it yet,
         * so no data file paths are held until ReadSource() runs.
         */
        DataSource();

        /**
         * @brief Reads data_source.txt and remembers the path of every data file listed in it.
         * A name that already contains a '/' or '\' is used as it is.
         * @return true if at least one file name was found, false if data_source.txt is missing
         *  or holds no file names
         */
        bool ReadSource();

        /**
         * @brief Tells how many data files were listed in data_source.txt.
         * @return the number of data file paths held
         */
        int GetFileCount() const;

        /**
         * @brief Gives the path of one data file found by ReadSource().
         * @pre index is from 0 to GetFileCount() - 1 (it is not checked)
         * @param index: which file, counting from 0
         * @return the path of that data file
         */
        string GetDataFilePath( int index ) const;

        /**
         * @brief Gives the location of data_source.txt.
         * @return the path of the source file
         */
        string GetSourceFilePath() const;

        /**
         * @brief Turns a line like 3/01/2014 into a Date.
         * The Date is only changed once the whole line has been read correctly.
         * @param line: the text to read, in the form D/M/YYYY
         * @param date: set to the day, month and year read (left untouched if the line is not D/M/YYYY)
         * @return true if the line was read correctly, false if it is not D/M/YYYY
         */
        static bool ParseDate( const string & line, Date & date );

    private:
        string m_sourceFilePath;// where data_source.txt is
        string m_dataFolder;//folder that holds the data files
        vector <string> m_dataFilePaths;// path of every data file, set by ReadSource
};

#endif // DATASOURCE_H
