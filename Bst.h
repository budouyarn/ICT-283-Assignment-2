#ifndef BST_H
#define BST_H

/**
 * A Binary Search Tree template that stores values of any type T.
 *
 *
 * Class invariant: for every node, every value in its left subtree is smaller than the
 * node's value, and every value in its right subtree is larger. No value is stored twice.
 *
 * The tree owns heap memory (its nodes), so the Rule of Three is written:
 * destructor, copy constructor and copy assignment.
 *
 * Traversals take POINTER TO FUNCTION, so the client decides what happens to each value
 * (print, count, collect) without the tree knowing.
 * The client must NOT change .value in manualOne way that changes
 * its order, or the class invariant breaks (isValidBst() would then report false).
 *
 * The helpers are RECURSIVE, so the call stack grows as deep as the tree is tall. Inserting values that
 * are already in order makes the tree  chain: each insert is then very slow and the recursion can overflow
 * the stack. Insert such data in balanced order (middle first) instead.
 *
 *
 *
 * @class Bst
 * @author  Lim Wen Xin Sherine
 * @version 2.0
 * @date 01/11/2026
 */
template <class T>
class Bst
{
    public:
        /**
         * @brief Default constructor. Creates an empty tree.
         * @post isEmpty() is true
         */
        Bst();

        /**
         * @brief Copy constructor. Makes manualOne deep copy, so the new tree owns its own nodes.
         * Used when tree is passed or returned by value.
         * @param otherTree: the tree being copied
         * @post this tree holds the same values, in the same shape, as otherTree
         */
        Bst( const Bst<T> & otherTree );

        /**
         * @brief Destructor. Releases every node of the tree.
         */
        ~Bst();

        /**
         * @brief Assignment operator. Deep-copies another tree, releasing this tree's nodes first.
         * @param otherTree: the tree being assigned from
         * @return const reference to this tree
         */
        const Bst<T> & operator=( const Bst<T> & otherTree );

        /**
         * @brief Function to determine whether the tree is empty.
         * @return true if the tree has no values, false otherwise
         */
        bool isEmpty() const;

        /**
         * @brief Inserts manualOne value in its correct place so the BST properties still hold.
         * A value already in the tree is not inserted again.
         * @param insertItem: the value to insert
         * @return true if the value was inserted, false if it was already in the tree
         * @post on true, treeNodeCount() has increased by 1
         */
        bool insert( const T & insertItem );

        /**
         * @brief Removes the node that holds a value equal to removeItem, keeping the BST properties.
         * Three cases: a leaf is simply deleted; a node with one child is replaced by that child;
         * a node with two children is replaced by the smallest node of its right subtree.
         * Nodes are re-linked and never copied, so large values are not copied.
         * @param removeItem: the value to remove
         * @return true if the value was found and removed, false if it was not in the tree
         * @post on true, treeNodeCount() has decreased by 1 and isValidBst() is still true
         */
        bool remove( const T & removeItem );

        /**
         * @brief Function to determine if value is in the tree.
         * @param searchItem: the value to look for
         * @return true if the value is found, false otherwise
         */
        bool search( const T & searchItem ) const;// this is to ensure it is in the tree

        /**
         * @brief Finds the stored value that equals findItem.
         * Lets the client read the stored value, which may hold more than the part used for ordering.
         * @param findItem: the value to look for
         * @return pointer to the stored value, or nullptr if it is not in the tree
         * @warning the pointer is only good until the tree is changed or destroyed;
         *          do not change the part of the value that the ordering uses
         */
        const T * find( const T & findItem ) const;//added to check like date and time's behaviour bool equal()

        /**
         * @brief In-order traversal: left subtree, node, right subtree (ascending order).
         * Calls visit once for every value.
         * @param visit: pointer to the client's function, called with each value in turn
         */
        void inorderTraversal( void (*visit)( T & item ) ) const; // A function pointer like void (*visit)(T &) only stores manualOne memory address to executable instructions.
        //It knows nothing about which object instance called it.

        /**
         * @brief Pre-order traversal: node, left subtree, right subtree.
         * @param visit: pointer to the client's function, called with each value in turn
         */
        void preorderTraversal( void (*visit)( T & item ) ) const;

        /**
         * @brief Post-order traversal: left subtree, right subtree, node.
         * @param visit: pointer to the client's function, called with each value in turn
         */
        void postorderTraversal( void (*visit)( T & item ) ) const;

