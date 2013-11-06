#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <ctime>
using namespace std;

/*
DISTRIBUTION OF DICTIONARY:
Read the words...88984
1       1
2       48
3       601
4       2409
5       4882
6       8205
7       11989
8       13672
9       13014
10      11297
11      8617
12      6003
13      3814
14      2173
15      1169
16      600
17      302
18      107
19      53
20      28
Elapsed time FAST: 5.218
Elapsed time MEDIUM: 77.735
Elapsed time SLOW: 450.891
**/

vector<string> readWords( istream & in )
{
    string oneLine;
    vector<string> v;

    while( in >> oneLine )
        v.push_back( oneLine );
    
    return v;
}

// Returns true if word1 and word2 are the same length
// and differ in only one character.
bool oneCharOff( const string & word1, const string & word2 )
{
    if( word1.length( ) != word2.length( ) )
        return false;

    int diffs = 0;

    for( int i = 0; i < word1.length( ); i++ )
        if( word1[ i ] != word2[ i ] )
            if( ++diffs > 1 )
                return false;

    return diffs == 1;
}

// Computes a map in which the keys are words and values are vectors of words
// that differ in only one character from the corresponding key.
// Uses a quadratic algorithm.
map<string,vector<string> >
computeAdjacentWordsSlow( const vector<string> & words )
{
    map<string,vector<string> > adjWords;

    for( int i = 0; i < words.size( ); i++ )
        for( int j = i + 1; j < words.size( ); j++ )
            if( oneCharOff( words[ i ], words[ j ] ) )
            {
                adjWords[ words[ i ] ].push_back( words[ j ] );
                adjWords[ words[ j ] ].push_back( words[ i ] );
            }

    return adjWords;
}


// Computes a map in which the keys are words and values are vectors of words
// that differ in only one character from the corresponding key.
// Uses a quadratic algorithm, but speeds things up a little by
// maintaining an additional map that groups words by their length.
map<string,vector<string> >
computeAdjacentWordsMedium( const vector<string> & words )
{
    map<string,vector<string> > adjWords;
    map<int,vector<string> > wordsByLength;

      // Group the words by their length
    for( int i = 0; i < words.size( ); i++ )
        wordsByLength[ words[ i ].length( ) ].push_back( words[ i ] );

      // Work on each group separately
    map<int,vector<string> >::const_iterator itr;
    for( itr = wordsByLength.begin( ); itr != wordsByLength.end( ); ++itr )
    {
        const vector<string> & groupsWords = itr->second;

        for( int i = 0; i < groupsWords.size( ); i++ )
            for( int j = i + 1; j < groupsWords.size( ); j++ )
                if( oneCharOff( groupsWords[ i ], groupsWords[ j ] ) )
                {
                    adjWords[ groupsWords[ i ] ].push_back( groupsWords[ j ] );
                    adjWords[ groupsWords[ j ] ].push_back( groupsWords[ i ] );
                }
    }

    return adjWords;
}

// Computes a map in which the keys are words and values are vectors of words
// that differ in only one character from the corresponding key.
// Uses an efficient algorithm that is O(N log N) with a map, or
// O(N) is a hash_map is used.
map<string,vector<string> >
computeAdjacentWords( const vector<string> & words )
{
    map<string,vector<string> > adjWords;
    map<int,vector<string> > wordsByLength;

      // Group the words by their length
    for( int i = 0; i < words.size( ); i++ )
        wordsByLength[ words[ i ].length( ) ].push_back( words[ i ] );

      // Work on each group separately
    map<int,vector<string> >::const_iterator itr;
    for( itr = wordsByLength.begin( ); itr != wordsByLength.end( ); ++itr )
    {
        const vector<string> & groupsWords = itr->second;
        int groupNum = itr->first;

        // Work on each position in each group
        for( int i = 0; i < groupNum; i++ )
        {
            // Remove one character in specified position, computing representative.
            // Words with same representatives are adjacent; so first populate
            // a map...
            map<string,vector<string> > repToWord;

            for( int j = 0; j < groupsWords.size( ); j++ )
            {
                string rep = groupsWords[ j ];
                rep.erase( i, 1 );
                repToWord[ rep ].push_back( groupsWords[ j ] );
            }

            // and then look for map values with more than one string
            map<string,vector<string> >::const_iterator itr2;
            for( itr2 = repToWord.begin( ); itr2 != repToWord.end( ); ++itr2 )
            {
                const vector<string> & clique = itr2->second;
                if( clique.size( ) >= 2 )
                {
                    for( int p = 0; p < clique.size( ); p++ )
                        for( int q = p + 1; q < clique.size( ); q++ )
                        {
                            adjWords[ clique[ p ] ].push_back( clique[ q ] );
                            adjWords[ clique[ q ] ].push_back( clique[ p ] );
                        }
                }
            }
        }
    }

    return adjWords;
}

