#include <climits>
#include <cstdlib>

#include <iostream> 
#include <sstream>
#include <boost/format.hpp>

#include <libs/date_time/src/gregorian/date_generators.cpp>
#include <libs/date_time/src/posix_time/posix_time_types.cpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time;


//  This value specifies, how to unroll the integer string generation loop in 
//  Karma.
//      Set this to some integer in between 0 (no unrolling) and max expected 
//      integer string len (complete unrolling). If not specified, this value
//      defaults to 6.
#define BOOST_KARMA_NUMERICS_LOOP_UNROLL 6

#include <boost/spirit/include/karma.hpp>

using namespace std;
using namespace boost::spirit;

#define MAX_ITERATION 10000000


///////////////////////////////////////////////////////////////////////////////
struct random_fill
{
    int operator()() const
    {
        int scale = std::rand() / 100 + 1;
        return (std::rand() * std::rand()) / scale;
    }
};

#define MUL10( a ) (((a) << 3 ) + ((a) << 1 ))

template<class T, class Char>
Char* signed_to(T n, Char *str)
{
	static const Char sym[]
		= { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

    if (n < 0)
        *str++ = '-';
    else
        n = -n;

	Char *ptr = str;

	while (n)
	{
		T nn = n / 10;
		T o = (nn<<3) + (nn<<1) - n;
		*ptr++ = sym[o];
		n = nn;
	}

	*ptr-- = 0;

    while (str < ptr)
   	{
        Char tmp = *ptr;
        *ptr-- = *str;
        *str++ = tmp;
    }

	return ptr;
}

template<class T, class Char>
std::size_t signed_to2(T n, Char *str, std::size_t decimals = 0,
	std::size_t width = -1)
{
	static const Char sym[]
		= { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

	Char buf[64];
	Char *buf_ptr = buf;

	Char *str_ptr = str;
	bool neg;

    if ( (neg = n < 0) == false)
        n = -n;

	do
	{
		T nn = n / 10;
		*buf_ptr++ = sym[ (nn<<3) + (nn<<1) - n ];
		n = nn;
	} while (n);

	Char *buf_min = buf + decimals;
	while (buf_ptr < buf_min)
		*buf_ptr++ = '0';

	if (buf_ptr - buf + neg > width)
	{
		Char *str_end = str + width;
		while (str_ptr != str_end)
   		    *str_ptr++ = '#';
   	}
   	else
   	{
	    if (neg)
    	    *str_ptr++ = '-';

		while (buf_ptr != buf)
   		    *str_ptr++ = *--buf_ptr;
   	}

    *str_ptr = 0;

	return str_ptr - str;
}

char* gcc_itoa(int value, char* result, int base) {
		// check that the base if valid
		if (base < 2 || base > 36) { *result = '\0'; return result; }
	
		char* ptr = result, *ptr1 = result, tmp_char;
		int tmp_value;
	
		do {
			tmp_value = value;
			value /= base;
			*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
		} while ( value );
	
		// Apply negative sign
		if (tmp_value < 0) *ptr++ = '-';
		*ptr-- = '\0';
		while(ptr1 < ptr) {
			tmp_char = *ptr;
			*ptr--= *ptr1;
			*ptr1++ = tmp_char;
		}
		return result;
	}


void itoa10_2( short a, char *s )
{
	static const char sym[]
		= { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

    unsigned int b = a;
    char *p;

    if ( a < 0 )
    {
        *s++ = '-';
        b = -a;
    }

    p = s;

    do
    {
        unsigned int d = ( b * 0x6667 ) >> 18; // magic
        *p++ = sym[ b - MUL10( d ) ];
        b = d;
    }
    while ( b );

    *p-- = '\0';

    // inverse
    //while ( s < p ) 
    //    *p-- ^= *s++ ^= *p ^= *s;

    while ( s < p ) 
    {
        char t = *p;
        *p-- = *s;
        *s++ = t;
        //*p-- ^= *s++ ^= *p ^= *s;
    }
}

void itoa10( short a, char *s )
{
    unsigned int b = a;
    char *p;

    if ( a < 0 )
    {
        *s++ = '-';
        b = -a;
    }

    p = s;

    do
    {
        unsigned int d = ( b * 0x6667 ) >> 18; // magic
        *p++ = b - MUL10( d ) + '0';
        b = d;
    }
    while ( b );

    *p-- = '\0';

    // inverse
    while ( s < p ) 
        *p-- ^= *s++ ^= *p ^= *s;
}


///////////////////////////////////////////////////////////////////////////////
int main()
{
    namespace karma = boost::spirit::karma;

    cout << "Converting " << MAX_ITERATION 
         << " randomly generated int values to strings." << flush << endl;

    std::srand(0);
    std::vector<int> v (MAX_ITERATION);
    std::generate(v.begin(), v.end(), random_fill()); // randomly fill the vector

    /* itoa */
    {
        char buffer[65];
        std::string str;
		ptime start = microsec_clock::local_time();
        for (int i = 0; i < MAX_ITERATION; ++i)
        {
            itoa(v[i], buffer, 10);
            //str = buffer;      // compensate for string ops in other benchmarks
        }

		time_duration time = microsec_clock::local_time() - start;
        cout << "itoa:\t\t" << to_simple_string(time) << endl;
    }

    /* signed_to */
    {
        char buffer[65];
        std::string str;
		ptime start = microsec_clock::local_time();
        for (int i = 0; i < MAX_ITERATION; ++i)
        {
            signed_to(v[i], buffer);
            //str = buffer;      // compensate for string ops in other benchmarks
        }
		time_duration time = microsec_clock::local_time() - start;
        cout << "signed_to:\t\t" << to_simple_string(time) << endl;
    }

    /* signed_to */
    {
        char buffer[65];
        std::string str;
		ptime start = microsec_clock::local_time();
        for (int i = 0; i < MAX_ITERATION; ++i)
        {
            signed_to2(v[i], buffer);
            //str = buffer;      // compensate for string ops in other benchmarks
        }
		time_duration time = microsec_clock::local_time() - start;
        cout << "signed_to2:\t\t" << to_simple_string(time) << endl;
    }

    /* gcc_itoa */
    {
        char buffer[65];
        std::string str;
		ptime start = microsec_clock::local_time();
        for (int i = 0; i < MAX_ITERATION; ++i)
        {
            gcc_itoa(v[i], buffer, 10);
            //str = buffer;      // compensate for string ops in other benchmarks
        }
		time_duration time = microsec_clock::local_time() - start;
        cout << "gcc_itoa:\t\t" << to_simple_string(time) << endl;
    }

    /* itoa10 */
    {
        char buffer[65];
        std::string str;
		ptime start = microsec_clock::local_time();
        for (int i = 0; i < MAX_ITERATION; ++i)
        {
            itoa10(v[i], buffer);
            //str = buffer;      // compensate for string ops in other benchmarks
        }
		time_duration time = microsec_clock::local_time() - start;
        cout << "itoa10:\t\t" << to_simple_string(time) << endl;
    }

    /* itoa10 */
    {
        char buffer[65];
        std::string str;
		ptime start = microsec_clock::local_time();
        for (int i = 0; i < MAX_ITERATION; ++i)
        {
            itoa10_2(v[i], buffer);
            //str = buffer;      // compensate for string ops in other benchmarks
        }
		time_duration time = microsec_clock::local_time() - start;
        cout << "itoa10_2:\t\t" << to_simple_string(time) << endl;
    }

    #if 0
    // test the iostreams library
    {
        //[karma_int_performance_iostreams
        std::stringstream str;
        //<-
		ptime start = microsec_clock::local_time();
        //->
        for (int i = 0; i < MAX_ITERATION; ++i)
        {
            str.str("");
            str << v[i];
        }
        //]

		time_duration time = microsec_clock::local_time() - start;
        cout << "iostreams:\t" << to_simple_string(time) << endl;
    }

    // test the Boost.Format library
    {
        //[karma_int_performance_format
        std::string str;
        boost::format int_format("%d");
        //<-
		ptime start = microsec_clock::local_time();
        //->
        for (int i = 0; i < MAX_ITERATION; ++i)
        {
            str = boost::str(int_format % v[i]);
        }
        //]

		time_duration time = microsec_clock::local_time() - start;
        cout << "Boost.Format:\t" << to_simple_string(time) << endl;
    }
    #endif

    // test the Karma int_ generation routines 
    {
        std::string str;
		ptime start = microsec_clock::local_time();

        char buffer[65]; // we don't expect more than 64 bytes to be generated here
        for (int i = 0; i < MAX_ITERATION; ++i)
        {
            char *ptr = buffer;
            karma::generate(ptr, int_, v[i]);
            *ptr = '\0';
            //str = buffer;      // compensate for string ops in other benchmarks
        }

		time_duration time = microsec_clock::local_time() - start;
        cout << "int_:\t\t" << to_simple_string(time) << endl;
    }

    #if 0
    // test the Karma int_ generation routines 
    {
	    typedef std::back_insert_iterator<std::string> sink_type;

        std::string str;
        sink_type sink(str);
		ptime start = microsec_clock::local_time();

        //[karma_int_performance_plain
        for (int i = 0; i < MAX_ITERATION; ++i)
        {
            karma::generate(sink, int_, v[i]);
        }
        //]

		time_duration time = microsec_clock::local_time() - start;
        cout << "int_:\t\t" << to_simple_string(time) << endl;
    }
    #endif

    return 0;
}
