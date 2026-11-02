/**
 *
 * Application to keep track of date recorded for each log in the .csv file
 * @class Date
 * @author  Lim Wen Xin Sherine
 * @version 3.0
 * @date 24/10/2026
 *
 */

#include "Date.h"
#include<iostream>

using namespace std;

/**@brief < Default Constructor, Initialisation of variables value */
Date::Date(){
    m_day = 0;
    m_month = 0;
    m_year = 0;
}

 /** @brief Parameterized Constructor that initializes all attributes
     *
     * @param day: indicates the day of the month
     * @param month:indicates the month of the year
     * @param year: indicates the year fom the Full Date
     */
Date::Date(int day, int month, int year){
    m_day=day;
    m_month=month;
    m_year=year;
}

    /** @brief Retrieves the day of the month
     *   @return Day value stored in this Date(in m_day)
     */
int Date::GetDay() const{
    return m_day;
}
    /** @brief Retrieves the Month of the Year
     *   @return Month value stored in this Date(in m_month)
     */
int Date::GetMonth() const{
    return m_month;
    }

    /** @brief Retrieves the Year of the Date
     *   @return Year value stored in this Date(in m_year)
     */
int Date::GetYear() const{
    return m_year;
}
//--------------Setters
    /** @brief Sets the Day of the month in day parameter
     *   @return void
     */
void Date::SetDay( int day ){
    m_day=day;
}
    /** @brief Sets the Month of the year in month parameter
     *  @return void
     */
void Date::SetMonth( int month ){
    m_month=month;
}

    /** @brief Sets the Year of the Date in year parameter
     *  @return void
     */
void Date::SetYear( int year ){
    m_year=year;
}
/** @brief checks for duplication of records holding the same day, month,year
 *  @param  other:the date to compare with this one
 *  @return true if the hour and the minute both match, false otherwise
 */
bool Date::Equals( const Date & other ) const
{
    if (m_day != other.m_day)
        return false;// the days differ

    if (m_month != other.m_month)
        return false;// the months differ

    if (m_year != other.m_year)
       return false; // the years differ

    return true;// all three match
}
/** @brief ostream operations for Writing to Output File
 *@param os: refers to the output stream to write
 * @param date: Date class object containing information of the current Date
 * @return Reference output stream ,allowing << output chain calls
 */
ostream & operator<<( ostream & os, const Date & date )
{   //CURRENT CHANGE
    //Since now that it is needed to determine where the delimiter is
    int day = date.GetDay();
    int month = date.GetMonth();

    //
    if (day < 10) // this is intentionally made to differentiate the single digit days like "09" than the double digit days "15"
        os << '0' << day; // initialisation before writing over
    else
        os << day;

    os << '/';

    if (month < 10)
        os << '0' << month;
    else
        os << month;

    os << '/' << date.GetYear();
    return os;


    // PREVIOUS IMPLEMENTATION
    //os << date.GetDay() << '/' << date.GetMonth() << '/' << date.GetYear();// Output Streams takes the information retrieved from the Getters
    // '/' is added so that when it is printed, they do not end up like 16082026, but 16/08/2026
    return os;
}

/** @brief Input Stream for reading Input File
 * @param input: refers to the stream that it reads from
 * @param date: Date class object containing information of the current Date
 * @return Reference input stream allowing >> input chain calls
 *
 */
istream & operator>>( istream & input, Date & date )
{
    //CURRENT CHANGE
     // The date now being read with a single slash-separated token, like "12/09/2026",
    // as the last field on its line. Read it whole as a string first, then
    // parse it into day/month/year and convert each piece to int.
    string tempStr; // temporary holder
    getline(input, tempStr);  // reads to end of line, Date is still the last field

    // find(): Finds the first occurrence of a character or substring
    char firstSlash = tempStr.find('/'); // tempStr.find is a function from std::string
    char secondSlash = tempStr.find('/', firstSlash + 1);

    //substr(): Returns a portion of the string, starting at a given index and length
    string dayStr = tempStr.substr(0, firstSlash);// 0 signifies the first index, where the day will be sitting at
    string monthStr = tempStr.substr(firstSlash + 1, secondSlash - firstSlash - 1); //firstslash+1 makes sures it moves on over after the first slash, secondslash  -1 is to ensure that the index before seconslash eblongs to the month
    //where in this case, substr gets the finder to find the secondslash, and after the secondslash, it behaves where it returns what was after the second slash only
    string yearStr  = tempStr.substr(secondSlash + 1);

    date.SetDay(stoi(dayStr));
    date.SetMonth(stoi(monthStr));
    date.SetYear(stoi(yearStr));

    return input;
}
bool operator<( const Date & left, const Date & right )
{
    if (left.GetYear() != right.GetYear()) // the year gets checked first
        return left.GetYear() < right.GetYear();

    if (left.GetMonth() != right.GetMonth()) // then the month gets checked
        return left.GetMonth() < right.GetMonth();

    return left.GetDay() < right.GetDay();
    // then the day will not need to check for duplicate as now the year and month is different, even if day is same, but they fall in different month
}
