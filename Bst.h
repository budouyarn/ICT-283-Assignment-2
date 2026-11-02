#ifndef BST_H
#define BST_H

/**
 * A Binary Search Tree template that stores values of any type T.
 *
 * What T must provide:
 *   - operator<  : the only comparison. Two values are equal when neither is smaller.
 *   - a copy constructor : used when a node is made to hold a value.
 * T does NOT need a default constructor, operator=, or operator<<, because the tree never
 * prints anything itself
  every traversal hands each value to a function the client supplies.
 *
 *
 * Traversals take a POINTER TO FUNCTION, so the client decides what happens to each value
 * (print it, count it, collect it) without the tree knowing.
 * The client must NOT change a value in a way that changes
 * its order, or the class invariant breaks (isValidBst() would then report false).
 *
 * Everything is in this one header because the compiler needs the full template
 * definition wherever Bst<T> is used. There is no .cpp file.
 *
 *
 * @tparam T the type of value stored in the tree
 *
 * @class Bst
 * @author  Lim Wen Xin Sherine
 * @version 2.0
 * @date    01/11/2026
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
         * @brief Copy constructor. Makes a deep copy, so the new tree owns its own nodes.
         * Used when a tree is passed or returned by value.
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
         * @return a const reference to this tree
         */
        const Bst<T> & operator=( const Bst<T> & otherTree );

        /**
         * @brief Function to determine whether the tree is empty.
         * @return true if the tree has no values, false otherwise
         */
        bool isEmpty() const;

        /**
         * @brief Inserts a value in its correct place so the BST properties still hold.
         * A value already in the tree is not inserted again.
         * @param insertItem: the value to insert
         * @return true if the value was inserted, false if it was already in the tree
         * @post on true, treeNodeCount() has increased by 1
         */
        bool insert( const T & insertItem );

        /**
         * @brief Function to determine if a value is in the tree.
         * @param searchItem: the value to look for
         * @return true if the value is found, false otherwise
         */
        bool search( const T & searchItem ) const;

        /**
         * @brief In-order traversal: left subtree, node, right subtree (ascending order).
         * Calls visit once for every value.
         * @param visit: nodePointer to the client's function, called with each value in turn
         */
        void inorderTraversal( void (*visit)( T & item ) ) const;

        /**
         * @brief Pre-order traversal: node, left subtree, right subtree.
         * @param visit: nodePointer to the client's function, called with each value in turn
         */
        void preorderTraversal( void (*visit)( T & item ) ) const;

        /**
         * @brief Post-order traversal: left subtree, right subtree, node.
         * @param visit: nodePointer to the client's function, called with each value in turn
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
            T info;                 /**< the value stored in this node */
            nodeType * lLink;       /**< root of the subtree of smaller values (nullptr if none) */
            nodeType * rLink;       /**< root of the subtree of larger values (nullptr if none) */

            /**
             * Builds a leaf node holding item, with no subtrees yet.
             * @param item: the value to store
             */
            nodeType( const T & item ) : info( item ), lLink( nullptr ), rLink( nullptr ) {}
        };

    private:
        /**
         * One node of the tree. Private, so only Bst can build or change nodes.
         */

        nodeType * root;/**< the top node of the tree, nullptr when the tree is empty */

        // The recursive helpers. Each one works on the subtree that starts at nodePointer.
        static nodeType * insertNode( nodeType * nodePointer, const T & insertItem, bool & inserted );
        static bool searchNode( const nodeType * nodePointer, const T & searchItem );
        static void copyTree( nodeType * & copiedTreeRoot, const nodeType * otherTreeRoot );
        static void destroy( nodeType * & nodePointer );
        static void inorder( nodeType * nodePointer, void (*visit)( T & item ) );
        static void preorder( nodeType * nodePointer, void (*visit)( T & item ) );
        static void postorder( nodeType * nodePointer, void (*visit)( T & item ) );
        static int nodeCount( const nodeType * nodePointer );
        static bool checkNode( const nodeType * nodePointer, const T * low, const T * high );
};

// ---------------------------------------------------------------------------
// Implementation (normal comments). It stays in the header because this is a
// template; the interface above stays separate from it.
// ---------------------------------------------------------------------------

template <class T>
Bst<T>::Bst() : root( nullptr )               // an empty tree has no root
{
}

template <class T>
Bst<T>::Bst( const Bst<T> & otherTree ) : root( nullptr )
{
    copyTree( root, otherTree.root );         // build a brand new set of nodes (nullptr if otherTree is empty)
}

template <class T>
Bst<T>::~Bst()
{
    destroy( root );                          // free every node
}

template <class T>
const Bst<T> & Bst<T>::operator=( const Bst<T> & otherTree )
{
    if (this != &otherTree)                   // avoid self-copy: a = a would delete the nodes we are about to copy
    {
        destroy( root );                      // release the old nodes first
        copyTree( root, otherTree.root );     // then take a copy of the other tree
    }
    return *this;
}

template <class T>
bool Bst<T>::isEmpty() const
{
    return root == nullptr;
}

template <class T>
bool Bst<T>::insert( const T & insertItem )
{
    bool inserted = false;                    // insertNode sets this to true only if a new node is made
    root = insertNode( root, insertItem, inserted );
    return inserted;
}

template <class T>
bool Bst<T>::search( const T & searchItem ) const
{
    return searchNode( root, searchItem );
}

