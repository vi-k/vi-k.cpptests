#include <stdlib.h>

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#include <my_stopwatch.h>
#include <my_time.cpp>
#include <my_num.cpp>

#define SIZE 1000


struct random_fill
{
	posix_time::ptime operator()() const
	{
		unsigned short y = rand() % 100 + 1970;
		unsigned short m = rand() % 12 + 1;
		unsigned short d = rand() % 31 + 1;
		long hh = rand() % 24;
		long mm = rand() % 60;
		long ss = rand() % 60;
		long ff = rand() % 100000;

		posix_time::ptime time;

		try
		{
			time = posix_time::ptime(gregorian::date(y,m,d),
				posix_time::time_duration(hh,mm,ss,ff));
		}
		catch(...)
		{
		}

		return time;
	}
};

template<class Time>
void test(const vector<Time> &v1,
	const vector<Time> &v2)
{
	if (v1.size() != v2.size())
	{
		cout << "test failed - sizes not eq" << endl;
		return;
	}

	for (vector<Time>::const_iterator iter1 = v1.begin(),
		iter2 = v2.begin(); iter1 != v1.end();
		iter1++, iter2++)
	{
		if (*iter1 != *iter2)
		{
			cout << "test failed - v1 != v2" << endl;
			return;
		}
	}
}

template<class Time>
void zero(Time &t)
{
	t = Time();
}

int main(int argc, char *argv[])
{
	my::stopwatch timer(MY_SW_COUNT | MY_SW_TOTAL | MY_SW_AVG);

	int n = 1000;
	if (argc > 1)
		n = atoi(argv[1]);
	cout << "n=" << n << endl;
	cout << "Iterations = n*" << SIZE << " = " << n * SIZE << endl;

	srand(0);

	vector<posix_time::ptime> times(SIZE);
	vector<posix_time::ptime> times_dest(SIZE);
	generate(times.begin(), times.end(), random_fill());
	
	vector<posix_time::time_duration> tods(SIZE);
	vector<posix_time::time_duration> tods_dest(SIZE);

	vector<gregorian::date> dates(SIZE);
	vector<gregorian::date> dates_dest(SIZE);

	vector<string> times_s(SIZE);
	vector<string> tods_s(SIZE);
	vector<string> dates_s(SIZE);

	for (int i = 0; i < SIZE; i++)
	{	
		times_s[i] = my::time::to_string(times[i]);
		dates[i] = times[i].date();
		dates_s[i] = my::time::to_string(dates[i]);
		tods[i] = times[i].time_of_day();
		tods_s[i] = my::time::to_string(tods[i]);
	}


	for_each( tods_dest.begin(), tods_dest.end(),
		zero<posix_time::time_duration>);
	timer.reset();
	timer.start();
	cout << "to_duration:         ";
	for (int i = 0; i < n; i++)
	{
		for (int i = 0; i < SIZE; i++)
		{
			tods_dest[i] = my::time::to_duration(tods_s[i]);
		}
	}
	timer.finish();
	timer.count = n;
	cout << timer << endl;
	test(tods, tods_dest);

	for_each( tods_dest.begin(), tods_dest.end(),
		zero<posix_time::time_duration>);
	timer.reset();
	timer.start();
	cout << "duration_from_string: ";
	for (int i = 0; i < n/10; i++)
	{
		for (int i = 0; i < SIZE; i++)
		{
			try
			{
				tods_dest[i] = posix_time::duration_from_string(tods_s[i]);
			}
			catch(...)
			{
				tods_dest[i] = posix_time::not_a_date_time;
			}
		}
	}
	timer.finish();
	timer.count = n/10;
	cout << timer << endl;
	test(tods, tods_dest);

	for_each( tods_dest.begin(), tods_dest.end(),
		zero<posix_time::time_duration>);
	timer.reset();
	timer.start();
	cout << ">>:                   ";
	for (int i = 0; i < n/10; i++)
	{
		istringstream ss;
		for (int i = 0; i < SIZE; i++)
		{
			ss.str(tods_s[i]);
			ss >> tods_dest[i];
		}
	}
	timer.finish();
	timer.count = n/10;
	cout << timer << endl;
	test(tods, tods_dest);

	for_each( tods_dest.begin(), tods_dest.end(),
		zero<posix_time::time_duration>);
	timer.reset();
	timer.start();
	cout << ">> (2):               ";
	for (int i = 0; i < n/500; i++)
	{
		for (int i = 0; i < SIZE; i++)
		{
			istringstream ss(tods_s[i]);
			ss >> tods_dest[i];
		}
	}
	timer.finish();
	timer.count = n/500;
	cout << timer << endl;
	test(tods, tods_dest);

	for_each( tods_dest.begin(), tods_dest.end(),
		zero<posix_time::time_duration>);
	timer.reset();
	timer.start();
	cout << ">> (3):               ";
	for (int i = 0; i < n/500; i++)
	{
		for (int i = 0; i < SIZE; i++)
		{
			istringstream ss(tods_s[i]);
			
			posix_time::time_facet *f = new posix_time::time_facet("");
			f->time_duration_format("%H:%M:%S%F");
			ss.imbue( std::locale(ss.getloc(), f) );

			ss >> tods_dest[i];
		}
	}
	timer.finish();
	timer.count = n/500;
	cout << timer << endl;
	test(tods, tods_dest);

	return 0;
}
