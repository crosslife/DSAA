#include <string>
#include <iostream>
#include <vector>
using namespace std;

const string & findMax( const vector<string> & arr )
{
    int maxIndex = 0;

    for( int i = 1; i < arr.size( ); i++ )
        if( arr[ maxIndex ] < arr[ i ] )
            maxIndex = i;

    return arr[ maxIndex ];
}

const string & findMaxWrong( const vector<string> & arr )
{
    string maxValue = arr[ 0 ];

    for( int i = 1; i < arr.size( ); i++ )
        if( maxValue < arr[ i ] )
            maxValue = arr[ i ];

    return maxValue;
}

int main( )
{
    vector<string> arr;

    arr.push_back( "hello" );
    arr.push_back( "there" );
    arr.push_back( "world" );

    cout << findMax( arr ) << endl;
    cout << findMaxWrong( arr ) << endl;

    return 0;
}
