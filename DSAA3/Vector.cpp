#ifndef VECTOR_CPP_
#define VECTOR_CPP_

#include "vector.h"

#include "StartConv.h"

template <class Object>
const vector<Object> & vector<Object>::operator=( const vector<Object> & rhs )
{
    if( this != &rhs )
    {
        delete [ ] objects;
        theSize = rhs.size( );
        theCapacity = rhs.theCapacity;

        objects = new Object[ capacity( ) ];
        for( int k = 0; k < size( ); k++ )
            objects[ k ] = rhs.objects[ k ];
    }
    return *this;
}

template <class Object>
void vector<Object>::resize( int newSize )
{
    if( newSize > theCapacity )
        reserve( newSize * 2 );
    theSize = newSize;
}


template <class Object>
void vector<Object>::reserve( int newCapacity )
{
    Object *oldArray = objects;

    int numToCopy = newCapacity < theSize ? newCapacity : theSize;
    newCapacity += SPARE_CAPACITY;

    objects = new Object[ newCapacity ];
    for( int k = 0; k < numToCopy; k++ )
        objects[ k ] = oldArray[ k ];

    theSize = numToCopy;
    theCapacity = newCapacity;

    delete [ ] oldArray;
}


template <class Object>
void vector<Object>::push_back( const Object & x )
{
    if( theSize == theCapacity )
        reserve( 2 * theCapacity + 1 );
    objects[ theSize++ ] = x;
}


template <class Object>
void vector<Object>::pop_back( )
{
    if( empty( ) )
        throw UnderflowException( "Cannot call pop_back on empty vector" );
    theSize--;
}


template <class Object>
const Object & vector<Object>::back( ) const
{
    if( empty( ) )
        throw UnderflowException( "Cannot call back on empty vector" );
    return objects[ theSize - 1 ];
}



#include "EndConv.h"

#endif
