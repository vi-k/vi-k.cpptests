#define BOOST_NO_MEMBER_TEMPLATE_FRIENDS
//#define BOOST_OLD_IOSTREAMS

////#define BOOST_PP_IS_ITERATING
////#define BOOST_HAS_DECLTYPE
////#include <boost/proto/transform/impl.hpp>

#include <string.h>
#include <boost/spirit/include/qi.hpp>
//using namespace boost::spirit;

#include <boost/spirit/include/classic_core.hpp>
////#include <boost/spirit/include/classic_push_back_actor.hpp>
////using namespace BOOST_SPIRIT_CLASSIC_NS;

#include <iostream>
#include <map>
#include <vector>
#include <string>
using namespace std;


#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <libs/date_time/src/posix_time/posix_time_types.cpp>
#include <libs/date_time/src/gregorian/date_generators.cpp>
//#include <libs/date_time/src/gregorian/greg_month.cpp>
//#include <libs/date_time/src/gregorian/greg_weekday.cpp>
//#include <libs/date_time/src/gregorian/gregorian_types.cpp>

//#include <boost/date_time/gregorian/gregorian.hpp>
//nclude <boost/date_time/gregorian/gregorian_io.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
////#include <boost/date_time/local_time/local_time.hpp>

typedef void(*my_proc)(const char*str, int n);

void process(my_proc proc, const wchar_t *text, const char *str, int n) {
	using namespace boost::posix_time;

	ptime start;
	time_duration time;

	wcout << text << L" (" << n << L" раз): ";

	start = microsec_clock::local_time();
	proc(str, n);
	/* Цикл выношу в саму функцию, чтоб не терять время на вызовах */
	time = microsec_clock::local_time() - start;

	cout << ": " << to_simple_string(time) << endl;
}

void str_to_ip1(const char *str, int n) {
	unsigned int a = 0, b = 0, c = 0, d = 0;
	char ch;

	while (n--) {
		stringstream s;
		s << str;
		s >> a;
		s >> ch;
		s >> b;
		s >> ch;
		s >> c;
		s >> ch;
		s >> d;
	}

	cout << a << "." << b << "." << c << "." << d;
	return;
}

void str_to_ip2(const char *str, int n) {
	unsigned int a = 0, b = 0, c = 0, d = 0;

	while (n--) {
		sscanf(str, "%u.%u.%u.%u", &a, &b, &c, &d);
	}

	cout << a << "." << b << "." << c << "." << d;
	return;
}

void str_to_ip3(const char *str, int n) {
	unsigned int a = 0, b = 0, c = 0, d = 0;

    using namespace BOOST_SPIRIT_CLASSIC_NS;
	rule<>r = uint_p[assign_a(a)] >> '.' >> uint_p[assign_a(b)] >> '.' >> uint_p
		[assign_a(c)] >> '.' >> uint_p[assign_a(d)];

	while (n--) {
		parse(str, r, space_p);
	}

	cout << a << "." << b << "." << c << "." << d;

	return;
}

void str_to_ip4(const char *str, int n) {
	unsigned int a = 0, b = 0, c = 0, d = 0;

	using namespace boost::spirit;

	while (n--) {
		qi::parse(str, str + strlen(str),
			qi::int_ >> '.' >>
			qi::int_ >> '.' >>
			qi::int_ >> '.' >>
			qi::int_,
			a, b, c, d);
	}

	cout << a << "." << b << "." << c << "." << d;
	return;
}

class test_t {
	private:
		bool _flag;
	public:
		test_t(bool flag):_flag(flag) {}
		void invert() { _flag = !_flag; }
};
std::vector<test_t> g_tests;

typedef void(*my_proc2)(int n);
void process2(my_proc2 proc, const wchar_t *text, int n) {
	using namespace boost::posix_time;

	ptime start;
	time_duration time;

	wcout << text << L" (" << n << L" раз): ";

	start = microsec_clock::local_time();
	proc(n);
	/* Цикл выношу в саму функцию, чтоб не терять время на вызовах */
	time = microsec_clock::local_time() - start;

	cout << ": " << to_simple_string(time) << endl;
}

void no_bind(int n) {
	while (n--) {
		for (vector<test_t>::iterator it = g_tests.begin();
				it!=g_tests.end(); ++it) {
			it->invert();
		}
	}
	return;
}

void no_bind2(int n) {
	while (n--) {
		vector<test_t>::iterator itE = g_tests.end();
		for (vector<test_t>::iterator it = g_tests.begin();
				it!=itE; ++it) {
			it->invert();
		}
	}
	return;
}

void std_bind(int n) {
	while (n--) {
		for_each( g_tests.begin(), g_tests.end(),
			mem_fun_ref(&test_t::invert));
	}
	return;
}

