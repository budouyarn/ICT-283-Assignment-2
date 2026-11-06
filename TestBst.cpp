/**
 * @file TestBst.cpp
 * @brief Simple test of the template Bst, using Bst<int>, with function pointers in the traversals.
 */
#include <iostream>
#include <string>
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

// ---- functions the client passes to the traversals (a function pointer cannot carry data, so statics are used) ----
static int seen[20];///< the values visited, in order
static int seenCount = 0;///< how many values have been visited
static int total = 0;///< sum of the values visited

/** @brief Call-back: remembers each value in the order it is visited. */
static void collect( int & item )
{
    seen[seenCount] = item;
    seenCount++;
}

/** @brief Call-back: adds each value to total. */
static void addToTotal( int & item )
{
    total += item;
}

/**
 * @brief Compares what the last traversal collected with the expected sequence.
 * @param expected: the expected values, in order
 * @param n: how many values are expected
 * @return true if the same n values were visited in the same order
 */
static bool sameAsSeen( const int * expected, int n )
{
    if (seenCount != n)
        return false;
    for (int counter = 0; counter < n; counter++)
        if (seen[counter] != expected[counter])
            return false;
    return true;
}

// ---- passing the tree to functions ----
/** @brief The tree is copied, so the caller's tree is not changed. */
static void addByValue( Bst<int> tree ) { tree.insert( 999 ); }
/** @brief The caller's own tree is changed. */
static void addByReference( Bst<int> & tree ) { tree.insert( 999 ); }

/**
 * @brief Runs the checks.
 * @return 0 if every check held, 1 otherwise
 */
int main()
{
    // ===== empty tree =====
    Bst<int> tree;
    check( tree.isEmpty(), "new tree is empty" );
    check( tree.treeNodeCount() == 0, "empty tree has 0 nodes" );
    check( !tree.search( 5 ), "search in an empty tree is false" );
    check( tree.find( 5 ) == nullptr, "find in an empty tree is nullptr" );

    // check insert, duplicates, search, find
    const int values[] = { 50, 30, 70, 20, 40, 60, 80 };
    for (int counter = 0; counter < 7; counter++)
        check( tree.insert( values[counter] ), "insert returns true for a new value" );
    check( tree.isValidBst(), "tree is a valid BST after the inserts" );
    check( tree.treeNodeCount() == 7, "7 nodes" );
    check( !tree.insert( 30 ), "a duplicate is refused" );
    check( tree.treeNodeCount() == 7, "still 7 nodes after the duplicate" );
    check( tree.search( 40 ) && !tree.search( 99 ), "search finds 40 and not 99" );
    check( tree.find( 40 ) != nullptr && *tree.find( 40 ) == 40, "find gives a pointer to the stored 40" );
    check( tree.find( 99 ) == nullptr, "find gives nullptr for 99" );

    //traversals with function pointers
    const int inorder[] = { 20, 30, 40, 50, 60, 70, 80 };
    const int preorder[] = { 50, 30, 20, 40, 70, 60, 80 };
    const int postorder[] = { 20, 40, 30, 60, 80, 70, 50 };

    seenCount = 0;
    tree.inorderTraversal( collect );
    check( sameAsSeen( inorder, 7 ), "in-order gives ascending order" );

    seenCount = 0;
    tree.preorderTraversal( collect );
    check( sameAsSeen( preorder, 7 ), "pre-order: node, left, right" );

    seenCount = 0;
    tree.postorderTraversal( collect );
    check( sameAsSeen( postorder, 7 ), "post-order: left, right, node" );

    total = 0;
    tree.inorderTraversal( addToTotal );
    check( total == 350, "a different function, same traversal: the values add up to 350" );

    void (*chosen)( int & ) = addToTotal;  // a variable that holds a function pointer
    total = 0;
    tree.preorderTraversal( chosen );
    check( total == 350, "a function pointer held in a variable works too" );

    // ===== passing the tree to functions =====
    addByValue( tree );
    check( tree.treeNodeCount() == 7, "by value: the caller's tree is unchanged" );
    addByReference( tree );
    check( tree.treeNodeCount() == 8 && tree.search( 999 ), "by reference: the caller's tree grew" );

    // ===== copy constructor and assignment are independent of the original =====
    Bst<int> copy( tree );
    copy.insert( 1 );
    check( copy.search( 1 ) && !tree.search( 1 ), "copy: changing the copy does not change the original" );

    Bst<int> assigned;
    assigned = tree;
    assigned.insert( 2 );
    check( assigned.search( 2 ) && !tree.search( 2 ), "assignment: changing the copy does not change the original" );

    // ===== destroyTree and reuse =====
    tree.destroyTree();
    check( tree.isEmpty() && tree.treeNodeCount() == 0, "destroyTree empties the tree" );
    check( copy.treeNodeCount() == 9, "the copy is still intact after the original is destroyed" );
    check( tree.insert( 7 ) && tree.treeNodeCount() == 1, "the tree can be used again" );

   if (failed == 0)
    {
        cout << "All checks passed" << endl;
        return 0;
    }
    cout << failed << " check(s) failed" << endl;
    return 1;
}
