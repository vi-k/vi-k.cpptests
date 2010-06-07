#include <winsock.h>

#include <stdlib.h>
#include <iostream>
using namespace std;

#include <boost/unordered_map.hpp>
#include <boost/functional/hash.hpp>
#include <boost/foreach.hpp>

/* Проверка map'а */

struct st_with_map {
	int a;
	struct timeval st;
	boost::unordered_map<int,int> map;
	//st_with_map() : a(0) { st.tv_sec = 0; st.tv_usec = 0; }
};

int main(void)
{
	st_with_map a;
	st_with_map b;
	st_with_map c;
	
	a.a = 1;
	a.map[123] = 456;

	b.a = 2;
	b.map[123] = 456;
	b.map[456] = 789;

	c.a = 3;
	c.map[123] = 456;
	c.map[456] = 789;
	c.map[789] = 123;

	boost::unordered_map<int,st_with_map> map;

	map[0] = a;
	map[1] = b;
	map[2] = c;

	for ( boost::unordered_map<int,st_with_map>::iterator it = map.begin();
		it != map.end(); it++)
	{
		cout << "[" << it->first << "]" << endl;
		cout << "a: " << it->second.a << endl;
		cout << "map.size(): " << it->second.map.size() << endl;
		cout << endl;
	}

	return 0;
}
