#include <string>
#include <iostream>
using namespace std;


int main(void)
{
	char ar[][6] = { "aaaaa", "bbbbb", "ccccc", "ddddd" };

	cout << ar[0] << endl;
	cout << ar[0] + 1 << endl;
	cout << ar[0] + 3 << endl;
	cout << "aaaaa" + 3 << endl;


	std::cout << sizeof("123456789") << std::endl; //Output: 10
	std::cout << sizeof("123456789" + 5) << std::endl; //Output: 10
  
	const char (&a)[10] = "123456789"; //ok
	const char (&b)[10] = "123456789" + 3; //ok
	//const char (&c)[10 - 3] = "123456789" + 3; //error C2440: 'initializing' : cannot convert from 'const char [10]' to 'const char (&)[7]'

	cout << a << endl;
	cout << b << endl;
  
	return 0;
}