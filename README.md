## ICT283 Assignment 2 (Version 3.0): Weather Data Programme

## Includes
Lab 8: Reading from data file using DataSource programme   
Lab 10 Qns D and E : sPCC Calculation, MAD Calculation
Lab 11: Implementation of Menu Option 3 and 4


## Project Objectives:
Reads weather readings (one reading every 10 minutes) from CSV files, then answers questions about them through a Menu:


-**Option 1:** Asks for Average wind speed (km/h) and le standard deviation for one month of one year	

-**Option2:** Asks for Average ambient air temperature (degrees Celsius) and standard deviation for one month of one year.

-**Option 3:** Asks for  Pearson correlation coefficients S_T (wind speed and temperature), S_R (wind speed and solar radiation) and T_R (temperature and solar radiation) for one month, using every year loaded	month.

-**Option 4:** Writes output WindTempSolar.csv: for every month of a year, the average wind speed and average temperature (each with (stdev, mad) next to it) and the total solar radiation in kWh/m2.

-**Option 5:** Wind speed summary of the whole file, and the readings that match the average.

-**Option 0:** Exit	
## ------------------------------------------------------------------

**Example of the file written by Option 4 :**

2015
January,11.22(5.13, 4.13),30.15(2.93, 2.52),152.61
February,11.28(4.98, 4.02),29.39(2.98, 2.56),131.73

A value shows N/A when a month has no valid readings, and the standard deviation shows N/A when there is only one. Option 3 shows "not available" for a coefficient that cannot be calculated (fewer than 2 matching readings, or a value that never varies).

## ---------------------------------------------------------------
## How to run

1) Open Assignment 2.cbp on code::blocks

2) **Run the program from a folder that contains a data folder. The working directory matters:** the program looks for data/data_source.txt.

3) data/data_source.txt lists the CSV file names, one per line. Names without a folder are looked for in data/. The same name may be listed many times (used for the stress test). Example:
   -**MetData-2014.csv**
   -**MetData-2015.csv**
   -**MetData-2016.csv**
Each CSV starts with a header line (WAST,DP,Dta,...,S,SD,SR,Sx,T). 
Wind speed S is in m/s and is shown in km/h. N/A or blank values are treated as missing. Blank lines at the end of a file are fine; badly formed rows are skipped and counted.

4) When it starts the program prints how many readings each file gave and how many different readings were kept.

## How it is built

Map<int cell, Bst<MetRecType>>, where cell = year * 100 + month (March 2015 is 201503). Each (year, month) is one cell and holds that month's readings in a binary search tree, ordered by date and then time. The reasoning, advantages and disadvantages are in
