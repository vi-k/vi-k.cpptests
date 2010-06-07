#include <stdlib.h>

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#include "../whoisalive/common/my_stopwatch.h"

#define SIZE 500


template<class Char>
struct random_fill
{
	basic_string<Char> operator()() const
	{
		size_t len = rand() % 10000 + 1;
		basic_string<Char> str(len, Char(' '));

		int max_rand = (1 << (8 * sizeof(Char) - 1)) - 1;

		for (int i = 0; i < len; i++)
        {
			Char ch = Char(rand() % max_rand + 1);
			str[i] = ch;
		}

		//cout << '[' << str.size() << ']' << endl;

		return str;
	}
};

template<class Char>
void test(const vector<basic_string<Char> > &v1,
	const vector<basic_string<Char> > &v2)
{
	if (v1.size() != v2.size())
	{
		cout << "test failed - sizes not eq" << endl;
		return;
	}

	for (vector<basic_string<Char> >::const_iterator iter1 = v1.begin(),
		iter2 = v2.begin(); iter1 != v1.end();
		iter1++, iter2++)
	{
		if (*iter1 != *iter2)
		{
			cout << "test failed - v1 != v2" << endl;
			return;
		}
	}

	//cout << "test ok" << endl;
}

template<class String>
void zero(String &str)
{
	str = String(str.size(), ' ');
}

int main(int argc, char *argv[])
{
	my::stopwatch timer(my::stopwatch::show_total);

	int n = 1000;
	if (argc > 1)
		n = atoi(argv[1]);
	cout << "n=" << n << endl;
	cout << "Iterations = n*" << SIZE << " = " << n * SIZE << endl;

	srand(0);
	vector<string> strings(SIZE), strings_dest(SIZE);
	generate(strings.begin(), strings.end(), random_fill<char>());
	strings_dest = strings;

	
	vector<wstring> wstrings(SIZE), wstrings_dest(SIZE);
	generate(wstrings.begin(), wstrings.end(), random_fill<wchar_t>());
	wstrings_dest = wstrings;


	cout << "\n*** std::string ***\n";

	for_each( strings_dest.begin(), strings_dest.end(), zero<string>);
	timer.reset();
	timer.start();
	cout << "copy(iterator -> iterator): ";
	for (int ni = 0; ni < n; ni++)
	{
		for (int i = 0; i < SIZE; i++)
		{
			copy(strings[i].begin(), strings[i].end(),
				strings_dest[i].begin());
		}
	}
	timer.finish();
	cout << timer << endl;
	test(strings, strings_dest);


	for_each( strings_dest.begin(), strings_dest.end(), zero<string>);
	timer.reset();
	timer.start();
	cout << "copy(iterator -> ptr):      ";
	for (int ni = 0; ni < n; ni++)
	{
		for (int i = 0; i < SIZE; i++)
		{
			copy(strings[i].begin(), strings[i].end(),
				(char*)strings_dest[i].c_str());
		}
	}
	timer.finish();
	cout << timer << endl;
	test(strings, strings_dest);


	for_each( strings_dest.begin(), strings_dest.end(), zero<string>);
	timer.reset();
	timer.start();
	cout << "copy(ptr -> ptr):           ";
	for (int ni = 0; ni < n; ni++)
	{
		for (int i = 0; i < SIZE; i++)
		{
			copy((char*)strings[i].c_str(),
				(char*)strings[i].c_str() + strings[i].size(),
				(char*)strings_dest[i].c_str());
		}
	}
	timer.finish();
	cout << timer << endl;
	test(strings, strings_dest);

	
	for_each( strings_dest.begin(), strings_dest.end(), zero<string>);
	timer.reset();
	timer.start();
	cout << "memcpy(ptr -> ptr):         ";
	for (int ni = 0; ni < n; ni++)
	{
		for (int i = 0; i < SIZE; i++)
		{
			memcpy((char*)strings_dest[i].c_str(),
				(char*)strings[i].c_str(), strings[i].size() + 1);
		}
	}
	timer.finish();
	cout << timer << endl;
	test(strings, strings_dest);


	#ifdef _OPENMP
	for_each( strings_dest.begin(), strings_dest.end(), zero<string>);
	timer.reset();
	timer.start();
	cout << "|| memcpy(ptr -> ptr):      ";
	for (int ni = 0; ni < n; ni++)
	{
		#pragma omp parallel for
		for (int i = 0; i < SIZE; i++)
		{
			memcpy(&*strings_dest[i].begin(),
				&*strings[i].begin(), strings[i].size() + 1);
		}
	}
	timer.finish();
	cout << timer << endl;
	test(strings, strings_dest);
	#endif

	
	cout << "\n*** std::wstring ***\n";

	for_each( wstrings_dest.begin(), wstrings_dest.end(), zero<wstring>);
	timer.reset();
	timer.start();
	cout << "copy(iterator -> iterator): ";
	for (int i = 0; i < n; i++)
	{
		for (vector<wstring>::iterator iter = wstrings.begin(),
			iter_dest = wstrings_dest.begin(); iter != wstrings.end();
			iter++, iter_dest++)
		{
			copy(iter->begin(), iter->end(), iter_dest->begin());
		}
	}
	timer.finish();
	cout << timer << endl;
	test(wstrings, wstrings_dest);


	for_each( wstrings_dest.begin(), wstrings_dest.end(), zero<wstring>);
	timer.reset();
	timer.start();
	cout << "copy(iterator -> ptr):      ";
	for (int i = 0; i < n; i++)
	{
		for (vector<wstring>::iterator iter = wstrings.begin(),
			iter_dest = wstrings_dest.begin(); iter != wstrings.end();
			iter++, iter_dest++)
		{
			copy(iter->begin(), iter->end(), (wchar_t*)iter_dest->c_str());
		}
	}
	timer.finish();
	cout << timer << endl;
	test(wstrings, wstrings_dest);


	for_each( wstrings_dest.begin(), wstrings_dest.end(), zero<wstring>);
	timer.reset();
	timer.start();
	cout << "copy(ptr -> ptr):           ";
	for (int i = 0; i < n; i++)
	{
		for (vector<wstring>::iterator iter = wstrings.begin(),
			iter_dest = wstrings_dest.begin(); iter != wstrings.end();
			iter++, iter_dest++)
		{
			copy((wchar_t*)iter->c_str(), (wchar_t*)iter->c_str() + iter->size(),
				(wchar_t*)iter_dest->c_str());
		}
	}
	timer.finish();
	cout << timer << endl;
	test(wstrings, wstrings_dest);

	
	for_each( wstrings_dest.begin(), wstrings_dest.end(), zero<wstring>);
	timer.reset();
	timer.start();
	cout << "memcpy(ptr -> ptr):         ";
	for (int i = 0; i < n; i++)
	{
		for (vector<wstring>::iterator iter = wstrings.begin(),
			iter_dest = wstrings_dest.begin(); iter != wstrings.end();
			iter++, iter_dest++)
		{
			memcpy((wchar_t*)iter_dest->c_str(), (wchar_t*)iter->c_str(),
				iter->size() * sizeof(wchar_t));
		}
	}
	timer.finish();
	cout << timer << endl;
	test(wstrings, wstrings_dest);

	return 0;
}