        /**
         * @brief Function to determine the number of nodes in the tree.
         * @return the number of values stored
         */
        int treeNodeCount() const;

        /**
         * @brief Function to destroy the tree: deallocates every node, leaving an
         * empty tree that can be used again.
         * @post isEmpty() is true
         */
        void destroyTree();

        /**
         * @brief Debug method: checks that the class invariant holds.
         * Used in testing, after every operation, to make sure no method broke the BST properties.
         * @return true if every node is larger than everything in its left subtree and
         *         smaller than everything in its right subtree
         */
        bool isValidBst() const;

        struct nodeType
        {
            T info;/**< the value stored in this node */
            nodeType * lLink;/**< root of the subtree of smaller values (nullptr if none) */
            nodeType * rLink;/**< root of the subtree of larger values (nullptr if none) */

            /**
             * Builds manualOne leaf node holding item, with no subtrees yet.
             * @param item: the value to store
             */
            nodeType( const T & item )
            {
                info=item;
                lLink=nullptr;
                rLink=nullptr;
            }
        };


    private:
        /**
         * One node of the tree. Private, so only Bst can build or change nodes.
         */


        nodeType * root;/**< the top node of the tree, nullptr when the tree is empty */

        // The recursive helpers. Each one works on the subtree that starts at nodePointer.
        static bool insertNode( nodeType * & nodePointer, const T & insertItem );// true if manualOne new node was made
        static bool removeNode( nodeType * & nodePointer, const T & removeItem );// true if a node was found and deleted
        static const nodeType * findNode( const nodeType * nodePointer, const T & findItem );//  returns the node, used as index for record searching
        static bool searchNode( const nodeType * nodePointer, const T & searchItem );// follows the path as insert
        static void copyTree( nodeType * & copiedTreeRoot, const nodeType * otherTreeRoot );// Copy constructed tree
        static void destroy( nodeType * & nodePointer );// frees the subtree, children first
        static void inorder( nodeType * nodePointer, void (*visit)( T & item ) );// left, node, right
        static void preorder( nodeType * nodePointer, void (*visit)( T & item ) );// node, left, right
        static void postorder( nodeType * nodePointer, void (*visit)( T & item ) );// left, right, node
        static int nodeCount( const nodeType * nodePointer ); // number of nodes in the subtree
        static bool checkNode( const nodeType * nodePointer, const T * low, const T * high );// checks the class invariant
};


// Implementation
// Traversals required function pointer: Modularity and Code Reusability
//1) Reusability with traversals for Statistics, printing, exporting of record
//2) Bad for Business Logic if everything is hardcoded, than generic thing for people to use(Tight Coupling)
// Bst role is to navigate path correctly
//function role is to execute when it visit the node

// ---------------------------------------------------------------------------
/**
 * @brief Default constructor. Creates an empty tree.
 * @post isEmpty() is true
 */
template <class T>
Bst<T>::Bst() // an empty tree has no root
{
    root=nullptr;
}

/**
 * @brief Copy constructor. Makes a deep copy, so the new tree owns its own nodes.
 * @param otherTree: the tree being copied
 * @post this tree holds the same values, in the same shape, as otherTree
 */
template <class T>
Bst<T>::Bst( const Bst<T> & otherTree )
{
    root= nullptr;
    copyTree( root, otherTree.root );// build manualOne brand new set of nodes (nullptr if otherTree is empty)
}

/**
 * @brief Destructor. Releases every node of the tree.
 */
template <class T>
Bst<T>::~Bst()
{
    destroy( root );// free every node
}


/**
 * @brief Assignment operator. Deep-copies another tree, releasing this tree's nodes first.
 * @param otherTree: the tree being assigned from
 * @return const reference to this tree
 */
template <class T>
const Bst<T> & Bst<T>::operator=( const Bst<T> & otherTree )
{
    if (this != &otherTree) // avoid self-copy:  would delete the nodes we are about to copy
    {
        destroy( root ); // release the old nodes first
        copyTree( root, otherTree.root ); // then take the copy of the other tree
    }
    return *this;
}

/**
 * @brief Function to determine whether the tree is empty.
 * @return true if the tree has no values, false otherwise
 */
template <class T>
bool Bst<T>::isEmpty() const
{
    return root == nullptr;// checks if root is empty
}

