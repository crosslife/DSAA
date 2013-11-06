#include "dsexceptions.h"
#include <stdlib.h>
#include "List.h"


#include <iostream>
using namespace std;

static const int NUMS_PER_LINE = 14;


template <typename Object>
class Stack 
{
  public:
    bool isEmpty( ) const
      { return theList.empty( ); }
    const Object & top( ) const
      { return theList.front( ); }
    void push( const Object & x )
      { theList.push_front( x ); }
    void pop( Object & x )
      { x = theList.front( ); theList.pop_front( ); }
  private:
    List<Object> theList;
};

template <typename Object>
class Queue
{
  public:
    bool isEmpty( ) const
      { return theList.empty( ); }
    const Object & getFront( ) const
      { return theList.front( ); }
    void enqueue( const Object & x )
      { theList.push_back( x ); }
    void dequeue( Object & x )
      { x = theList.front( ); theList.pop_front( ); }
  private:
    List<Object> theList;
};

template <typename Collection>
void printCollection( const Collection & c )
{
    cout << "Collection contains: " << c.size( ) << " items" << endl;
    int i = 1;

    if( c.empty( ) )
        cout << "Empty container." << endl;
    else
    {
        for( typename Collection::const_iterator itr = c.begin( ); itr != c.end( ); itr++ )
        {
            cout << *itr << " ";
            if( i++ % NUMS_PER_LINE == 0 )
                cout << endl;
        }
        cout << endl;

        if( c.size( ) > NUMS_PER_LINE )
            return;
        cout << "In reverse: " << endl;
        for( typename Collection::const_iterator ritr = c.end( ); ritr != c.begin( ); )
            cout << *--ritr << " ";
        cout << endl << endl;
    }
}


int jos( int people, int passes, List<int> & order )
{
    List<int> theList;
    List<int>::iterator p = theList.begin( );
    List<int>::iterator tmp;
    Stack<int> s;
    Queue<int> q;
 

    order = List<int>( );

    int i;
    for( i = people; i >= 1; i-- )
        p = theList.insert( p, i );

    while( people-- != 1 )
    {
        for( i = 0; i < passes; i++ )
            if( ++p == theList.end( ) )
                p = theList.begin( );

        order.push_back( *p );
        s.push( *p );
        q.enqueue( *p );
        tmp = p;
        if( ++p == theList.end( ) )
            p = theList.begin( );
        theList.erase( tmp );
    }

    if( order.size( ) % 2 == 0 )
    {
        s.push( 0 );
        q.enqueue( 0 );
    }

    while( !s.isEmpty( ) && !q.isEmpty( ) )
    {
        int x, y;
        s.pop( x );
        q.dequeue( y );
        if( x == y )
            cout << "Middle removed is " << x << endl;
    }
    cout << "Only unremoved is ";
    return *theList.begin( );
}

    
void nonsense( int people, int passes )
{
    List<int> lastFew, theList;

    cout << jos( people, passes, lastFew ) << endl;
    
    cout << "(Removal order) ";
    printCollection( lastFew );
}

int main( )
{
    nonsense( 12, 0 );
    nonsense( 12, 1 );
    nonsense( 3737, 37 );
    
    return 0;
}
