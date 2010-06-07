#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

#include <libs/date_time/src/gregorian/date_generators.cpp>
#include <libs/date_time/src/gregorian/gregorian_types.cpp>
#include <libs/date_time/src/gregorian/greg_month.cpp>
#include <libs/date_time/src/gregorian/greg_names.hpp>
#include <libs/date_time/src/gregorian/greg_weekday.cpp>

#include <libs/date_time/src/posix_time/posix_time_types.cpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time;

#include <boost/unordered_map.hpp>
#include <boost/functional/hash.hpp>

/* Проверка map'а */

struct macaddr_info {
	int test;
	macaddr_info() {}
	macaddr_info(int _test) : test(_test) {}
};

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
	/* Чтобы не было никаких случайных выходов
		за пределы буфера */
	res[17] = 0;
	return res; /* Для удобства */
}

void print_all_macs( map<__int64,macaddr_info> &macs)
{
	char macbuf[18];
	cout << "count=" << macs.size() << endl;
	for( map<__int64,macaddr_info>::iterator it = macs.begin();
			it != macs.end(); it++) {
		cout << mac2str(it->first,macbuf) << ": " << it->second.test << endl;
	}
}

template<class T1,class T2>
void compare( T1 &m1, T2 &m2)
{
	if( m1.size() != m2.size()) {
		cout << " failed: sizes not equal" << endl;
		return;
	}

	for( T1::iterator i1 = m1.begin(); i1 != m1.end(); i1++) {
		if( i1->second.test != m2[ i1->first ].test) {
			cout << " failed: element not found" << endl;
			return;
		}
	}

	cout << " ok" << endl;
}

template<class T1,class T2>
void compare_s( T1 &m1, T2 &m2)
{
	if( m1.size() != m2.size()) {
		cout << " failed: sizes not equal" << endl;
		return;
	}

	for( T1::iterator i1 = m1.begin(); i1 != m1.end(); i1++) {
		if( i1->second.test != m2[ str2mac( i1->first.c_str()) ].test) {
			cout << " failed: element not found" << endl;
			return;
		}
	}

	cout << " ok" << endl;
}

