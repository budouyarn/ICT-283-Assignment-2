/**
 * @file TestMap.cpp
 * @brief Simple test of the Map class: Insert, Find, Delete and Keys, and a Map holding Bst trees.
 *
 * To run the same tests on the Vector-based Map, change the Map.h include below to MapVectorVersion.h.
 */
#include <iostream>
#include <string>
#include "Map.h"
#include "Bst.h"

using std::cout;
using std::endl;
using std::string;

static int failed = 0;///< number of checks that did not hold

/**
 * @brief Prints FAIL when a check does not hold.
 * @param condition: true if the check holds
 * @param what: description of the check
 */
static void check( bool condition, const string & what )
{
    if (!condition)
    {
        failed++;
        cout << "FAIL: " << what << endl;
    }
}

static int visited = 0;///< a function pointer cannot carry data, so the count is kept here

/**
 * @brief Call-back for the Bst traversal: counts how many values were visited.
 * @param item: the value being visited (not used)
 */
static void countValue( int & item )
{
    (void)item;// the value itself is not needed
    visited++;
}

/**
 * @brief Runs the checks.
 * @return 0 if every check held, 1 otherwise
 */
int main()
{
    // ===== empty Map =====
    Map<int, string> names;
    check( names.Keys().Size() == 0, "new Map has no keys" );
    check( names.Find( 1 ) == nullptr, "Find in an empty Map gives nullptr" );
    check( !names.Delete( 1 ), "Delete in an empty Map gives false" );

    // ===== Insert and Find =====
    check( names.Insert( 2015, "fifteen" ), "Insert 2015" );
    check( names.Insert( 2014, "fourteen" ), "Insert 2014" );
    check( names.Keys().Size() == 2, "2 keys after 2 inserts" );
    check( !names.Insert( 2015, "again" ), "a repeated key is refused" );
    check( *names.Find( 2015 ) == "fifteen", "the first value is kept" );
    check( names.Find( 1999 ) == nullptr, "a key never inserted gives nullptr" );

    // ===== change a value through the pointer from Find =====
    *names.Find( 2014 ) = "changed";
    check( *names.Find( 2014 ) == "changed", "value changed through the pointer" );

    // ===== Keys come back in ascending order =====
    Vector<int> keys = names.Keys();
    check( keys[0] == 2014 && keys[1] == 2015, "Keys are ascending" );

    // ===== Delete =====
    check( names.Delete( 2015 ), "Delete 2015" );
    check( names.Find( 2015 ) == nullptr, "a deleted key is gone" );
    check( !names.Delete( 2015 ), "deleting it again gives false" );
    check( names.Keys().Size() == 1, "1 key left" );

    // ===== copy is independent of the original =====
    Map<int, string> copy( names );
    *copy.Find( 2014 ) = "copy only";
    check( *names.Find( 2014 ) == "changed", "changing the copy does not change the original" );

    // ===== a Map whose values are Bst trees (how the weather programme uses it) =====
    Map<int, Bst<int> > trees;
    trees.Insert( 201501, Bst<int>() );
    trees.Find( 201501 )->insert( 5 );
    trees.Find( 201501 )->insert( 3 );
    trees.Find( 201501 )->insert( 8 );
    check( trees.Find( 201501 )->treeNodeCount() == 3, "3 values kept in the tree inside the Map" );

    visited = 0;
    trees.Find( 201501 )->inorderTraversal( countValue );
    check( visited == 3, "the traversal visits all 3 values" );

    if (failed == 0)
    {
        cout << "All checks passed" << endl;
        return 0;
    }
    cout << failed << " check(s) failed" << endl;
    return 1;
}