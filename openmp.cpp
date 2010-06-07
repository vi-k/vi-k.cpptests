/*
	Ќахождение простых чисел
	ѕроверка скорости работы с использованием
	и без использовани€ OpenMP
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <iostream>
using namespace std;

#include "../whoisalive/common/my_stopwatch.h"

#define COUNT 10000

int main(int argc, char *argv[])
{ 
	my::stopwatch timer(my::stopwatch::show_total);

	int n, ni;
	int start, end;             /* range of numbers to search */
	int print_primes = 0;       /* should each prime be printed? */ 
	int number_of_primes;       /* number of primes found */
	int number_of_41primes;     /* number of 4n+1 primes found */
	int number_of_43primes;     /* number of 4n-1 primes found */

	if (argc < 4)
	{
		printf("Usage: n openmp.exe start end [show_primes]\n");
		return 1;
	}

	n = atoi(argv[1]);
	start = atoi(argv[2]);
	end = atoi(argv[3]);

	if (!(start % 2))
		start++;

	if (argc == 5 && atoi(argv[4]) != 0)
		print_primes = 1;

	printf("Range to check for Primes: %d - %d\n",start, end);

	cout << "\nw/o openmp:  ";
	timer.reset();
	timer.start();
	for(ni = 0; ni < n; ++ni)
	{
		int i;
		number_of_primes = 0;
		number_of_41primes = 0;
		number_of_43primes = 0;

		for(i = start; i <= end; i += 2)
		{
			int limit = (int) sqrt((float)i) + 1;
			int prime = 1; /* assume number is prime */
			int j = 3;

			while (prime && (j <= limit))
			{
				if (i%j == 0)
					prime = 0;
				j += 2;
			}

			if (prime)
			{
		 		if (print_primes)
	 				printf("%5d is prime\n",i);

				number_of_primes++;

				if (i%4 == 1)
			  		number_of_41primes++;
				if (i%4 == 3)
					number_of_43primes++;
			}
		}
	}
	timer.finish();
	cout << timer << endl;

	printf("%d primes found\n",number_of_primes);
	printf("Number of 4n+1 primes found: %d\n",number_of_41primes);
	printf("Number of 4n-1 primes found: %d\n",number_of_43primes);

	cout << "\nwith openmp: ";

	#ifndef _OPENMP
	cout << "key '/openmp' not defined\n";
	#else
	timer.reset();
	timer.start();
	for(ni = 0; ni < n; ++ni)
	{
		int i;
		number_of_primes = 0;
		number_of_41primes = 0;
		number_of_43primes = 0;

		#pragma omp parallel for schedule(dynamic,100) reduction(+:number_of_primes,number_of_41primes,number_of_43primes)
		for(i = start; i <= end; i += 2)
		{
			int limit = (int) sqrt((float)i) + 1;
			int prime = 1; /* assume number is prime */
			int j = 3;

			while (prime && (j <= limit))
			{
				if (i%j == 0)
					prime = 0;
				j += 2;
			}

			if (prime)
			{
		 		if (print_primes)
	 				printf("%5d is prime\n",i);

				number_of_primes++;

				if (i%4 == 1)
			  		number_of_41primes++;
				if (i%4 == 3)
					number_of_43primes++;
			}
		}
	}
	timer.finish();
	cout << timer << endl;

	printf("%d primes found\n",number_of_primes);
	printf("Number of 4n+1 primes found: %d\n",number_of_41primes);
	printf("Number of 4n-1 primes found: %d\n",number_of_43primes);
	#endif

	return 0;
}
