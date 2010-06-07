/*
	Тест скорости распознавания числа (int)
	1) atoi
	2) sscanf
	3) stringstream
	4) lexical_cast
	5) boost::spirit
	6) boost::spirit::qi
	7) boost::spirit::qi - через вызов функции
*/

#include <stdlib.h> /* atoi() */
#include <string.h> /* strlen() */
#include <wchar.h> /* wcslen() */
#include <boost/spirit/include/classic_core.hpp> /* boost::spirit */
#include <boost/spirit/include/qi.hpp> /* boost::spirit:qi */
#include <boost/lexical_cast.hpp>

/* Boost'овское время (и чтоб без подключения библиотек) */
#include <libs/date_time/src/posix_time/posix_time_types.cpp>
#include <libs/date_time/src/gregorian/date_generators.cpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <iostream> /* cout, wcout */
#include <sstream>
using namespace std;


#define SIZE 100000
int a; /* Глобальная, чтоб не было ненужной оптимизации */
int g_num_array[SIZE];
char g_str_array[SIZE][20];
string g_string_array[SIZE];

/* Проверка правильности выполнения */
#define TEST 0
#define RUN_SLOW 1

template<class Result, class Rule>
Result to_num(const string &str, Rule rule, Result def)
{
	const char *ptr = str.c_str();
	boost::spirit::qi::parse(ptr, ptr + strlen(ptr), rule, def);
	return def;
}

template<class Result, class Rule>
Result to_num(const wstring &str, Rule rule, Result def)
{
	const wchar_t *ptr = str.c_str();
	boost::spirit::qi::parse(ptr, ptr + wcslen(ptr), rule, def);
	return def;
}

template<class Result, class Rule>
Result to_num(const char *str, Rule rule, Result def)
{
	boost::spirit::qi::parse(str, str + strlen(str), rule, def);
	return def;
}

template<class Result, class Rule>
Result to_num(const wchar_t *str, Rule rule, Result def)
{
	boost::spirit::qi::parse(str, str + wcslen(str), rule, def);
	return def;
}



template<class Char>
int to_int(const basic_string<Char> &str, int def)
{
	return to_num(str, boost::spirit::qi::int_, def);
}

template<class Char>
int to_int(const Char *str, int def)
{
	return to_num(str, boost::spirit::qi::int_, def);
}

typedef void(*my_proc)();

void process(my_proc proc, const wchar_t *text, int n)
{
	using namespace boost::posix_time;

	ptime start;
	time_duration time;

	wcout << text << L" (" << n * SIZE << L" раз): ";

	start = microsec_clock::local_time();

	while (n--) proc();

	time = microsec_clock::local_time() - start;

	cout << to_simple_string(time) << endl;
}

void int_to_str1()
{
	int index = 0;

	do
	{
		a = atoi(g_str_array[index]);
			
		#if TEST
		if (g_num_array[index] != a)
		{
        	wcout << L"Ошибка разбора:\n";
        	wcout << L"index=" << index << endl;
        	wcout << L"a=" << a << endl;
        	wcout << L"num[index]=" << g_num_array[index] << endl;
        	wcout << L"str[index]=\"" << g_str_array[index] << "\"\n";
        	break;
		}
		#endif

	} while (++index < SIZE);
}

void int_to_str2()
{
	int index = 0;

	do
	{
		sscanf(g_str_array[index], "%d", &a);
			
		#if TEST
		if (g_num_array[index] != a)
		{
        	wcout << L"Ошибка разбора:\n";
        	wcout << L"index=" << index << endl;
        	wcout << L"a=" << a << endl;
        	wcout << L"num[index]=" << g_num_array[index] << endl;
        	wcout << L"str[index]=\"" << g_str_array[index] << "\"\n";
        	break;
		}
		#endif

	} while (++index < SIZE);
}

void int_to_str3()
{
	int index = 0;

	do
	{
		stringstream ss;
		ss << (char*)g_str_array[index];
		ss >> a;
			
		#if TEST
		if (g_num_array[index] != a)
		{
        	wcout << L"Ошибка разбора:\n";
        	wcout << L"index=" << index << endl;
        	wcout << L"a=" << a << endl;
        	wcout << L"ss=\"" << ss.str().c_str() << "\"\n";
        	wcout << L"num[index]=" << g_num_array[index] << endl;
        	wcout << L"str[index]=\"" << g_str_array[index] << "\"\n";
        	break;
		}
		#endif

	} while (++index < SIZE);
}

