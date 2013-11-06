#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Employee
{
  public:
    void setValue( const string & n, double s )
      { name = n; salary = s; }

    void print( ostream & out ) const
      { out << name << " (" << salary << ")"; }
    bool operator< ( const Employee & rhs ) const
      { return salary < rhs.salary; }

    // Other general accessors and mutators, not shown
  private:
    string name;
    double salary;
};

    // Define an output operator for Employee
ostream & operator<< ( ostream & out, const Employee & rhs )
{
    rhs.print( out );
    return out;
}

/**
 * Return the maximum item in array a.
 * Assumes a.size( ) > 0.
 * Comparable objects must provide operator< and operator=
 */
template <typename Comparable>
const Comparable & findMax( const vector<Comparable> & a )
{
    int maxIndex = 0;

    for( int i = 1; i < a.size( ); i++ )
        if( a[ maxIndex ] < a[ i ] )
            maxIndex = i;
    return a[ maxIndex ];
}

int main( )
{
    vector<Employee> v( 3 );

    v[0].setValue( "George Bush", 400000.00 );
    v[1].setValue( "Bill Gates", 2000000000.00 );
    v[2].setValue( "Dr. Phil", 13000000.00 );
    cout << findMax( v ) << endl;

    return 0;
}
