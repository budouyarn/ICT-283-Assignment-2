// copied over from Registration programme
// using istream and ostream as Date and Time has to be read together, rather than being split through the sstream
/**
 *
 * Application to keep track of date recorded in the csv file
 * @class Date
 * @author  Lim Wen Xin Sherine
 * @version 2.0
 * @date 24/10/2026
 *
 */
#ifndef DATE_H
#define DATE_H

#include<iostream>

using std::istream;
using std::ostream;

class Date {


public:
    /** @brief Default Constructor
     */
    Date(); // default constructor
//------------------------------------------------
    /** @brief Parameterized Constructor that initializes all attributes
     *
     * @param day: indicates the day of the month
     * @param month:indicates the month of the year
     * @param year: indicates the year fom the Full Date
     */
    Date( int day, int month, int year );// parameterized constructor

     /** @brief Retrieves the day of the month
     *   @return Day value stored in this Date
     */
    int GetDay() const;

    /** @brief Retrieves the Month of the Year
     *   @return Month value stored in this Date
     */
    int GetMonth() const;

    /** @brief Retrieves the Year of the Date
     *   @return Year value stored in this Date
     */
    int GetYear() const;

//------------Setters
    /** @brief Sets the Day of the month in day parameter
     *   @return void
     */
    void SetDay( int day );

    /** @brief Sets the Month of the year in month parameter
     *   @return void
     */
    void SetMonth( int month );

    /** @brief Sets the Year of the Date in year parameter
     *   @return void
     */
    void SetYear( int year );

//-----------behaviour
    /** @brief returns true if day, month and year all match
     *
     */
bool Equals( const Date & other ) const;

private:
    int m_day;/**< stores day value */
    int m_month;/**< store month value */
    int m_year;/**< store year value */
};

/** @brief ostream operations for Writing to Output File
 *@param os: refers to the output stream to write
 * @param date: Date class object containing information of the current Date
 * @return Reference output stream ,allowing << output chain calls
 */
ostream & operator<<( ostream & os, const Date & date );

/** @brief Input Stream for reading Input File
 * @param input: refers to the stream that it reads from
 * @param date: Date class object containing information of the current Date
 * @return Reference input stream allowing >> input chain calls
 *
 */
istream & operator>>( istream & input, Date & date );

/**
 * @brief Compares two dates so a Date can be placed in a binary search tree.
 * The year is compared first, then the month, then the day.
 * @param left: the first date
 * @param right: the second date
 * @return true if left comes before right, false otherwise (also false when they are equal)
 */
bool operator<( const Date & left, const Date & right );

#endif
