#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "Bst.h"
#include "Date.h"


using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::istringstream;
using std::invalid_argument;

// The call-back for the traversals: the tree calls it once for every date, in traversal order.
// Printing is the client's job, so the tree does not need operator<< for Date.
void printDate( Date & item )
{
    cout << item << " ";
}

// The copy constructor gives the function its own copy of the tree, so changes made here
// do not affect the original tree.
void addByValue( Bst<Date> tree )
{
    tree.insert( Date( 1, 1, 9999 ) ); // a date that is not in date.txt, so it is accepted
    cout << "Inside byValue: " << tree.treeNodeCount() << " dates" << endl;
}

// anything done is permanent
void addByReference( Bst<Date> & tree )
{
    tree.insert( Date( 1, 1, 9999 ) );
    cout << "Inside byReference: " << tree.treeNodeCount() << " dates" << endl;
}

// for reading only
void showTree( const Bst<Date> & tree )
{
    cout << "In-order: ";
    tree.inorderTraversal( printDate );
    cout << endl;
}

// Searches for one date and prints whether it was found
void showSearch( const Bst<Date> & tree, const Date & searchItem )
{
    cout << "Search " << searchItem << ": ";
    if (tree.search( searchItem ))
        cout << "found" << endl;
    else
        cout << "not found" << endl;
}

int main()
{
    Bst<Date> dateTree; // Template tree

    // read the file of dates and insert every date
    string file = "date_data.txt";// one date per line, like 15/06/2016

    ifstream infile( file );// open the file of dates
    if( !infile ) {
        cout << "ERROR: could not open file " << file << endl;
        return -1;
    }
    cout << "File opened successfully: " << file << endl;

    string line;
    while (getline( infile, line )) // one whole line at a time
    {
        if (line.empty())
            continue; // skip a blank line

        istringstream lineStream( line );// the line becomes its own small stream
        Date date;
        try
        {
            lineStream >> date; //Date operator, reads D/M/YYYY
        }
        catch (const invalid_argument & error)
        {
            cout << "Skipped \"" << line << "\": " << error.what() << endl; // not a date, go on to the next line
            continue;
        }

        if (dateTree.insert( date )) // if it finds a new date, insert
            cout << "Inserted " << date << endl;
        else
            cout << date << " is already in the tree, not inserted again" << endl; // check for duplicates
    }
    infile.close(); // close the file once reading is complete

    cout << "Tree holds " << dateTree.treeNodeCount() << " dates" << endl;
    if (dateTree.isValidBst())
        cout << "The tree follows the BST rules" << endl;
    else
        cout << "ERROR: the tree broke the BST rules" << endl;

    // Traversals
    cout << endl << "In-order: ";
    dateTree.inorderTraversal( printDate );
    cout << endl;

    cout << "Pre-order: ";
    dateTree.preorderTraversal( printDate );
    cout << endl;

    cout << "Post-order: ";
    dateTree.postorderTraversal( printDate );
    cout << endl;

    // Search test: one date that is in the tree and one that is not
    cout << endl << "[Search Test]" << endl;
    showSearch( dateTree, Date( 1, 3, 2015 ) );   // in the tree
    showSearch( dateTree, Date( 2, 3, 2015 ) );   // not in the tree

    // passing the tree to functions
    cout << endl << "Pass by value (Called tree must stay at " << dateTree.treeNodeCount() << "):" << endl;
    addByValue( dateTree );
    cout << "Result: " << dateTree.treeNodeCount() << " dates" << endl;

    cout << "Pass by reference (Called tree must grow by 1):" << endl;
    addByReference( dateTree );
    cout << "Result: " << dateTree.treeNodeCount() << " dates" << endl;

    cout << "Pass by const reference:" << endl;
    showTree( dateTree );

    // copy constructor, copy assignment, destroyTree
    Bst<Date> copied( dateTree ); // copy constructor
    Bst<Date> assigned;// starts empty
    assigned = dateTree; // copy assignment
    dateTree.destroyTree();// the original is emptied, the copies keep their own nodes
    cout << endl << "After destroyTree on the original: original " << dateTree.treeNodeCount()
         << " dates, copy " << copied.treeNodeCount() << ", assigned " << assigned.treeNodeCount() << endl;

    cout << endl << "[Check for Empty tree]" << endl;
    if (dateTree.isEmpty())
        cout << "Empty?: YES" << endl;
    else
        cout << "Empty?: NO" << endl;

    if (dateTree.isValidBst())
        cout << "Valid BST?: YES" << endl;
    else
        cout << "Valid BST?: NO" << endl;

    return 0;
}
