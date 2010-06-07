#include <climits>
#include <cstdlib>

#include <iostream> 
#include <sstream>

#include <libs/date_time/src/gregorian/date_generators.cpp>
#include <libs/date_time/src/posix_time/posix_time_types.cpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time;
using namespace std;

#include <boost/tuple/tuple_comparison.hpp>

#define MAX_ITERATION 10000000

struct foo
{
    int a;
    unsigned int b;
    long c;
    unsigned long d;
};

bool s1(foo const &l, foo const &r)
{
    if (l.a < r.a) return true;
    if (l.a > r.a) return false;
    if (l.b < r.b) return true;
    if (l.b > r.b) return false;
    if (l.c < r.c) return true;
    if (l.c > r.c) return false;
    return l.d < r.d;
}

bool s2(foo const &l, foo const &r)
{
    return boost::tie(l.a, l.b, l.c, l.d) < boost::tie(r.a, r.b, r.c, r.d);
}

///////////////////////////////////////////////////////////////////////////////
struct random_fill
{
    int operator()() const
    {
        int scale = std::rand() / 100 + 1;
        return (std::rand() * std::rand()) / scale;
    }
};

///////////////////////////////////////////////////////////////////////////////
int main()
{
    cout << "Converting " << MAX_ITERATION 
         << " randomly generated int values to strings." << flush << endl;

    std::srand(0);
    std::vector<int> a(MAX_ITERATION);
    std::generate(a.begin(), a.end(), random_fill()); // randomly fill the vector

    std::vector<int> b(MAX_ITERATION);
    std::generate(a.begin(), a.end(), random_fill()); // randomly fill the vector

    std::vector<int> c(MAX_ITERATION);
    std::generate(a.begin(), a.end(), random_fill()); // randomly fill the vector

    std::vector<int> d(MAX_ITERATION);
    std::generate(a.begin(), a.end(), random_fill()); // randomly fill the vector

    std::vector<foo> f(MAX_ITERATION);

    for (int i = 0; i < MAX_ITERATION; i++) {
    	foo _f;
    	_f.a = a[i];
    	_f.b = b[i];
    	_f.c = c[i];
    	_f.d = d[i];
    	f[i] = _f;
    }

    /*-
    {
		ptime start = microsec_clock::local_time();
		std::sort(f.begin(), f.end(), s1);
		time_duration time = microsec_clock::local_time() - start;
        cout << "s1:\t" << to_simple_string(time) << endl;
    }

    for (int i = 0; i < MAX_ITERATION; i++) {
    	foo _f;
    	_f.a = a[i];
    	_f.b = b[i];
    	_f.c = c[i];
    	_f.d = d[i];
    	f[i] = _f;
    }

    {
		ptime start = microsec_clock::local_time();
		std::sort(f.begin(), f.end(), s2);
		time_duration time = microsec_clock::local_time() - start;
        cout << "s2:\t" << to_simple_string(time) << endl;
    }
    -*/

    {
        ptime start = microsec_clock::local_time();

        for (int i = 1; i < MAX_ITERATION; i++) {
        	bool res;
		    if (f[i-1].a < f[i].a) res = true;
		    else if (f[i-1].a > f[i].a) res = false;
		    else if (f[i-1].b < f[i].b) res = true;
		    else if (f[i-1].b > f[i].b) res = false;
		    else if (f[i-1].c < f[i].c) res = true;
		    else if (f[i-1].c > f[i].c) res = false;
		    else res = f[i-1].d < f[i].d;
	    }
		time_duration time = microsec_clock::local_time() - start;
        cout << "s1:\t" << to_simple_string(time) << endl;
    }

    {
        ptime start = microsec_clock::local_time();

        for (int i = 1; i < MAX_ITERATION; i++) {
        	bool res = boost::tie(f[i-1].a, f[i-1].b, f[i-1].c, f[i-1].d)
        		< boost::tie(f[i].a, f[i].b, f[i].c, f[i].d);
	    }
		time_duration time = microsec_clock::local_time() - start;
        cout << "s2:\t" << to_simple_string(time) << endl;
    }

    return 0;
}