void int_to_str4()
{
	int index = 0;

	do
	{
		using namespace boost::spirit;

		a = boost::lexical_cast<int>(g_str_array[index]);

		#if TEST
		if (g_num_array[index] != a)
		{
        	wcout << L"Ошибка разбора:\n";
        	wcout << L"index=" << index << endl;
        	wcout << L"a=" << a << endl;
        	wcout << L"num[index]=" << g_num_array[index] << endl;
        	wcout << L"str[index]=\"" << g_str_array[index] << "\"\n";
        	break;
		}
		#endif

	} while (++index < SIZE);
}

void int_to_str5()
{
	int index = 0;

	do
	{
	    using namespace BOOST_SPIRIT_CLASSIC_NS;

		parse(g_str_array[index],
			int_p[assign_a(a)], space_p);

		#if TEST
		if (g_num_array[index] != a)
		{
        	wcout << L"Ошибка разбора:\n";
        	wcout << L"index=" << index << endl;
        	wcout << L"a=" << a << endl;
        	wcout << L"num[index]=" << g_num_array[index] << endl;
        	wcout << L"str[index]=\"" << g_str_array[index] << "\"\n";
        	break;
		}
		#endif

	} while (++index < SIZE);
}

void int_to_str6()
{
	int index = 0;

	do
	{
		using namespace boost::spirit;

		const char *str = g_str_array[index];

		qi::parse(str, str + strlen(str),
			qi::int_, a);

		#if TEST
		if (g_num_array[index] != a)
		{
        	wcout << L"Ошибка разбора:\n";
        	wcout << L"index=" << index << endl;
        	wcout << L"a=" << a << endl;
        	wcout << L"num[index]=" << g_num_array[index] << endl;
        	wcout << L"str[index]=\"" << g_str_array[index] << "\"\n";
        	break;
		}
		#endif

	} while (++index < SIZE);
}

void int_to_str7()
{
	int index = 0;

	do
	{
		using namespace boost::spirit;

		a = to_int(g_str_array[index], 0);

		#if TEST
		if (g_num_array[index] != a)
		{
        	wcout << L"Ошибка разбора:\n";
        	wcout << L"index=" << index << endl;
        	wcout << L"a=" << a << endl;
        	wcout << L"num[index]=" << g_num_array[index] << endl;
        	wcout << L"str[index]=\"" << g_str_array[index] << "\"\n";
        	break;
		}
		#endif

	} while (++index < SIZE);
}

void int_to_str7a()
{
	int index = 0;

	do
	{
		using namespace boost::spirit;

		a = to_int(g_string_array[index], 0);

		#if TEST
		if (g_num_array[index] != a)
		{
        	wcout << L"Ошибка разбора:\n";
        	wcout << L"index=" << index << endl;
        	wcout << L"a=" << a << endl;
        	wcout << L"num[index]=" << g_num_array[index] << endl;
        	wcout << L"str[index]=\"" << g_str_array[index] << "\"\n";
        	break;
		}
		#endif

	} while (++index < SIZE);
}

int main(int argc, char *argv[])
{
	wcout.imbue( locale("Russian_Russia.866") );
	wcout << L"str_to_int" << endl;

	int n = 10;

	if (argc > 1)
	{
		n = atoi(argv[1]);
	}

	wcout << L"n=" << n << endl;

	for (int i = 0; i < SIZE; i++)
	{	
		int n = rand();
		g_num_array[i] = n;
		sprintf_s(g_str_array[i], sizeof(g_str_array[i]),
			"%d", n);
		g_string_array[i] = g_str_array[i];
	}

	process(int_to_str1, L"atoi          ", n);
	#if RUN_SLOW
	process(int_to_str2, L"sscanf        ", n);
	process(int_to_str3, L"stringstream  ", n);
	process(int_to_str4, L"lexical_cast  ", n);
	process(int_to_str5, L"spirit        ", n);
	#endif
	process(int_to_str6, L"spirit::qi    ", n);
	process(int_to_str7, L"spirit::qi(*) ", n);
	process(int_to_str7a, L"spirit::qi(&) ", n);

	return 0;
}
