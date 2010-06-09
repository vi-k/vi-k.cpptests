#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include <cstddef>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

template<class T, class Char>
std::size_t signed_to(T n, Char *str, std::size_t decimals = 0,
	std::size_t width = -1)
{
	static const Char sym[]
		= { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

	Char buf[64];
	Char *buf_ptr = buf;
	Char *str_ptr = str;
	bool neg;

    if ( (neg = n < 0) == false)
        n = -n;

	do
	{
		T nn = n / 10;
		*buf_ptr++ = sym[ (nn<<3) + (nn<<1) - n ];
		n = nn;
	} while (n);

	Char *buf_min = buf + decimals;
	while (buf_ptr < buf_min)
		*buf_ptr++ = '0';

	if (buf_ptr - buf + neg > width)
	{
		Char *str_end = str + width;
		while (str_ptr != str_end)
   		    *str_ptr++ = '#';
   	}
   	else
   	{
	    if (neg)
    	    *str_ptr++ = '-';

		while (buf_ptr != buf)
   		    *str_ptr++ = *--buf_ptr;
   	}

    *str_ptr = 0;

	return str_ptr - str;
}

template<class T, class Char>
std::size_t unsigned_to(T n, Char *str, std::size_t decimals = 0,
	std::size_t width = -1)
{
	static const Char sym[]
		= { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

	Char buf[64];
	Char *buf_ptr = buf;
	Char *str_ptr = str;

	do
	{
		T nn = n / 10;
		*buf_ptr++ = sym[ n - (nn<<3) - (nn<<1) ];
		n = nn;
	} while (n);

	Char *buf_min = buf + decimals;
	while (buf_ptr < buf_min)
		*buf_ptr++ = '0';

	if (buf_ptr - buf > width)
	{
		Char *str_end = str + width;
		while (str_ptr != str_end)
   		    *str_ptr++ = '#';
   	}
   	else
   	{
		while (buf_ptr != buf)
   		    *str_ptr++ = *--buf_ptr;
   	}

    *str_ptr = 0;

	return str_ptr - str;
}

template<class T, class Char>
Char* unsigned_to2(T n, Char *str)
{
	static const Char sym[]
		= { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

	char *ptr = str;

	while (n)
	{
		T nn = n / 10;
		T o = n - (nn<<3) - (nn<<1);
		*ptr++ = sym[o];
		n = nn;
	}

	*ptr-- = 0;

    while (str < ptr)
   	{
        char tmp = *ptr;
        *ptr-- = *str;
        *str++ = tmp;
    }

	return ptr;
}

int main(void)
{
#define TEST_S(TN,T) {\
	cout << "type=" << TN << endl;\
    char buf[64];\
    T n;\
    n = 0;\
    signed_to(n, buf);\
    cout << n << " = " << buf << endl;\
    n = -1;\
    signed_to(n, buf);\
    cout << n << " = " << buf << endl;\
    n = (unsigned T)n >> 1;\
    signed_to(n, buf);\
    cout << n << " = " << buf << endl;\
    n = -n - 1;\
    signed_to(n, buf);\
    cout << n << " = " << buf << endl;\
    n = 123;\
    signed_to(n, buf);\
    cout << n << " = " << buf << endl;\
    n = 123;\
    signed_to(n, buf, 5);\
    cout << n << "[5] = " << buf << endl;\
    n = -123;\
    signed_to(n, buf, 5);\
    cout << n << "[5] = " << buf << endl;\
    n = 123;\
    signed_to(n, buf, 0, 3);\
    cout << n << "[0,3] = " << buf << endl;\
    n = 123;\
    signed_to(n, buf, 0, 2);\
    cout << n << "[0,2] = " << buf << endl;\
    n = -123;\
    signed_to(n, buf, 0, 3);\
    cout << n << "[0,3] = " << buf << endl;\
    n = -123;\
    signed_to(n, buf, 0, 4);\
    cout << n << "[0,4] = " << buf << endl;\
    n = 123;\
    signed_to(n, buf, 5, 4);\
    cout << n << "[5,4] = " << buf << endl;\
    n = 123;\
    signed_to(n, buf, 5, 5);\
    cout << n << "[5,5] = " << buf << endl;\
    n = -123;\
    signed_to(n, buf, 5, 5);\
    cout << n << "[5,5] = " << buf << endl;\
    n = -123;\
    signed_to(n, buf, 5, 6);\
    cout << n << "[5,6] = " << buf << endl;\
    cout << endl;}

#define TEST_U(TN,T) {\
	cout << "type=" << TN << endl;\
    char buf[64];\
    T n;\
    n = 0;\
    unsigned_to(n, buf);\
    cout << n << " = " << buf << endl;\
    n = -1;\
    unsigned_to(n, buf);\
    cout << n << " = " << buf << endl;\
    n = 123;\
    unsigned_to(n, buf);\
    cout << n << " = " << buf << endl;\
    n = 123;\
    unsigned_to(n, buf, 5);\
    cout << n << "[5] = " << buf << endl;\
    n = 123;\
    unsigned_to(n, buf, 0, 3);\
    cout << n << "[0,3] = " << buf << endl;\
    n = 123;\
    unsigned_to(n, buf, 0, 2);\
    cout << n << "[0,2] = " << buf << endl;\
    n = 123;\
    unsigned_to(n, buf, 5, 4);\
    cout << n << "[5,4] = " << buf << endl;\
    n = 123;\
    unsigned_to(n, buf, 5, 5);\
    cout << n << "[5,5] = " << buf << endl;\
    cout << endl;}

    TEST_S("short",short)
    TEST_S("int",int)
    TEST_S("long",long)
    TEST_S("longlong",long long)

    char buf[64];
    unsigned int n= 123;
    unsigned_to(n, buf, 5, 5);
    cout << n << "[5,5] = " << buf << endl;\

    /*-

    TEST_S("ushort",unsigned short)
    TEST_S("uint",unsigned int)
    TEST_S("ulong",unsigned long)
    TEST_S("ulonglong",unsigned long long)
    -*/

    return 0;
}