void std_bind2(int n) {
	mem_fun_ref_t<void,test_t> f
		= mem_fun_ref(&test_t::invert);
	while (n--) {
		for_each( g_tests.begin(), g_tests.end(), f);
	}
	return;
}

void boost_bind(int n) {
	while (n--) {
		for_each( g_tests.begin(), g_tests.end(),
			boost::bind(&test_t::invert, _1));
	}
	return;
}

typedef void (*invert_proc)(test_t *t);

void boost_bind2(int n) {
	boost::function<void (test_t&)> f
		= boost::bind(&test_t::invert, _1);
	while (n--) {
		for_each( g_tests.begin(), g_tests.end(), f);
	}
	return;
}

/* Проверка map'а */
struct macaddr_info {
	int a;
	int b;
	int c;
	macaddr_info() {}
	macaddr_info(int _a, int _b, int _c) : a(_a), b(_b), c(_c) {}
};

ostream& operator<<( ostream& s, macaddr_info &addr)
{
	s
		<< "a=" << addr.a
		<< ", b=" << addr.b
		<< ", c=" << addr.c;
	return s;
}

__int64 str2mac( const char *str)
{
	unsigned int a,b,c,d,e,f;
	if( sscanf( str, "%02X-%02X-%02X-%02X-%02X-%02X",
			&a, &b, &c, &d, &e, &f) != 6) {
		return 0LL;
	}

	unsigned char res[8];
	res[0] = (unsigned char)f;
	res[1] = (unsigned char)e;
	res[2] = (unsigned char)d;
	res[3] = (unsigned char)c;
	res[4] = (unsigned char)b;
	res[5] = (unsigned char)a;
	res[6] = 0;
	res[7] = 0;

	return *(__int64*)res;
}

const char *mac2str( __int64 addr, char *res)
{
	unsigned char *saddr = (unsigned char*)&addr;
	sprintf_s( res, 18, "%02X-%02X-%02X-%02X-%02X-%02X",
		(unsigned int)saddr[5],
		(unsigned int)saddr[4],
		(unsigned int)saddr[3],
		(unsigned int)saddr[2],
		(unsigned int)saddr[1],
		(unsigned int)saddr[0]);
	return res; /* Для удобства */
}

void print_all_macs( map<__int64,macaddr_info> &macs) {
	char macbuf[18];
	cout << "count=" << macs.size() << endl;
	for( map<__int64,macaddr_info>::iterator it = macs.begin();
			it != macs.end(); it++) {
		cout << mac2str(it->first,macbuf) << ": " << it->second << endl;
	}
	cout << endl;
}

int main(void) {
	wcout.imbue(locale("Russian_Russia.866"));
	wcout << L"Разбор ip-адреса" << endl;

	string str = "172.16.22.158";
	process( str_to_ip1, L"std::iostream (>>)", str.c_str(), 1000000);
	process( str_to_ip2, L"sscanf            ", str.c_str(), 1000000);
	process( str_to_ip3, L"boost::spirit     ", str.c_str(), 1000000);
	process( str_to_ip4, L"boost::spirit::qi ", str.c_str(), 1000000);

	g_tests.push_back(test_t(false));
	g_tests.push_back(test_t(true));
	g_tests.push_back(test_t(false));
	g_tests.push_back(test_t(true));

	process2( no_bind, L"no_bind", 1000000);         // no_bind (10000000 раз): : 00:00:00.312500
	process2( no_bind2, L"no_bind2", 1000000);       // no_bind2 (10000000 раз): : 00:00:00.312500
	process2( std_bind, L"std_bind", 1000000);       // std_bind (10000000 раз): : 00:00:00.578125
	process2( std_bind2, L"std_bind2", 1000000);     // std_bind2 (10000000 раз): : 00:00:00.656250
	process2( boost_bind, L"boost_bind", 1000000);   // boost_bind (10000000 раз): : 00:00:04.843750
	process2( boost_bind2, L"boost_bind2", 1000000); // boost_bind2 (10000000 раз): : 00:00:04.687500

	/* Проверка map'а */
	map<__int64,macaddr_info> macs;
	char macbuf[18];

	cout << endl;

	macs[ str2mac("00-1B-B9-99-0E-F2") ] = macaddr_info(1,2,3);
	print_all_macs( macs);

	macs[ str2mac("00-1B-B9-99-0E-F2") ] = macaddr_info(4,5,6);
	print_all_macs( macs);

	macs[ str2mac("AA-BB-CC-DD-EE-FF") ] = macaddr_info(7,8,9);
	print_all_macs( macs);

	return 0;
}