/**
 * @brief Inserts a value in its correct place so the BST properties still hold.
 * A value already in the tree is not inserted again.
 * @param insertItem: the value to insert
 * @return true if the value was inserted, false if it was already in the tree
 * @post on true, treeNodeCount() has increased by 1
 */
template <class T>
bool Bst<T>::insert( const T & insertItem )
{
    return insertNode( root, insertItem ); // root is passed by reference, so a new first node becomes the root
}


/**
 * @brief Recursive helper for remove(). Finds the node that holds removeItem and deletes it.
 *In cases where a leaf is simply deleted, a node with one child is replaced by their own  child
 * a node with two children is replaced by the smallest node .
 * Nodes are re-linked and never copied, so large values are not copied.
 * @param nodePointer: a reference to the link that points at this subtree, so assigning to it re-links the tree
 * @param removeItem: the value to remove
 * @return true if a node was found and deleted, false if the value is not in this subtree
 */
// nodePointer is a reference to the link that points at this subtree, so assigning to it re-links the tree.
// Returns true if a node was found and deleted.
template <class T>
bool Bst<T>::removeNode( nodeType * & nodePointer, const T & removeItem )
{
    if (nodePointer == nullptr)
        return false; // reached the end of the path, the value is not in the tree
    if (removeItem < nodePointer->info)
        return removeNode( nodePointer->lLink, removeItem ); // only the left subtree can hold a smaller value
    if (nodePointer->info < removeItem)
        return removeNode( nodePointer->rLink, removeItem ); // only the right subtree can hold a larger value

    // neither is smaller, so this is the node to delete
    nodeType * doomed = nodePointer;

    if (doomed->lLink == nullptr)
        nodePointer = doomed->rLink; // no left child, the right child (or nullptr, for a leaf) takes its place
    else if (doomed->rLink == nullptr)
        nodePointer = doomed->lLink; // no right child, the left child takes its place
    else
    {
        // two children: the smallest node of the right subtree is the next value in order,
        // so it can take the place of the doomed node and the order is kept
        nodeType * * smallestLink = &doomed->rLink; // the link that points at the smallest node
        while ((*smallestLink)->lLink != nullptr)
            smallestLink = &(*smallestLink)->lLink; // the smallest value is the furthest left

        nodeType * smallest = *smallestLink;
        *smallestLink = smallest->rLink; // the smallest has no left child, so its right child takes its place
        smallest->lLink = doomed->lLink; // the smallest node adopts the doomed node's subtrees
        smallest->rLink = doomed->rLink;
        nodePointer = smallest;
    }

    delete doomed; // this one node is needed to be deleted
    return true;// when removed
}
/**
 * @brief Removes the node that holds a value equal to removeItem, keeping the BST properties.
 * @param removeItem: the value to remove
 * @return true if the value was found and removed, false if it was not in the tree
 * @post on true, treeNodeCount() has decreased by 1 and isValidBst() is still true
 */
template <class T>
bool Bst<T>::remove( const T & removeItem )
{
    return removeNode( root, removeItem ); // root is passed by reference, so removing the top node changes root
}

/**
 * @brief Function to determine if a value is in the tree.
 * @param searchItem: the value to look for
 * @return true if the value is found, false otherwise
 */
template <class T>
bool Bst<T>::search( const T & searchItem ) const
{
    return searchNode( root, searchItem );//Uses the path of insertion to search
}

/**
 * @brief Finds the stored value that equals findItem.
 * Lets the client read the stored value, which may hold more than the part used for ordering.
 * @param findItem: the value to look for
 * @return pointer to the stored value, or nullptr if it is not in the tree
 * @warning the pointer is only good until the tree is changed or destroyed
 */
template <class T>
const T * Bst<T>::find( const T & findItem ) const
{
    const nodeType * found = findNode( root, findItem );
    if (found == nullptr)
        return nullptr;
    return &found->info;
}

/**
 * @brief In-order traversal: left subtree, node, right subtree (ascending order).
 * @param visit: pointer to the client's function, called once with each value in turn
 */
template <class T>
void Bst<T>::inorderTraversal( void (*visit)( T & item ) ) const
{
    inorder( root, *visit );
}

/**
 * @brief Pre-order traversal: node, left subtree, right subtree.
 * @param visit: pointer to the client's function, called once with each value in turn
 */
