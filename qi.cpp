#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

namespace qi=boost::spirit::qi;
namespace phoenix=boost::phoenix;
namespace karma=boost::spirit::karma;

#include <string.h>
#include <iostream>
#include <string>
#include <utility> /* std::pair */
using namespace std;

#include <boost/foreach.hpp>

string unescape(string &str)
{
	string out;

	/* Для нормального преобразования символов с кодами выше 127
		необходимо использовать unsigned char, поэтому оформляем
		это дело отдельным правилом */
	qi::rule<string::const_iterator, unsigned char()> char_r
		= qi::char_ - '%'
			| '%' >> qi::uint_parser<unsigned char, 16, 2, 2>()
			| qi::char_('%');

	qi::parse( str.begin(), str.end(),
		*char_r >> qi::eoi, out); /* eoi - указатель конца строки */

	return out;
}

string escape(const string &str,
	const char *escape_symbols = NULL)
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

int main()
{
	//string in = "ASDF-%41%53%44%46 \x8F\xE0\xA8\xA2\xA5\xE2-%8F%E0%A8%A2%A5%E2!!!";

    /* Проверка символов с кодами 0..31 */
 	//string in = "\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f";
 	//in.push_back('\0');
 	
 	/* Проверка символов ascii */
 	string in = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMOPQRSTUVWXYZ[\\]^_`abcdefghijklmopqrstuvwxyz{|}~ Привет";
	in = "Hello +%#()+ Привет";

	cout << "in:       \"" << in  << "\" (" << in.size() << ")" << endl;

	string out = in;

	//out = unescape(out);
	//cout << "unescape: \"" << out << "\" (" << out.size() << ")" << endl;

	out = escape(out, "%#()");
	cout << "escape:   \"" << out << "\" (" << out.size() << ")" << endl;

	out = unescape(out);
	cout << "unescape: \"" << out << "\" (" << out.size() << ")" << endl;

	return 0;
}
