#ifndef AA_TREE_H
#define AA_TREE_H

#include "dsexceptions.h"
#include <iostream>        // For NULL
using namespace std;

// AATree class
//
// CONSTRUCTION: with ITEM_NOT_FOUND object used to signal failed finds
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// boolean isEmpty( )     --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order
// ******************ERRORS********************************
// Throws UnderflowException as warranted

template <typename Comparable>
class AATree
{
  public:
    AATree( )
    {
        nullNode = new AANode;
        nullNode->left = nullNode->right = nullNode;
        nullNode->level = 0;
        root = nullNode;
    }

    AATree( const AATree & rhs )
    {
        nullNode = new AANode;
        nullNode->left = nullNode->right = nullNode;
        nullNode->level = 0;
        root = clone( rhs.root );
    }

    ~AATree( )
    {
        makeEmpty( );
        delete nullNode;
    }

    const Comparable & findMin( ) const
    {
        if( isEmpty( ) )
            throw UnderflowException( );

        AANode *ptr = root;
        while( ptr->left != nullNode )
            ptr = ptr->left;

        return ptr->element;
    }

    const Comparable & findMax( ) const
    {
        if( isEmpty( ) )
            throw UnderflowException( );

        AANode *ptr = root;
        while( ptr->right != nullNode )
            ptr = ptr->right;

        return ptr->element;
    }

    bool contains( const Comparable & x ) const
    {
        AANode *current = root;
        nullNode->element = x;

        for( ; ; )
        {
            if( x < current->element )
                current = current->left;
            else if( current->element < x )
                current = current->right;
            else
                return current != nullNode;
        }
    }

    bool isEmpty( ) const
    {
        return root == nullNode;
    }

    void printTree( ) const
    {
        if( root == nullNode )
            cout << "Empty tree" << endl;
        else
            printTree( root );
    }

    void makeEmpty( )
    {
        makeEmpty( root );
    }


    /**
     * Insert item x into the tree. Does nothing if x already prsent.
     */
    void insert( const Comparable & x )
    {
        insert( x, root );
    }

    void remove( const Comparable & x )
    {
        remove( x, root );
    }

    const AATree & operator=( const AATree & rhs )
    {
        if( this != &rhs )
        {
            makeEmpty( );
            root = clone( rhs.root );
        }

        return *this;
    }

  private:
    struct AANode
    {
        Comparable element;
        AANode    *left;
        AANode    *right;
        int        level;

        AANode( ) : left( NULL ), right( NULL ), level( 1 ) { }
        AANode( const Comparable & e, AANode *lt, AANode *rt, int lv = 1 )
          : element( e ), left( lt ), right( rt ), level( lv ) { }
    };

    AANode *root;
    AANode *nullNode;

        // Recursive routines
    /**
     * Internal method to insert into a subtree.
     * x is the item to insert.
     * t is the node that roots the tree.
     * Set the new root of the subtree.
     */
    void insert( const Comparable & x, AANode * & t )
    {
        if( t == nullNode )
            t = new AANode( x, nullNode, nullNode );
        else if( x < t->element )
            insert( x, t->left );
        else if( t->element < x )
            insert( x, t->right );
        else
            return;  // Duplicate; do nothing

        skew( t );
        split( t );
    }

    /**
     * Internal method to remove from a subtree.
     * x is the item to remove.
     * t is the node that roots the tree.
     * Set the new root of the subtree.
     */
    void remove( const Comparable & x, AANode * & t )
    {
        static AANode *lastNode, *deletedNode = nullNode;

        if( t != nullNode )
        {
            // Step 1: Search down the tree and set lastNode and deletedNode
            lastNode = t;
            if( x < t->element )
                remove( x, t->left );
            else
            {
                deletedNode = t;
                remove( x, t->right );
            }

            // Step 2: If at the bottom of the tree and
            //         x is present, we remove it
            if( t == lastNode )
            {
                if( deletedNode == nullNode || x != deletedNode->element )
                    return;   // Item not found; do nothing
                deletedNode->element = t->element;
                deletedNode = nullNode;
                t = t->right;
                delete lastNode;
            }

            // Step 3: Otherwise, we are not at the bottom; rebalance
            else
                if( t->left->level < t->level - 1 ||
                    t->right->level < t->level - 1 )
                {
                    if( t->right->level > --t->level )
                        t->right->level = t->level;
                    skew( t );
                    skew( t->right );
                    skew( t->right->right );
                    split( t );
                    split( t->right );
                }
        }
    }

    void makeEmpty( AANode * & t )
    {
        if( t != nullNode )
        {
            makeEmpty( t->left );
            makeEmpty( t->right );
            delete t;
        }
        t = nullNode;
    }

    void printTree( AANode *t ) const
    {
        if( t != nullNode )
        {
            printTree( t->left );
            cout << t->element << endl;
            printTree( t->right );
        }
    }

        // Rotations
    /**
     * Skew primitive for AA-trees.
     * t is the node that roots the tree.
     */
    void skew( AANode * & t )
    {
        if( t->left->level == t->level )
            rotateWithLeftChild( t );
    }

    /**
     * Split primitive for AA-trees.
     * t is the node that roots the tree.
     */
    void split( AANode * & t )
    {
        if( t->right->right->level == t->level )
        {
            rotateWithRightChild( t );
            t->level++;
        }
    }

    void rotateWithLeftChild( AANode * & k2 )
    {
        AANode *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2 = k1;
    }

    void rotateWithRightChild( AANode * & k1 )
    {
        AANode *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1 = k2;
    }

    AANode * clone( AANode *t ) const
    {
        if( t == t->left )  // Cannot test against nullNode!!!
            return nullNode;
        else
            return new AANode( t->element, clone( t->left ), clone( t->right ), t->level );
    }
};

#endif