// Find most changeable word: the word that differs in only one
// character with the most words. Return a list of these words, in case of a tie.
vector<string>
findMostChangeable( const map<string,vector<string> > & adjacentWords )
{
    vector<string> mostChangeableWords;
    int maxNumberOfAdjacentWords = 0;

    map<string,vector<string> >::const_iterator itr = adjacentWords.begin( );

    while( itr != adjacentWords.end( ) )
    {
        const pair<string,vector<string> > & entry = *itr++;

        if( entry.second.size( ) > maxNumberOfAdjacentWords )
        {
            maxNumberOfAdjacentWords = entry.second.size( );
            mostChangeableWords.clear( );
        }
        if( entry.second.size( ) == maxNumberOfAdjacentWords )
            mostChangeableWords.push_back( entry.first );
    }

    return mostChangeableWords;
}

void printMostChangeables( const vector<string> & mostChangeable,
                           const map<string,vector<string> > & adjWords )
{
    map<string,vector<string> > & adjacentWords = const_cast<map<string,vector<string> > &>( adjWords );

    for( int i = 0; i < mostChangeable.size( ); i++ )
    {
        cout << mostChangeable[ i ] << ":";
        vector<string> adjacents = adjacentWords[ mostChangeable[ i ] ];
        for( int j = 0; j < adjacents.size( ); j++ )
            cout << " " << adjacents[ j ];
        cout << " (" << adjacents.size( ) << " words)" << endl;
    }
}

void printHighChangeables( const map<string,vector<string> > & adjacentWords, int minWords )
{
    map<string,vector<string> >::const_iterator itr = adjacentWords.begin( );

    while( itr != adjacentWords.end( ) )
    {
        const pair<string,vector<string> > & entry = *itr++;
        const vector<string> & words = entry.second;

        if( words.size( ) >= minWords )
        {
            cout << entry.first << " (" << words.size( ) << "):";
            for( int i = 0; i < words.size( ); i++ )
                cout << " " << words[ i ];
            cout << endl;
        }

    }
}



// After the shortest path calculation has run, computes the vector that
// contains the sequence of words changes to get from first to second.
vector<string> getChainFromPreviousMap( const map<string,string> & previous,
                                       const string & first,
                                       const string & second )
{
    map<string,string> & prev = const_cast<map<string,string> &>( previous );
    vector<string> result;
    if( prev[ second ] != "" )
    {
        string current = second;
        while( current != "" )
        {
            result.push_back( current );
            current = prev[ current ];
        }
    }

    reverse( result.begin( ), result.end( ) );
    return result;
}

// Runs the shortest path calculation from the adjacency map, returning a vector
// that contains the sequence of word changes to get from first to second.
vector<string> findChain( const map<string,vector<string> > & adjacentWords,
                          const string & first,
                          const string & second )
{
    map<string,string> previousWord;
    queue<string> q;

    q.push( first );

    while( !q.empty( ) )
    {
        string current = q.front( ); q.pop( );

        map<string,vector<string> >::const_iterator itr;

        itr = adjacentWords.find( current );
        if( itr != adjacentWords.end( ) )
        {
            const vector<string> & adj = itr->second;
            for( int i = 0; i < adj.size( ); i++ )
                if( previousWord[ adj[ i ] ] == "" )
                {
                    previousWord[ adj[ i ] ] = current;
                    q.push( adj[ i ] );
                }
        }
    }
    previousWord[ first ] = "";

    return getChainFromPreviousMap( previousWord, first, second );
}

// Runs the shortest path calculation from the original set of words, returning
// a vector that contains the sequence of word changes to get from first to
// second. Since this calls computeAdjacentWords, it is recommended that the
// user instead call computeAdjacentWords once and then call other findChain for
// each word pair
vector<string> findChain( const vector<string> & words,
                          const string & first,
                          const string & second )
{
    map<string,vector<string> > adjacentWords = computeAdjacentWords( words );
    return findChain( adjacentWords, first, second );
}

int main( )
{
    clock_t start, end;

    ifstream fin( "dict.txt" );
    vector<string> words = readWords( fin );
    cout << "Read the words..." << words.size( ) << endl;
    map<string,vector<string> > adjacentWords;
    
    start = clock( );
    adjacentWords = computeAdjacentWords( words );
    end = clock( );
    cout << "Elapsed time FAST: " << double(end-start)/CLOCKS_PER_SEC << endl;

    printHighChangeables( adjacentWords, 15 );

    /*
    start = clock( );
    adjacentWords = computeAdjacentWordsMedium( words );
    end = clock( );
    cout << "Elapsed time MEDIUM: " << double(end-start)/CLOCKS_PER_SEC << endl;
    
    start = clock( );
    adjacentWords = computeAdjacentWordsSlow( words );
    end = clock( );
    cout << "Elapsed time SLOW: " << double(end-start)/CLOCKS_PER_SEC << endl;


    cout << "Adjacents computed..." << endl;
    vector<string> mostChangeable = findMostChangeable( adjacentWords );
    cout << "Most changeable computed..." << endl;
    printMostChangeables( mostChangeable, adjacentWords );
    */

    for( ; ; )
    {
        cout << "Enter two words: ";
        string w1, w2;
        cin >> w1 >> w2;

        vector<string> path = findChain( adjacentWords, w1, w2 );
        cout << path.size( ) << endl;
        for( int i = 0; i < path.size( ); i++ )
            cout << path[ i ] << " " ;
        cout << endl;
    }


    return 0;
}