int main(void) {
	/* mac в __int64 */
	map<__int64,macaddr_info> macs;
	char macbuf[18];

	cout << endl;

	macs[ str2mac("00-1B-B9-99-0E-F2") ] = macaddr_info(1);
	print_all_macs( macs);

	macs[ str2mac("00-1B-B9-99-0E-F2") ] = macaddr_info(2);
	print_all_macs( macs);

	macs[ str2mac("AA-BB-CC-DD-EE-FF") ] = macaddr_info(3);
	print_all_macs( macs);

	macs.clear();

	/* Генерируем список ip-адресов для теста */
	__int64 addr_list[100];
	string str_addr_list[100];
	for( size_t i = 0; i < sizeof(addr_list) / sizeof(addr_list[0]); i++) {
		((unsigned char*)&addr_list[i])[0] = (unsigned char)rand();
		((unsigned char*)&addr_list[i])[1] = (unsigned char)rand();
		((unsigned char*)&addr_list[i])[2] = (unsigned char)rand();
		((unsigned char*)&addr_list[i])[3] = (unsigned char)rand();
		((unsigned char*)&addr_list[i])[4] = (unsigned char)rand();
		((unsigned char*)&addr_list[i])[5] = (unsigned char)rand();
		str_addr_list[i] = mac2str( addr_list[i], macbuf);
	}

	macaddr_info mi[100];
	for( size_t i = 0; i < sizeof(mi) / sizeof(mi[0]); i++) {
		mi[i].test = rand();
	}

	cout << endl << "test" << endl;

	ptime start;
	time_duration time;
	const int count = 300000;
	int n;

	n = count;
	cout << "std::map                       (" << n << "): ";
	start = microsec_clock::local_time();
	while( n--) {
		for( size_t i = 0; i < sizeof(addr_list) / sizeof(addr_list[0]); i++) {
			macs[ addr_list[i] ] = mi[i];
		}
	}
	time = microsec_clock::local_time() - start;
	cout << to_simple_string(time) << endl;

	//print_all_macs( macs);


	boost::unordered_map<__int64,macaddr_info> boost_macs1;
	n = count;
	cout << "boost::unordered_map (default) (" << n << "): ";
	start = microsec_clock::local_time();
	while( n--) {
		for( size_t i = 0; i < sizeof(addr_list) / sizeof(addr_list[0]); i++) {
			boost_macs1[ addr_list[i] ] = mi[i];
		}
	}
	time = microsec_clock::local_time() - start;
	cout << to_simple_string(time);
	compare( macs, boost_macs1);
	
	boost::unordered_map<__int64,macaddr_info,boost::hash<__int64> > boost_macs2;
	n = count;
	cout << "boost::unordered_map (__int64) (" << n << "): ";
	start = microsec_clock::local_time();
	while( n--) {
		for( size_t i = 0; i < sizeof(addr_list) / sizeof(addr_list[0]); i++) {
			boost_macs2[ addr_list[i] ] = mi[i];
		}
	}
	time = microsec_clock::local_time() - start;
	cout << to_simple_string(time);
	compare( macs, boost_macs2);

	boost::unordered_map<__int64,macaddr_info,boost::hash<int> > boost_macs3;
	n = count;
	cout << "boost::unordered_map (int)     (" << n << "): ";
	start = microsec_clock::local_time();
	while( n--) {
		for( size_t i = 0; i < sizeof(addr_list) / sizeof(addr_list[0]); i++) {
			boost_macs3[ addr_list[i] ] = mi[i];
		}
	}
	time = microsec_clock::local_time() - start;
	cout << to_simple_string(time);
	compare( macs, boost_macs3);
	
	boost::unordered_map<__int64,macaddr_info,boost::hash<unsigned int> > boost_macs8;
	n = count;
	cout << "boost::unordered_map (uint)    (" << n << "): ";
	start = microsec_clock::local_time();
	while( n--) {
		for( size_t i = 0; i < sizeof(addr_list) / sizeof(addr_list[0]); i++) {
			boost_macs8[ addr_list[i] ] = mi[i];
		}
	}
	time = microsec_clock::local_time() - start;
	cout << to_simple_string(time);
	compare( macs, boost_macs8);


	boost::unordered_map<__int64,macaddr_info,boost::hash<unsigned short> > boost_macs9;
	n = count;
	cout << "boost::unordered_map (ushort)  (" << n << "): ";
	start = microsec_clock::local_time();
	while( n--) {
		for( size_t i = 0; i < sizeof(addr_list) / sizeof(addr_list[0]); i++) {
			boost_macs9[ addr_list[i] ] = mi[i];
		}
	}
	time = microsec_clock::local_time() - start;
	cout << to_simple_string(time);
	compare( macs, boost_macs9);


	boost::unordered_map<__int64,macaddr_info,boost::hash<char> > boost_macs4;
	n = count;
	cout << "boost::unordered_map (char)    (" << n << "): ";
	start = microsec_clock::local_time();
	while( n--) {
		for( size_t i = 0; i < sizeof(addr_list) / sizeof(addr_list[0]); i++) {
			boost_macs4[ addr_list[i] ] = mi[i];
		}
	}
	time = microsec_clock::local_time() - start;
	cout << to_simple_string(time);
	compare( macs, boost_macs4);

	boost::unordered_map<__int64,macaddr_info,boost::hash<unsigned char> > boost_macs6;
	n = count;
	cout << "boost::unordered_map (uchar)   (" << n << "): ";
	start = microsec_clock::local_time();
	while( n--) {
		for( size_t i = 0; i < sizeof(addr_list) / sizeof(addr_list[0]); i++) {
			boost_macs6[ addr_list[i] ] = mi[i];
		}
	}
	time = microsec_clock::local_time() - start;
	cout << to_simple_string(time);
	compare( macs, boost_macs6);


	string str;
	boost::unordered_map<string,macaddr_info> boost_macs_s;
	n = count;
	cout << "boost::unordered_map (string)  (" << n << "): ";
	start = microsec_clock::local_time();
	while( n--) {
		for( size_t i = 0; i < sizeof(addr_list) / sizeof(addr_list[0]); i++) {
			boost_macs_s[ str_addr_list[i] ] = mi[i];
		}
	}
	time = microsec_clock::local_time() - start;
	cout << to_simple_string(time);
	compare_s( boost_macs_s, macs);

	cout << endl;

	return 0;
}
