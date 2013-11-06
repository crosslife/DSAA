#include <iostream>
using namespace std;

bool isEven( int n )
{
    return n % 2 == 0;
}

long pow( long x, int n )
{
    if( n == 0 )
        return 1;
    if( n == 1 )
        return x;
    if( isEven( n ) )
        return pow( x * x, n / 2 );
    else
        return pow( x * x, n / 2 ) * x;
}

// Test program
int main( )
{
    cout << "2^21 = " << pow( 2, 21 ) << endl;
    cout << "2^30 = " << pow( 2, 30 ) << endl;
    return 0;
}
