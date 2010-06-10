/*
	“ест скорости преобразовани€ числа (int) в строку
*/
#include <stdio.h>
#include <stdlib.h>

#include <my_num.h>
#include <my_stopwatch.h>

#include <iostream> 
#include <sstream>
using namespace std;

#include <boost/spirit/include/karma.hpp>
#include <boost/format.hpp>

#include <libs/date_time/src/gregorian/date_generators.cpp>
#include <libs/date_time/src/posix_time/posix_time_types.cpp>
#include <boost/date_time/posix_time/posix_time.hpp>


#define SIZE 100000

struct random_fill
{
    int operator()() const
    {
        int scale = rand() / 100 + 1;
        return (rand() * rand()) / scale;
    }
};

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

	do
	{
		T nn = n / 10;
		T o = (nn<<3) + (nn<<1) - n;
		*ptr++ = sym[o];
		n = nn;
	} while (n);

	*ptr-- = 0;

    while (str < ptr)
   	{
        Char tmp = *ptr;
        *ptr-- = *str;
        *str++ = tmp;
    }

	return ptr;
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


#define MUL10( a ) (((a) << 3 ) + ((a) << 1 ))

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
    while ( s < p ) 
    {
        char t = *p;
        *p-- = *s;
        *s++ = t;
    }
}

template<class Char>
void test(const vector<basic_string<Char> > &v1,
	const vector<basic_string<Char> > &v2)
{
	if (v1.size() != v2.size())
	{
		cout << "* test failed - sizes not eq" << endl;
		return;
	}

	int index = 0;
	for (vector<basic_string<Char> >::const_iterator iter1 = v1.begin(),
		iter2 = v2.begin(); iter1 != v1.end();
		iter1++, iter2++)
	{
		if (*iter1 != *iter2)
		{
			cout << "* test failed: #" << index
				<< " " << *iter1 << " vs " << *iter2 << endl;
			return;
		}
		index++;
	}

	//cout << "test ok" << endl;
}

