#include <iostream>
#include "DSL.h"
using namespace std;

    // Test program
int main( )
{
    const int INFINITY = 99999999;
    DSL<int> t( INFINITY );
    int NUMS = 3900;
    const int GAP  =   37;
    int i;

    cout << "Checking... (no more output means success)" << endl;

    for( i = GAP; i != 0; i = ( i + GAP ) % NUMS )
        t.insert( i );

    if( NUMS < 40 )
        t.printList( );
    if( t.findMin( ) != 1 || t.findMax( ) != NUMS - 1 )
        cout << "FindMin or FindMax error!" << endl;

    for( i = 1; i < NUMS; i++ )
        if( !t.contains( i ) )
            cout << "Find error1!" << endl;
    if( t.contains( 0 ) )
        cout << "Oops!" << endl;

    cout << "Test finished..." << endl;

    return 0;
}
