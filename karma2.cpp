/*
	Тест скорости преобразования строки (percent_encode)
	1) my
	2) karma
	3) printf
*/

#include <stdlib.h> /* atoi() */
#include <string.h> /* strlen() */
#include <wchar.h> /* wcslen() */
#include <boost/spirit/include/karma.hpp>
namespace karma=boost::spirit::karma;

/* Boost'овское время (и чтоб без подключения библиотек) */
#include <libs/date_time/src/posix_time/posix_time_types.cpp>
#include <libs/date_time/src/gregorian/date_generators.cpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <iostream> /* cout, wcout */
#include <sstream>
using namespace std;

const char *g_escape_symbols;
string g_str_in;
string g_str_out; /* Глобальная, чтоб не было ненужной оптимизации */
string g_str_must;

/* Проверка правильности выполнения */
#define TEST 1


typedef void(*my_proc)(int n);

void process(my_proc proc, const wchar_t *text, int n)
{
	using namespace boost::posix_time;

	ptime start;
	time_duration time;

	wcout << text << L" (" << n << L" раз): ";

	start = microsec_clock::local_time();
	proc(n);
	time = microsec_clock::local_time() - start;

	cout << to_simple_string(time) << endl;
}

string percent_encode_my(const string &str, const char *escape_symbols)
{
	const char hex[17] = "0123456789abcdef";

	const char *ptr_in = str.c_str();
	/* Строка может содержать нулевой символ, поэтому
		пользуемся размером исходной строки */
	const char *end_in = ptr_in + str.size(); 

	/* Сразу готовим буфер */
	string out(str.size() * 3, ' ');
	char *begin_out = (char*)out.c_str();
	char *ptr_out = begin_out;

	while (ptr_in != end_in)
	{
		char ch = *ptr_in++;
        /* Кодируются все специальные символы, все не ascii-символы (>127),
        	пробел и заказынне пользователем */
		if (ch <= 32 || ch > 127 ||
			escape_symbols && strchr(escape_symbols, ch))
		{
			*ptr_out++ = '%';
			*ptr_out++ = hex[ ((unsigned char)ch) >> 4];
			*ptr_out++ = hex[ ch & 0xf ];
		}
		else
			*ptr_out++ = ch;
	}

	out.resize(ptr_out - begin_out);

	return out;
}

string percent_encode_karma(const string &str, const char *escape_symbols)
{
	const char *ptr_in = str.c_str();
	/* Строка может содержать нулевой символ, поэтому
		пользуемся размером исходной строки */
	const char *end_in = ptr_in + str.size(); 

	/* Сразу готовим буфер */
	string out(str.size() * 3, ' ');
	char *begin_out = (char*)out.c_str();
	char *ptr_out = begin_out;

	while (ptr_in != end_in)
	{
		char ch = *ptr_in++;
        /* Кодируются все специальные символы, все не ascii-символы (>127),
        	пробел и заказынне пользователем */
		if (ch <= 32 || ch > 127 ||
			escape_symbols && strchr(escape_symbols, ch))
				karma::generate(ptr_out,
					'%' <<karma::right_align(2,'0')[karma::hex], ch);
		else
			*ptr_out++ = ch;
	}

	out.resize(ptr_out - begin_out);

	return out;
}

string percent_encode_printf(const string &str, const char *escape_symbols)
{
	const char hex[17] = "0123456789abcdef";

	const char *ptr_in = str.c_str();
	/* Строка может содержать нулевой символ, поэтому
		пользуемся размером исходной строки */
	const char *end_in = ptr_in + str.size(); 

	/* Сразу готовим буфер */
	string out(str.size() * 3, ' ');
	char *begin_out = (char*)out.c_str();
	char *ptr_out = begin_out;

	while (ptr_in != end_in)
	{
		char ch = *ptr_in++;
        /* Кодируются все специальные символы, все не ascii-символы (>127),
        	пробел и заказынне пользователем */
		if (ch <= 32 || ch > 127 ||
			escape_symbols && strchr(escape_symbols, ch))
		{
			ptr_out += sprintf(ptr_out, "%%%02x",
				(unsigned int)(unsigned char)ch);
		}
		else
			*ptr_out++ = ch;
	}

	out.resize(ptr_out - begin_out);

	return out;
}

void encode_my(int n)
{

	while(n--)
	{
		g_str_out = percent_encode_my(g_str_in, g_escape_symbols);

		#if TEST
		if (g_str_out != g_str_must)
		{
        	wcout << L"Ошибка преобразования:\n";
        	cout << "out:  " << g_str_out << endl;
        	cout << "must: " << g_str_must << endl;
        	break;
		}
		#endif

	}
}

void encode_karma(int n)
{

	while(n--)
	{
		g_str_out = percent_encode_karma(g_str_in, g_escape_symbols);

		#if TEST
		if (g_str_out != g_str_must)
		{
        	wcout << L"Ошибка преобразования:\n";
        	cout << "out:  " << g_str_out << endl;
        	cout << "must: " << g_str_must << endl;
        	break;
		}
		#endif

	}
}

void encode_printf(int n)
{

	while(n--)
	{
		g_str_out = percent_encode_printf(g_str_in, g_escape_symbols);

		#if TEST
		if (g_str_out != g_str_must)
		{
        	wcout << L"Ошибка преобразования:\n";
        	cout << "out:  " << g_str_out << endl;
        	cout << "must: " << g_str_must << endl;
        	break;
		}
		#endif

	}
}


int main(int argc, char *argv[])
{
	wcout.imbue( locale("Russian_Russia.866") );
	wcout << L"percent_encode" << endl;

	int n = 100000;

	if (argc > 1)
	{
		n = atoi(argv[1]);
	}

	g_escape_symbols = "%#()";
	g_str_in = "Hello +%#()+ Привет";
	g_str_must = "Hello%20+%25%23%28%29+%20%cf%f0%e8%e2%e5%f2";

	cout << "in:   " << g_str_in << endl;

	process(encode_my,     L"my     ", n);
	process(encode_karma,  L"karma  ", n);
	process(encode_printf, L"printf ", n);

	return 0;
}