template <class T>
void Bst<T>::inorderTraversal( void (*visit)( T & item ) ) const
{
    inorder( root, *visit );
}

template <class T>
void Bst<T>::preorderTraversal( void (*visit)( T & item ) ) const
{
    preorder( root, *visit );
}

template <class T>
void Bst<T>::postorderTraversal( void (*visit)( T & item ) ) const
{
    postorder( root, *visit );
}

template <class T>
int Bst<T>::treeNodeCount() const
{
    return nodeCount( root );
}

template <class T>
void Bst<T>::destroyTree()
{
    destroy( root );                          // sets root back to nullptr
}

template <class T>
bool Bst<T>::isValidBst() const
{
    return checkNode( root, nullptr, nullptr );     // no limits on the first node
}

// Returns the root of the subtree after the insert. A nullptr is the place where the new node goes.
// "typename" is needed because nodeType is a type that depends on T.
template <class T>
typename Bst<T>::nodeType * Bst<T>::insertNode( nodeType * nodePointer, const T & insertItem, bool & inserted )
{
    if (nodePointer == nullptr)                         // base case: found the empty place
    {
        nodeType * newNode = new nodeType( insertItem );   // a leaf: both links start as nullptr
        inserted = true;
        return newNode;
    }

    if (insertItem < nodePointer->info)                 // smaller values go in the left subtree
        nodePointer->lLink = insertNode( nodePointer->lLink, insertItem, inserted );
    else if (nodePointer->info < insertItem)            // larger values go in the right subtree
        nodePointer->rLink = insertNode( nodePointer->rLink, insertItem, inserted );
    // else: neither is smaller, so the value is already here; nothing changes and inserted stays false

    return nodePointer;                                 // this node is unchanged, so it stays the root of its subtree
}

template <class T>
bool Bst<T>::searchNode( const nodeType * nodePointer, const T & searchItem )
{
    if (nodePointer == nullptr)                         // base case: ran off the tree, so it is not there
        return false;
    if (searchItem < nodePointer->info)
        return searchNode( nodePointer->lLink, searchItem );   // only the left subtree can hold a smaller value
    if (nodePointer->info < searchItem)
        return searchNode( nodePointer->rLink, searchItem );   // only the right subtree can hold a larger value
    return true;                              // neither is smaller, so it is equal: found it
}

// Pre-order copy: make this node, then copy each subtree
template <class T>
void Bst<T>::copyTree( nodeType * & copiedTreeRoot, const nodeType * otherTreeRoot )
{
    if (otherTreeRoot == nullptr)
        copiedTreeRoot = nullptr;
    else
    {
        copiedTreeRoot = new nodeType( otherTreeRoot->info );   // same value; the recursive calls set both links
        copyTree( copiedTreeRoot->lLink, otherTreeRoot->lLink );
        copyTree( copiedTreeRoot->rLink, otherTreeRoot->rLink );
    }
}

// Post-order on purpose: both children must be deleted before the node that points to them
template <class T>
void Bst<T>::destroy( nodeType * & nodePointer )
{
    if (nodePointer != nullptr)
    {
        destroy( nodePointer->lLink );
        destroy( nodePointer->rLink );
        delete nodePointer;
        nodePointer = nullptr;                          // the nodePointer must not be left pointing at freed memory
    }
}

template <class T>
void Bst<T>::inorder( nodeType * nodePointer, void (*visit)( T & item ) )
{
    if (nodePointer != nullptr)
    {
        inorder( nodePointer->lLink, *visit );           // left first,
        (*visit)( nodePointer->info );                  // then hand this node's value to the client's function,
        inorder( nodePointer->rLink, *visit );           // then the right
    }
}

template <class T>
void Bst<T>::preorder( nodeType * nodePointer, void (*visit)( T & item ) )
{
    if (nodePointer != nullptr)
    {
        (*visit)( nodePointer->info );                  // this node first,
        preorder( nodePointer->lLink, *visit );          // then the left,
        preorder( nodePointer->rLink, *visit );          // then the right
    }
}

template <class T>
void Bst<T>::postorder( nodeType * nodePointer, void (*visit)( T & item ) )
{
    if (nodePointer != nullptr)
    {
        postorder( nodePointer->lLink, *visit );         // left,
        postorder( nodePointer->rLink, *visit );         // then right,
        (*visit)( nodePointer->info );                  // this node last
    }
}

template <class T>
int Bst<T>::nodeCount( const nodeType * nodePointer )
{
    if (nodePointer == nullptr)
        return 0;
    return 1 + nodeCount( nodePointer->lLink ) + nodeCount( nodePointer->rLink );   // this node plus both subtrees
}

// Every value in this subtree must be above *low and below *high (a nullptr means no limit on that side)
template <class T>
bool Bst<T>::checkNode( const nodeType * nodePointer, const T * low, const T * high )
{
    if (nodePointer == nullptr)                         // an empty subtree is always valid
        return true;
    if (low != nullptr && !(*low < nodePointer->info))  // not larger than everything on its left
        return false;
    if (high != nullptr && !(nodePointer->info < *high))   // not smaller than everything on its right
        return false;
    // going left, this node becomes the upper limit; going right, it becomes the lower limit
    return checkNode( nodePointer->lLink, low, &nodePointer->info ) && checkNode( nodePointer->rLink, &nodePointer->info, high );
}

#endif // BST_H