int main(int argc, char *argv[])
{
    namespace karma = boost::spirit::karma;

	my::stopwatch timer(MY_SW_AVG);

	int n = 100;
	int nn;

	if (argc > 1)
		n = atoi(argv[1]);
	cout << "n=" << n << endl;
	cout << "SIZE=" << SIZE << endl;

    srand(0);
    vector<int> v(SIZE);
    generate(v.begin(), v.end(), random_fill());

    vector<string> v_s(SIZE);
    vector<string> v_d;

	for (int i = 0; i < SIZE; i++)
	{
		stringstream ss;
		ss << v[i];
		v_s[i] = ss.str();
	}

    cout << "\n*** int to char* ***\n\n" << flush;

    {
        cout << "itoa (internal): " << flush;
        
        char buf[64];
		timer.restart();
        for (nn = 0; nn < n; ++nn)
	        for (int i = 0; i < SIZE; ++i)
    	    {
        	    itoa(v[i], buf, 10);
	        }
	    timer.finish();
	    timer.count = nn;
		cout << timer << endl;
    }

    {
        cout << "itoa (from gcc): " << flush;
        
        char buf[64];
		timer.restart();
        for (nn = 0; nn < n; ++nn)
	        for (int i = 0; i < SIZE; ++i)
    	    {
        	    gcc_itoa(v[i], buf, 10);
	        }
	    timer.finish();
	    timer.count = nn;
		cout << timer << endl;
    }

    {
        cout << "int_to:          " << flush;
        
        char buf[64];
		timer.restart();
        for (nn = 0; nn < n; ++nn)
	        for (int i = 0; i < SIZE; ++i)
    	    {
        	    signed_to(v[i], buf);
	        }
	    timer.finish();
	    timer.count = nn;
		cout << timer << endl;
    }

    {
        cout << "my::num::int_to: " << flush;
        
        char buf[64];
		timer.restart();
        for (nn = 0; nn < n; ++nn)
	        for (int i = 0; i < SIZE; ++i)
    	    {
        	    my::num::signed_to(v[i], buf, sizeof(buf));
	        }
	    timer.finish();
	    timer.count = nn;
		cout << timer << endl;
    }

    {
        cout << "boost::karma:    " << flush;
        
        char buf[64];
		timer.restart();
        for (nn = 0; nn < n; ++nn)
	        for (int i = 0; i < SIZE; ++i)
    	    {
	            char *ptr = buf;
    	        karma::generate(ptr, karma::int_, v[i]);
        	    *ptr = '\0';
	        }
	    timer.finish();
	    timer.count = nn;
		cout << timer << endl;
    }

    {
        cout << "itoa10 (16 bit): " << flush;
        
        char buf[64];
		timer.restart();
        for (nn = 0; nn < n; ++nn)
	        for (int i = 0; i < SIZE; ++i)
    	    {
	            itoa10(v[i], buf);
	        }
	    timer.finish();
	    timer.count = nn;
		cout << timer << endl;
    }

    {
        cout << "itoa10 (ver.2):  " << flush;
        
        char buf[64];
		timer.restart();
        for (nn = 0; nn < n; ++nn)
	        for (int i = 0; i < SIZE; ++i)
    	    {
	            itoa10_2(v[i], buf);
	        }
	    timer.finish();
	    timer.count = nn;
		cout << timer << endl;
    }

    {
        cout << "sprintf(\"%d\"):   " << flush;
        
        char buf[64];
		timer.restart();
        for (nn = 0; nn < n; ++nn)
	        for (int i = 0; i < SIZE; ++i)
    	    {
	            sprintf(buf, "%d", v[i]);
	        }
	    timer.finish();
	    timer.count = nn;
		cout << timer << endl;
    }


    cout << "\n*** int to std::string ***\n\n" << flush;

    {
        cout << "itoa (internal) (cast):       " << flush;
		v_d.assign(SIZE, "");

        char buf[64];
		timer.restart();
        for (nn = 0; nn < n; ++nn)
	        for (int i = 0; i < SIZE; ++i)
    	    {
        	    itoa(v[i], buf, 10);
        	    string str(buf);
        	    v_d[i] = str;
	        }
	    timer.finish();
	    timer.count = nn;
		cout << timer << endl;

		test(v_s, v_d);
    }

    {
        cout << "itoa (from gcc) (cast):       " << flush;
		v_d.assign(SIZE, "");
        
        char buf[64];
		timer.restart();
        for (nn = 0; nn < n; ++nn)
	        for (int i = 0; i < SIZE; ++i)
    	    {
        	    gcc_itoa(v[i], buf, 10);
        	    string str(buf);
        	    v_d[i] = str;
	        }
	    timer.finish();
	    timer.count = nn;
		cout << timer << endl;

		test(v_s, v_d);
    }

    {
        cout << "signed_to (cast):             " << flush;
		v_d.assign(SIZE, "");
        
        char buf[64];
		timer.restart();
        for (nn = 0; nn < n; ++nn)
	        for (int i = 0; i < SIZE; ++i)
    	    {
        	    signed_to(v[i], buf);
        	    string str(buf);
        	    v_d[i] = str;
	        }
	    timer.finish();
	    timer.count = nn;
		cout << timer << endl;

		test(v_s, v_d);
    }

    {
        cout << "my::num::int_to (cast):       " << flush;
		v_d.assign(SIZE, "");
        
        char buf[64];
		timer.restart();
        for (nn = 0; nn < n; ++nn)
	        for (int i = 0; i < SIZE; ++i)
    	    {
        	    my::num::int_to(v[i], buf, sizeof(buf));
        	    string str(buf);
        	    v_d[i] = str;
	        }
	    timer.finish();
	    timer.count = nn;
		cout << timer << endl;

		test(v_s, v_d);
    }

    {
        cout << "my::num::int_to_string:       " << flush;
		v_d.assign(SIZE, "");
        
		timer.restart();
        for (nn = 0; nn < n; ++nn)
	        for (int i = 0; i < SIZE; ++i)
    	    {
        	    v_d[i] = my::num::int_to_string(v[i]);
	        }
	    timer.finish();
	    timer.count = nn;
		cout << timer << endl;

		test(v_s, v_d);
    }

    {
        cout << "boost::karma (cast):          " << flush;
		v_d.assign(SIZE, "");
        
        char buf[64];
		timer.restart();
        for (nn = 0; nn < n; ++nn)
	        for (int i = 0; i < SIZE; ++i)
    	    {
	            char *ptr = buf;
    	        karma::generate(ptr, karma::int_, v[i]);
        	    *ptr = '\0';
        	    string str(buf);
        	    v_d[i] = str;
	        }
	    timer.finish();
	    timer.count = nn;
		cout << timer << endl;

		test(v_s, v_d);
    }

    {
	    typedef back_insert_iterator<string> sink_type;

        cout << "boost::karma (direct):        " << flush;
		v_d.assign(SIZE, "");
        
		timer.restart();
        for (nn = 0; nn < n; ++nn)
	        for (int i = 0; i < SIZE; ++i)
    	    {
		        string str;
        		sink_type sink(str);
    	        karma::generate(sink, karma::int_, v[i]);
        	    v_d[i] = str;
	        }
	    timer.finish();
	    timer.count = nn;
		cout << timer << endl;

		test(v_s, v_d);
    }

    {
        cout << "itoa10 (16 bit) (cast):       " << flush;
		v_d.assign(SIZE, "");
        
        char buf[64];
		timer.restart();
        for (nn = 0; nn < n; ++nn)
	        for (int i = 0; i < SIZE; ++i)
    	    {
	            itoa10(v[i], buf);
        	    string str(buf);
        	    v_d[i] = str;
	        }
	    timer.finish();
	    timer.count = nn;
		cout << timer << endl;

		test(v_s, v_d);
    }

    {
        cout << "itoa10 (ver.2) (cast):        " << flush;
		v_d.assign(SIZE, "");
        
        char buf[64];
		timer.restart();
        for (nn = 0; nn < n; ++nn)
	        for (int i = 0; i < SIZE; ++i)
    	    {
	            itoa10_2(v[i], buf);
        	    string str(buf);
        	    v_d[i] = str;
	        }
	    timer.finish();
	    timer.count = nn;
		cout << timer << endl;

		test(v_s, v_d);
    }

    {
        cout << "iostreams (<<) (global):      " << flush;
		v_d.assign(SIZE, "");

		stringstream ss;
        
		timer.restart();
        for (nn = 0; nn < n/10; ++nn)
	        for (int i = 0; i < SIZE; ++i)
    	    {
        	    ss.str("");
	            ss << v[i];
        	    v_d[i] = ss.str();
	        }
	    timer.finish();
	    timer.count = nn;
		cout << timer << endl;

		test(v_s, v_d);
    }

    {
        cout << "iostreams (<<) (local):       " << flush;
		v_d.assign(SIZE, "");

		timer.restart();
        for (nn = 0; nn < n/10; ++nn)
	        for (int i = 0; i < SIZE; ++i)
    	    {
        	    stringstream ss;
	            ss << v[i];
        	    v_d[i] = ss.str();
	        }
	    timer.finish();
	    timer.count = nn;
		cout << timer << endl;

		test(v_s, v_d);
    }

    {
        cout << "boost::format(\"%d\") (global): " << flush;
		v_d.assign(SIZE, "");

		boost::format int_format("%d");

		timer.restart();
        for (nn = 0; nn < n/10; ++nn)
	        for (int i = 0; i < SIZE; ++i)
    	    {
        	    v_d[i] = boost::str(int_format % v[i]);
	        }
	    timer.finish();
	    timer.count = nn;
		cout << timer << endl;

		test(v_s, v_d);
    }

    {
        cout << "boost::format(\"%d\") (local):  " << flush;
		v_d.assign(SIZE, "");

		timer.restart();
        for (nn = 0; nn < n/10; ++nn)
	        for (int i = 0; i < SIZE; ++i)
    	    {
        	    v_d[i] = boost::str(boost::format("%d") % v[i]);
	        }
	    timer.finish();
	    timer.count = nn;
		cout << timer << endl;

		test(v_s, v_d);
    }

    return 0;
}
