#ifndef TREAP_H
#define TREAP_H

#include <climits>
#include "Random.h"
#include "dsexceptions.h"
#include <iostream>        // For NULL

using namespace std;

// Treap class
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x (unimplemented)
// bool contains( x )     --> Return true if x is present
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// bool isEmpty( )        --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order
// ******************ERRORS********************************
// Throws UnderflowException as warranted

template <typename Comparable>
class Treap
{
  public:
    Treap( )
    {
        nullNode = new TreapNode;
        nullNode->left = nullNode->right = nullNode;
        nullNode->priority = INT_MAX;
        root = nullNode;
    }

    Treap( const Treap & rhs )
    {
        nullNode = new TreapNode;
        nullNode->left = nullNode->right = nullNode;
        nullNode->priority = INT_MAX;
        root = nullNode;
        *this = rhs;
    }

    ~Treap( )
    {
        makeEmpty( );
        delete nullNode;
    }

    const Comparable & findMin( ) const
    {
        if( isEmpty( ) )
            throw UnderflowException( );

        TreapNode *ptr = root;
        while( ptr->left != nullNode )
            ptr = ptr->left;

        return ptr->element;
    }

    const Comparable & findMax( ) const
    {
        if( isEmpty( ) )
            throw UnderflowException( );

        TreapNode *ptr = root;
        while( ptr->right != nullNode )
            ptr = ptr->right;

        return ptr->element;
    }

    bool contains( const Comparable & x ) const
    {
        TreapNode *current = root;
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
        if( isEmpty( ) )
            cout << "Empty tree" << endl;
        else
            printTree( root );
    }

    void makeEmpty( )
    {
        makeEmpty( root );
    }

    void insert( const Comparable & x )
    {
        insert( x, root );
    }

    void remove( const Comparable & x )
    {
        remove( x, root );
    }

    const Treap & operator=( const Treap & rhs )
    {
        if( this != &rhs )
        {
            makeEmpty( );
            root = clone( rhs.root );
        }

        return *this;
    }

  private:
    struct TreapNode
    {
        Comparable element;
        TreapNode *left;
        TreapNode *right;
        int        priority;

        TreapNode( ) : left( NULL ), right( NULL ), priority( INT_MAX ) { }
        TreapNode( const Comparable & theElement, TreapNode *lt, TreapNode *rt, int pr )
            : element( theElement ), left( lt ), right( rt ), priority( pr )
            { }
    };

    TreapNode *root;
    TreapNode *nullNode;
    Random randomNums;

        // Recursive routines
    /**
     * Internal method to insert into a subtree.
     * x is the item to insert.
     * t is the node that roots the tree.
     * Set the new root of the subtree.
     * (randomNums is a Random object that is a data member of Treap.)
     */
    void insert( const Comparable & x, TreapNode* & t )
    {
        if( t == nullNode )
            t = new TreapNode( x, nullNode, nullNode, randomNums.randomInt( ) );
        else if( x < t->element )
        {
            insert( x, t->left );
            if( t->left->priority < t->priority )
                rotateWithLeftChild( t );
        }
        else if( t->element < x )
        {
            insert( x, t->right );
            if( t->right->priority < t->priority )
                rotateWithRightChild( t );
        }
        // else duplicate; do nothing
    }

    /**
     * Internal method to remove from a subtree.
     * x is the item to remove.
     * t is the node that roots the tree.
     * Set the new root of the subtree.
     */
    void remove( const Comparable & x, TreapNode * & t )
    {
        if( t != nullNode )
        {
            if( x < t->element )
                remove( x, t->left );
            else if( t->element < x )
                remove( x, t->right );
            else
            {
                    // Match found
                if( t->left->priority < t->right->priority )
                    rotateWithLeftChild( t );
                else
                    rotateWithRightChild( t );

                if( t != nullNode )      // Continue on down
                    remove( x, t );
                else
                {
                    delete t->left;
                    t->left = nullNode;  // At a leaf
                }
            }
        }
    }

    void makeEmpty( TreapNode * & t )
    {
        if( t != nullNode )
        {
            makeEmpty( t->left );
            makeEmpty( t->right );
            delete t;
        }
        t = nullNode;
    }

    void printTree( TreapNode *t ) const
    {
        if( t != nullNode )
        {
            printTree( t->left );
            cout << t->element << endl;
            printTree( t->right );
        }
    }

        // Rotations
    void rotateWithLeftChild( TreapNode * & k2 )
    {
        TreapNode *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2 = k1;
    }

    void rotateWithRightChild( TreapNode * & k1 )
    {
        TreapNode *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1 = k2;
    }

    TreapNode * clone( TreapNode * t ) const
    {
        if( t == t->left )  // Cannot test against nullNode!!!
            return nullNode;
        else
            return new TreapNode( t->element, clone( t->left ), clone( t->right ), t->priority );
    }
};

#endif
