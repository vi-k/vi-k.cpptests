#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

#define MUL10( a ) (((a) << 3 ) + ((a) << 1 ))

char* i_to_s(int v, char *str)
{
	static const char sym[]
		= { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

    if (v < 0)
    {
        *str++ = '-';
        //v = -v;
    }

	char *ptr = str;

	while (v)
	{
		int n = v / 10;
		int o = v - (n<<3) - (n<<1); /* -(n * 10) */
		*ptr++ = sym[o];
		v = n;
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

void itoa10( int a, char *s )
{
    unsigned int b = a;
    char *p;

    if ( a < 0 )
    {
        *s++ = '-';
        b = -a;
    }

    p = s;

    do
    {
        unsigned int d = ( b * 0x6667 ) >> 18; // magic
        *p++ = b - MUL10( d ) + '0';
        b = d;
    }
    while ( b );

    *p-- = '\0';

    // inverse
    while ( s < p ) 
        *p-- ^= *s++ ^= *p ^= *s;
}

int main(void)
{
    char buf[20];
    itoa10(123456789, buf);
    cout << buf << endl;

    i_to_s(2147483647, buf);
    cout << buf << endl;

    i_to_s(-2147483647, buf);
    cout << buf << endl;

    return 0;
}