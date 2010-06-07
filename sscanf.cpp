/*
	Тест скорости распознования ip-адреса
	1) std::iostream
	2) sscanf
	3) boost::spirit
	4) boost::spirit::qi
*/

//#define BOOST_NO_MEMBER_TEMPLATE_FRIENDS
//#define BOOST_OLD_IOSTREAMS

#include <stdlib.h> /* atoi() */
#include <string.h> /* strlen() */
#include <boost/spirit/include/classic_core.hpp> /* boost::spirit */
#include <boost/spirit/include/qi.hpp> /* boost::spirit:qi */

/* Boost'овское время (и чтоб без подключения библиотек) */
#include <libs/date_time/src/posix_time/posix_time_types.cpp>
#include <libs/date_time/src/gregorian/date_generators.cpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <iostream> /* cout, wcout */
using namespace std;


#define SIZE 100000
unsigned char g_num_array[SIZE][4];
char g_str_array[SIZE][16];

#define TEST


typedef void(*my_proc)(int n);

void process(my_proc proc, const wchar_t *text, int n) {
	using namespace boost::posix_time;

	ptime start;
	time_duration time;

	wcout << text << L" (" << n << L" раз): ";

	start = microsec_clock::local_time();
	proc(n); /* Цикл выношу в саму функцию, чтоб не терять время на вызовах */
	time = microsec_clock::local_time() - start;

	cout << to_simple_string(time) << endl;
}

void str_to_ip1(int n) {
	unsigned int a = 0, b = 0, c = 0, d = 0;
	char ch1, ch2, ch3;

	while (n--) {
		int index = rand() % SIZE;

		stringstream s;
		s << g_str_array[index];
		s >> a;
		s >> ch1;
		s >> b;
		s >> ch2;
		s >> c;
		s >> ch3;
		s >> d;
		
        #ifdef TEST
        if (a != (unsigned int)g_num_array[index][0]
        	&& b != (unsigned int)g_num_array[index][1]
        	&& c != (unsigned int)g_num_array[index][2]
        	&& d != (unsigned int)g_num_array[index][3]) {

        	wcout << L"Ошибка разбора. n=" << n << endl;
        }
        #endif
	}

	return;
}

void str_to_ip2(int n) {
	unsigned int a = 0, b = 0, c = 0, d = 0;

	while (n--) {
		int index = rand() % SIZE;

		//sscanf(g_str_array[index], "%u.%u.%u.%u", &a, &b, &c, &d);
		sscanf_s(g_str_array[index], "%u.%u.%u.%u", &a, &b, &c, &d);

        #ifdef TEST
        if (a != (unsigned int)g_num_array[index][0]
        	&& b != (unsigned int)g_num_array[index][1]
        	&& c != (unsigned int)g_num_array[index][2]
        	&& d != (unsigned int)g_num_array[index][3]) {

        	wcout << L"Ошибка разбора. n=" << n << endl;
        }
        #endif
	}

	return;
}

void str_to_ip3(int n) {
	unsigned int a = 0, b = 0, c = 0, d = 0;

    using namespace BOOST_SPIRIT_CLASSIC_NS;

	while (n--) {
		int index = rand() % SIZE;

		parse(g_str_array[index],
			uint_p[assign_a(a)] >> '.' >>
			uint_p[assign_a(b)] >> '.' >>
			uint_p[assign_a(c)] >> '.' >>
			uint_p[assign_a(d)],
			space_p);

        #ifdef TEST
        if (a != (unsigned int)g_num_array[index][0]
        	&& b != (unsigned int)g_num_array[index][1]
        	&& c != (unsigned int)g_num_array[index][2]
        	&& d != (unsigned int)g_num_array[index][3]) {

        	wcout << L"Ошибка разбора. n=" << n << endl;
        }
        #endif
	}

	return;
}

void str_to_ip4(int n) {
	unsigned int a = 0, b = 0, c = 0, d = 0;

	using namespace boost::spirit;

	while (n--) {
		int index = rand() % SIZE;
		const char *str = g_str_array[index];

		qi::parse(str, str + strlen(str),
			qi::int_ >> '.' >>
			qi::int_ >> '.' >>
			qi::int_ >> '.' >>
			qi::int_,
			a, b, c, d);

        #ifdef TEST
        if (a != (unsigned int)g_num_array[index][0]
        	&& b != (unsigned int)g_num_array[index][1]
        	&& c != (unsigned int)g_num_array[index][2]
        	&& d != (unsigned int)g_num_array[index][3]) {

        	wcout << L"Ошибка разбора. n=" << n << endl;
        }
        #endif
	}

	return;
}

int main(int argc, char *argv[])
{
	wcout.imbue( locale("Russian_Russia.866") );
	wcout << L"Разбор ip-адреса" << endl;

	int n = 100000;

	if (argc > 1) {
		n = atoi(argv[1]);
	}

	wcout << L"n=" << n << endl;

	if (n < RAND_MAX) {
		wcout << L"n < RAND_MAX" << endl;
		return 1;
	}

	for( int i = 0; i < SIZE; i++) {
		g_num_array[i][0] = (unsigned char)rand();
		g_num_array[i][1] = (unsigned char)rand();
		g_num_array[i][2] = (unsigned char)rand();
		g_num_array[i][3] = (unsigned char)rand();
		sprintf_s( g_str_array[i], sizeof(g_str_array[i]),
			"%u.%u.%u.%u",
			(unsigned int)g_num_array[i][0],
			(unsigned int)g_num_array[i][1],
			(unsigned int)g_num_array[i][2],
			(unsigned int)g_num_array[i][3]);
	}

	process( str_to_ip1, L"std::iostream (>>)", n);
	process( str_to_ip2, L"sscanf            ", n);
	process( str_to_ip3, L"boost::spirit     ", n);
	process( str_to_ip4, L"boost::spirit::qi ", n);

	return 0;
}
