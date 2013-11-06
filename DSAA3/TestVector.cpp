#include "Vector.h"
#include <iostream>
using namespace std;

int main( )
{
    Vector<int> v;

    for( int i = 0; i < 10; i++ )
        v.push_back( i );

    for( int i = 0; i < 10; i++ )
        cout << v[ i ] << endl;

    return 0;
}

