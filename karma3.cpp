/*
	Тест скорости преобразования строки (escape)
	1) my
	2) karma
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

string escape_my(const string &str)
{
	const char hex[17] = "0123456789abcdef";
	const char *esc[] = {"\\0", "\\x01", "\\x02", "\\x03",
		"\\x04", "\\x05", "\\x06", "\\a", "\\b", "\\t", "\\n",
		"\\v", "\\f", "\\r", "\\x0e", "\\x0f", "\\x10",
		"\\x11", "\\x12", "\\x13", "\\x14", "\\x15", "\\x16",
		"\\x17", "\\x18", "\\x19", "\\x1a", "\\x1b", "\\x1c",
		"\\x1d", "\\x1e", "\\x1f"};

	size_t len = str.size();

	const char *ptr_in = str.c_str();
	/* Строка может содержать нулевой символ, поэтому
		пользуемся размером исходной строки */
	const char *end_in = ptr_in + len; 

	/* Сразу готовим буфер */
	string out(len * 4, ' ');
	char *begin_out = (char*)out.c_str();
	char *ptr_out = begin_out;

	while (ptr_in != end_in)
	{
		char ch = *ptr_in++;
        /* Кодируются все специальные символы, все не ascii-символы (>127),
        	пробел и заказынне пользователем */
		if (ch >= 0 && ch < 32)
		{
			const char *val = esc[ch];
			while (*val)
				*ptr_out++ = *val++;
		}
		else
		{
			if (ch == '\'' || ch == '\"' || ch == '\\')
				*ptr_out++ = '\\';

			*ptr_out++ = ch;
		}
	}

	out.resize(ptr_out - begin_out);

	return out;
}

string escape_karma(const string &str)
{
	size_t len = str.size();
	string out(len * 4, ' ');

	karma::symbols<char, char const*> esc_char;

	esc_char.add
		('\0', "\\0")('\x01', "\\x01")('\x02', "\\x02")('\x03', "\\x03")
		('\x04', "\\x04")('\x05', "\\x05")('\x06', "\\x06")
		('\a', "\\a")('\b', "\\b")('\t', "\\t")('\n', "\\n")
		('\v', "\\v")('\f', "\\f")('\r', "\\r")
		('\x0e', "\\x0e")('\x0f', "\\x0f")('\x10', "\\x10")
		('\x11', "\\x11")('\x12', "\\x12")('\x13', "\\x13")
		('\x14', "\\x14")('\x15', "\\x15")('\x16', "\\x16")
		('\x17', "\\x17")('\x18', "\\x18")('\x19', "\\x19")
		('\x1a', "\\x1a")('\x1b', "\\x1b")('\x1c', "\\x1c")
		('\x1d', "\\x1d")('\x1e', "\\x1e")('\x1f', "\\x1f")
		('\'', "\\\'")('\"', "\\\"")('\\', "\\\\");

	char *begin_out = (char*)out.c_str();
	char *ptr_out = begin_out;

	karma::generate(ptr_out, *(esc_char | karma::char_), str);

	out.resize(ptr_out - begin_out);

	return out;
}

karma::symbols<char, char const*> g_esc_char;
string escape_karma2(const string &str)
{
	size_t len = str.size();
	string out(len * 4, ' ');

	char *begin_out = (char*)out.c_str();
	char *ptr_out = begin_out;

	karma::generate(ptr_out, *(g_esc_char | karma::char_), str);

	out.resize(ptr_out - begin_out);

	return out;
}

void encode_my(int n)
{

	while(n--)
	{
		g_str_out = escape_my(g_str_in);

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
		g_str_out = escape_karma(g_str_in);

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

void encode_karma2(int n)
{

	while(n--)
	{
		g_str_out = escape_karma2(g_str_in);

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
	wcout << L"escape" << endl;

	int n = 100000;

	if (argc > 1)
	{
		n = atoi(argv[1]);
	}

	g_escape_symbols = "%#()";
	g_str_in = "Hello \a\b\r\n\t\f\'\"\x1F\\ Привет";
	g_str_must = "Hello \\0\\a\\b\\r\\n\\t\\f\\\'\\\"\\x1f\\\\ Привет";
	g_str_in.insert(6, 1, '\0');

	//cout << "in:   " << g_str_in << endl;

	g_esc_char.add
		('\0', "\\0")('\x01', "\\x01")('\x02', "\\x02")('\x03', "\\x03")
		('\x04', "\\x04")('\x05', "\\x05")('\x06', "\\x06")
		('\a', "\\a")('\b', "\\b")('\t', "\\t")('\n', "\\n")
		('\v', "\\v")('\f', "\\f")('\r', "\\r")
		('\x0e', "\\x0e")('\x0f', "\\x0f")('\x10', "\\x10")
		('\x11', "\\x11")('\x12', "\\x12")('\x13', "\\x13")
		('\x14', "\\x14")('\x15', "\\x15")('\x16', "\\x16")
		('\x17', "\\x17")('\x18', "\\x18")('\x19', "\\x19")
		('\x1a', "\\x1a")('\x1b', "\\x1b")('\x1c', "\\x1c")
		('\x1d', "\\x1d")('\x1e', "\\x1e")('\x1f', "\\x1f")
		('\'', "\\\'")('\"', "\\\"")('\\', "\\\\");

	process(encode_my,     L"my     ", n);
	process(encode_karma,  L"karma  ", n);
	process(encode_karma2, L"karma2 ", n);

	cout << "out:  " << g_str_out << endl;

	return 0;
}