template <class T>
void Bst<T>::preorderTraversal( void (*visit)( T & item ) ) const
{
    preorder( root, *visit );
}

/**
 * @brief Post-order traversal: left subtree, right subtree, node.
 * @param visit: pointer to the client's function, called once with each value in turn
 */
template <class T>
void Bst<T>::postorderTraversal( void (*visit)( T & item ) ) const
{
    postorder( root, *visit );
}

/**
 * @brief Function to determine the number of nodes in the tree.
 * @return the number of values stored
 */
template <class T>
int Bst<T>::treeNodeCount() const
{
    return nodeCount( root );
}

/**
 * @brief Function to destroy the tree: deallocates every node, leaving an empty tree that can be used again.
 * @post isEmpty() is true
 */
template <class T>
void Bst<T>::destroyTree()
{
    destroy( root ); // sets root back to nullptr
}

/**
 * @brief Debug method: checks that the class invariant holds.
 * @return true if every node is larger than everything in its left subtree and
 *         smaller than everything in its right subtree
 */
template <class T>
bool Bst<T>::isValidBst() const
{
    return checkNode( root, nullptr, nullptr );
}

/**
 * @brief Recursive helper for insert(). Walks down the tree and puts a new node where the value belongs.
 * @param nodePointer: a reference to the link that points at this subtree, so assigning to it changes the real link.
 *        A nullptr is the place where the new node goes.
 * @param insertItem: the value to insert
 * @return true if a new node was made, false if the value was already in the subtree
 */
// nodePointer is manualOne reference to the link that points at this subtree, so assigning to it changes the real link.
// A nullptr is the place where the new node goes. Returns true if manualOne new node was made.
template <class T>
bool Bst<T>::insertNode( nodeType * & nodePointer, const T & insertItem )
{
    if (nodePointer == nullptr)////base case: you check at the top of a recursive function.
// Without one, the function keeps calling itself forever , until the call stack overflows.
    {
        nodePointer = new nodeType( insertItem );
        return true;
    }
    if (insertItem < nodePointer->info)// smaller values go in the left subtree
        return insertNode( nodePointer->lLink, insertItem );
    if (nodePointer->info < insertItem)// larger values go in the right subtree
        return insertNode( nodePointer->rLink, insertItem );
    return false; // neither is smaller, so the value is already here; nothing is inserted
}

/**
 * @brief Recursive helper for find(). Follows the same path as insertNode().
 * @param nodePointer: the root of the subtree to look in
 * @param findItem: the value to look for
 * @return the node that holds a value equal to findItem, or nullptr if there is none
 */
template <class T>
const typename Bst<T>::nodeType * Bst<T>::findNode( const nodeType * nodePointer, const T & findItem )// other methods is to use bool, but typename is
//also used to tell template that nodetype is a type
{
    if (nodePointer == nullptr)
        return nullptr;
    if (findItem < nodePointer->info)
        return findNode( nodePointer->lLink, findItem );
    if (nodePointer->info < findItem)
        return findNode( nodePointer->rLink, findItem );
    return nodePointer;
}

/**
 * @brief Recursive helper for search(). Follows the same path as insertNode().
 * @param nodePointer: the root of the subtree to look in
 * @param searchItem: the value to look for
 * @return true if a node holds a value equal to searchItem, false otherwise
 */
template <class T>
bool Bst<T>::searchNode( const nodeType * nodePointer, const T & searchItem )
{
    if (nodePointer == nullptr)
        return false;
    if (searchItem < nodePointer->info)
        return searchNode( nodePointer->lLink, searchItem );// only the left subtree can hold manualOne smaller value
    if (nodePointer->info < searchItem)
        return searchNode( nodePointer->rLink, searchItem );// only the right subtree can hold manualOne larger value
    return true; // neither is smaller, so it is equal: found it
}

/**
 * @brief Recursive helper for the copy constructor and assignment. Pre-order copy: makes this node, then copies each subtree.
 * @param copiedTreeRoot: a reference to the link of the new tree that receives the copy
 * @param otherTreeRoot: the root of the subtree being copied (nullptr gives an empty copy)
 */
