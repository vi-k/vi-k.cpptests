#include <stdio.h>
#include <math.h>

#define PI 3.1415926535897932384626433832795
#define EXCT 0.081819790992

double lon2x(double lon)
{
	return 128.0 + lon / 180.0 * 128.0;
}

inline double atanh(double v)
{
	return 0.5 * log( (1.0 + v) / (1.0 - v) );
}

double lat2y(double lat, int type)
{
	double s = sin( lat * PI / 180.0 );
	double y;

	switch (type) {
		case 1:
			y = 128 * (1 - atanh(s) / PI);
			break;

		case 2:
			y = 128 * (1 - (atanh(s) - EXCT * atanh(EXCT * s)) / PI);
			break;
	}

	return y;
}

void ll2xy(double lon, double lat, int type)
{
	printf( "%d: ", type);
	printf( "x = %5.1lf / %5.1lf; ", lon, lon2x(lon));
	printf( "y = %5.1lf / %5.1lf\n", lat, lat2y(lat,type));
}

double d2dd(double d, double m, double s)
{
	return d + m / 60.0 + s / 3600.0;
}

void dd2d(double d)
{
	double m = (d - (double)(int)d) * 60.0;
	double s = (m - (double)(int)m) * 60.0;
	printf( "%d° %d' %04.2lf\"\n", (int)d, (int)m, s);
}

int main(void)
{
	ll2xy(0, 90, 1);
	ll2xy(0, 90, 2);

	ll2xy( d2dd(135,4,58), d2dd(85,3,4.0636073037), 1 );
	ll2xy( d2dd(135,4,58), d2dd(85,3,4.0636073037), 2 );
	
	ll2xy(0, 80, 1);
	ll2xy(0, 80, 2);

	ll2xy(0, 50, 1);
	ll2xy(0, 50, 2);

	ll2xy(0, 20, 1);
	ll2xy(0, 20, 2);

	ll2xy(0, 0, 1);
	ll2xy(0, 0, 2);

	ll2xy( d2dd(135,4,58), d2dd(48,28,39.52), 1 );
	ll2xy( d2dd(135,4,58), d2dd(48,28,39.52), 2 );

    /* 224.1, 88.7 */

	double sum1 = 0, sum2 = 0;

	sum1 += d2dd(129,41,55.05);sum2 += d2dd(62,2,44.06);
	sum1 += d2dd(150,48,29.00);sum2 += d2dd(59,33,15.00);
	sum1 += d2dd(158,31,13.18);sum2 += d2dd(53,7,16.11);
	sum1 += d2dd(134,6,48.27);sum2 += d2dd(48,0,59.01);

	dd2d(sum1 / 4);
	dd2d(sum2 / 4);


	
	return 0;
}
