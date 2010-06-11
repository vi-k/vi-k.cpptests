#include <stdio.h>
#include <stdlib.h>

#include <my_stopwatch.h>

#include <algorithm> 
#include <iostream> 
#include <sstream>
using namespace std;

#include <libs/date_time/src/gregorian/date_generators.cpp>
#include <libs/date_time/src/posix_time/posix_time_types.cpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#define SIZE 10000

#define CHAR char
#define COUT cout


/* ¬ывод бинарной строки заданного размера */
template<class Char>
inline std::size_t put(Char *buf, std::size_t buf_sz,
	const Char *str, std::size_t str_sz)
{
	Char *ptr = buf;

	if (buf_sz)
	{
		const Char *end = str + (buf_sz > str_sz ? str_sz : buf_sz - 1);

		/* Ётот вариант быстрее std::copy() */
		while (str != end)
			*ptr++ = *str++;

		*ptr = 0;
	}
	
	return ptr - buf;
}


/* ¬ывод бинарной строки заданного размера */
template<class Char>
inline std::size_t put2(Char *buf, std::size_t buf_sz,
	const Char *str, std::size_t str_sz)
{
	if (buf_sz)
	{
		buf_sz = (buf_sz > str_sz ? str_sz : buf_sz - 1);

		memcpy(buf, str, buf_sz * sizeof(Char));

		buf[buf_sz] = 0;
	}
	
	return buf_sz;
}

/* ¬ывод бинарной строки заданного размера */
template<class Char>
inline std::size_t put3(Char *buf, std::size_t buf_sz,
	const Char *str, std::size_t str_sz)
{
	Char *ptr = buf;

	if (buf_sz)
	{
		const Char *end = str + (buf_sz > str_sz ? str_sz : buf_sz - 1);

		ptr = std::copy(str, end, buf);

		*ptr = 0;
	}
	
	return ptr - buf;
}


CHAR strs1[SIZE][64] = {0};
CHAR strs2[SIZE][1000] = {0};
CHAR bufs1[SIZE][200];
CHAR bufs2[SIZE][15];
CHAR bufs3[SIZE][1100];

int main(int argc, char *argv[])
{
	my::stopwatch timer(MY_SW_AVG);

	int n = 1000;
	int nn;

	if (argc > 1)
		n = atoi(argv[1]);
	cout << "n=" << n << endl;

	cout << "*** " << sizeof(*strs1)/sizeof(**strs1)
		<< " -> " << sizeof(*bufs1)/sizeof(**bufs1) << " ***\n\n";

	{
		cout << "put with while():  " << flush;
		
		timer.restart();
		for (nn = 0; nn < n; ++nn)
			for (int i = 0; i < SIZE; ++i)
			{
				put(bufs1[i], sizeof(*bufs1)/sizeof(**bufs1),
					strs1[i], sizeof(*strs1)/sizeof(**strs1));
			}
		timer.finish();
		timer.count = nn;
		cout << timer << endl;
	}

	{
		cout << "put with memcpy(): " << flush;
		
		timer.restart();
		for (nn = 0; nn < n; ++nn)
			for (int i = 0; i < SIZE; ++i)
			{
				put2(bufs1[i], sizeof(*bufs1)/sizeof(**bufs1),
					strs1[i], sizeof(*strs1)/sizeof(**strs1));
			}
		timer.finish();
		timer.count = nn;
		cout << timer << endl;
	}

	{
		cout << "put with copy():   " << flush;
		
		timer.restart();
		for (nn = 0; nn < n; ++nn)
			for (int i = 0; i < SIZE; ++i)
			{
				put3(bufs1[i], sizeof(*bufs1)/sizeof(**bufs1),
					strs1[i], sizeof(*strs1)/sizeof(**strs1));
			}
		timer.finish();
		timer.count = nn;
		cout << timer << endl;
	}

	cout << "\n*** " << sizeof(*strs1)/sizeof(**strs1)
		<< " -> " << sizeof(*bufs2)/sizeof(**bufs2) << " ***\n\n";

	{
		cout << "put with while():  " << flush;
		
		timer.restart();
		for (nn = 0; nn < n; ++nn)
			for (int i = 0; i < SIZE; ++i)
			{
				put(bufs2[i], sizeof(*bufs2)/sizeof(**bufs2),
					strs1[i], sizeof(*strs1)/sizeof(**strs1));
			}
		timer.finish();
		timer.count = nn;
		cout << timer << endl;
	}

	{
		cout << "put with memcpy(): " << flush;
		
		timer.restart();
		for (nn = 0; nn < n; ++nn)
			for (int i = 0; i < SIZE; ++i)
			{
				put2(bufs2[i], sizeof(*bufs2)/sizeof(**bufs2),
					strs1[i], sizeof(*strs1)/sizeof(**strs1));
			}
		timer.finish();
		timer.count = nn;
		cout << timer << endl;
	}

	{
		cout << "put with copy():   " << flush;
		
		timer.restart();
		for (nn = 0; nn < n; ++nn)
			for (int i = 0; i < SIZE; ++i)
			{
				put3(bufs2[i], sizeof(*bufs2)/sizeof(**bufs2),
					strs1[i], sizeof(*strs1)/sizeof(**strs1));
			}
		timer.finish();
		timer.count = nn;
		cout << timer << endl;
	}

	cout << "\n*** " << sizeof(*strs2)/sizeof(**strs2)
		<< " -> " << sizeof(*bufs3)/sizeof(**bufs3) << " ***\n\n";

	{
		cout << "put with while():  " << flush;
		
		timer.restart();
		for (nn = 0; nn < n/10; ++nn)
			for (int i = 0; i < SIZE; ++i)
			{
				put(bufs3[i], sizeof(*bufs3)/sizeof(**bufs3),
					strs2[i], sizeof(*strs2)/sizeof(**strs2));
			}
		timer.finish();
		timer.count = nn;
		cout << timer << endl;
	}

	{
		cout << "put with memcpy(): " << flush;
		
		timer.restart();
		for (nn = 0; nn < n/10; ++nn)
			for (int i = 0; i < SIZE; ++i)
			{
				put2(bufs3[i], sizeof(*bufs3)/sizeof(**bufs3),
					strs2[i], sizeof(*strs2)/sizeof(**strs2));
			}
		timer.finish();
		timer.count = nn;
		cout << timer << endl;
	}

	{
		cout << "put with copy():   " << flush;
		
		timer.restart();
		for (nn = 0; nn < n/10; ++nn)
			for (int i = 0; i < SIZE; ++i)
			{
				put3(bufs3[i], sizeof(*bufs3)/sizeof(**bufs3),
					strs2[i], sizeof(*strs2)/sizeof(**strs2));
			}
		timer.finish();
		timer.count = nn;
		cout << timer << endl;
	}

	return 0;
}
