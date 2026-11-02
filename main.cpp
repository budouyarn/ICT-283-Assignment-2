/**
 * @file main.cpp
 * @brief Assignment 2 (progress version): loads every data file named in data/data_source.txt,
 * arranges the readings in a Map of year trees, and runs the weather menu.
 * @author  Lim Wen Xin Sherine
 * @version 2.0
 * @date    01/11/2026
 */
#include <iostream>
#include <fstream>
#include <string>
#include "Metlog.h"
#include "DataSource2.h"
#include "MetStore.h"
#include "Menu.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::string;

int main()
{
    Metlog metLog;// holds the readings of every file

    // the data files' names come from data_source.txt only
    DataSource2 dataSource;
    if( !dataSource.ReadSource() ) {
        cout << "ERROR: could not read a file name from " << dataSource.GetSourceFilePath() << endl;
        return -1;
    }

    for (int counter = 0; counter < dataSource.GetFileCount(); counter++)
    {
        string dataFilePath = dataSource.GetDataFilePath( counter );

        ifstream infile( dataFilePath );
        if( !infile ) {
            cout << "ERROR: could not open file " << dataFilePath << endl;
            return -1;
        }

        Metlog fileLog; // this file's readings only
        try
        {
            infile >> fileLog;
        }
        catch (const std::exception & error)
        {
            cout << "ERROR: " << dataFilePath << ": " << error.what() << endl;
            return -1;
        }
        infile.close();

        metLog.Merge( fileLog );
        cout << dataFilePath << ": " << fileLog.GetRecordCount() << " readings loaded" << endl;
    }

    MetStore store;
    store.Load( metLog ); // sorts, then builds one balanced tree per year
    cout << metLog.GetRecordCount() << " readings read, " << store.GetReadingCount()
         << " different readings kept in " << store.GetYearCount() << " year tree(s)" << endl;

    Menu menu;
    menu.Run( metLog, store );
    return 0;
}