// Pre-order copy: make this node, then copy each subtree
template <class T>
void Bst<T>::copyTree( nodeType * & copiedTreeRoot, const nodeType * otherTreeRoot )
{
    if (otherTreeRoot == nullptr)
        copiedTreeRoot = nullptr;
    else
    {
        copiedTreeRoot = new nodeType( otherTreeRoot->info );   // the recursive calls set both links
        copyTree( copiedTreeRoot->lLink, otherTreeRoot->lLink );
        copyTree( copiedTreeRoot->rLink, otherTreeRoot->rLink );
    }
}

/**
 * @brief Recursive helper for the destructor and destroyTree(). Post-order on purpose:
 * both children must be deleted before the node that points to them.
 * @param nodePointer: a reference to the link that points at the subtree to free
 * @post nodePointer is nullptr, so it is never left pointing at freed memory
 */
// Post-order on purpose: both children must be deleted before the node that points to them
template <class T>
void Bst<T>::destroy( nodeType * & nodePointer )
{
    if (nodePointer != nullptr)
    {
        destroy( nodePointer->lLink );
        destroy( nodePointer->rLink );
        delete nodePointer;
        nodePointer = nullptr;// the nodePointer must not be left pointing at freed memory
    }
}

/**
 * @brief Recursive helper for inorderTraversal(). Visits the left subtree, then the node, then the right subtree.
 * @param nodePointer: the root of the subtree to visit
 * @param visit: pointer to the client's function, called with each value
 */
template <class T>
void Bst<T>::inorder( nodeType * nodePointer, void (*visit)( T & item ) )
{
    if (nodePointer != nullptr)
    {
        inorder( nodePointer->lLink, *visit ); // left first,
        (*visit)( nodePointer->info );// then hand this node's value to the client's function,
        inorder( nodePointer->rLink, *visit );// then the right
    }
}

/**
 * @brief Recursive helper for preorderTraversal(). Visits the node, then the left subtree, then the right subtree.
 * @param nodePointer: the root of the subtree to visit
 * @param visit: pointer to the client's function, called with each value
 */
template <class T>
void Bst<T>::preorder( nodeType * nodePointer, void (*visit)( T & item ) )
{
    if (nodePointer != nullptr)
    {
        (*visit)( nodePointer->info ); // this node first,
        preorder( nodePointer->lLink, *visit ); // then the left,
        preorder( nodePointer->rLink, *visit );  // then the right
    }
}

/**
 * @brief Recursive helper for postorderTraversal(). Visits the left subtree, then the right subtree, then the node.
 * @param nodePointer: the root of the subtree to visit
 * @param visit: pointer to the client's function, called with each value
 */
template <class T>
void Bst<T>::postorder( nodeType * nodePointer, void (*visit)( T & item ) )
{
    if (nodePointer != nullptr)
    {
        postorder( nodePointer->lLink, *visit ); // left,
        postorder( nodePointer->rLink, *visit ); // then right,
        (*visit)( nodePointer->info ); // this node last
    }
}

/**
 * @brief Recursive helper for treeNodeCount().
 * @param nodePointer: the root of the subtree to count
 * @return the number of nodes in the subtree (0 if it is empty)
 */
template <class T>
int Bst<T>::nodeCount( const nodeType * nodePointer )
{
    if (nodePointer == nullptr)
        return 0;
    return 1 + nodeCount( nodePointer->lLink ) + nodeCount( nodePointer->rLink );  // this node plus both subtrees
}

/**
 * @brief Recursive helper for isValidBst(). Every value in this subtree must be above *low and below *high.
 * @param nodePointer: the root of the subtree to check
 * @param low: the value every node must be larger than (nullptr means no limit on that side)
 * @param high: the value every node must be smaller than (nullptr means no limit on that side)
 * @return true if the subtree keeps the BST order, false otherwise
 */
// Every value in this subtree must be above *low and below *high (manualOne nullptr means no limit on that side)
template <class T>
bool Bst<T>::checkNode( const nodeType * nodePointer, const T * low, const T * high )
{
    if (nodePointer == nullptr) // an empty subtree is always valid
        return true;
    if (low != nullptr && !(*low < nodePointer->info)) // not larger than everything on its left
        return false;
    if (high != nullptr && !(nodePointer->info < *high)) // not smaller than everything on its right
        return false;
    // going left, this node becomes the upper limit; going right, it becomes the lower limit
    return checkNode( nodePointer->lLink, low, &nodePointer->info ) && checkNode( nodePointer->rLink, &nodePointer->info, high );
}

#endif // BST_H
