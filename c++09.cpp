#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <array>
#include <unordered_map>
#include <functional>
#include <cmath>
using namespace std;

//#include <boost/math/tr1.hpp>
#include <boost/math/special_functions/acosh.hpp>

class A
{
public:
	int ptr;
	A() : ptr(0) { cout << "A(0)" << endl; }
	A(int p) : ptr(p) { cout << "A(" << ptr << ")" << endl; }
	A(const A &a) :ptr(0) { cout << "A() - fail" << endl; }
	A(A &&a) { ptr = a.ptr; a.ptr = 0; }
};

A f(int n) { return A(n); }
auto f2 = [] (int n) -> A { return A(n); }; 

int main()
{
	A a1(123);
	A a2(456);

	cout << "a1: " << a1.ptr << endl;
	cout << "a2: " << a2.ptr << endl;
	cout << "---" << endl;

	A a3(a1);

	cout << "a1: " << a1.ptr << endl;
	cout << "a3: " << a3.ptr << endl;
	cout << "---" << endl;

	A a4 = A(7);

	cout << "a4: " << a4.ptr << endl;
	cout << "---" << endl;

	A a5(move(a1));

	cout << "a1: " << a1.ptr << endl;
	cout << "a5: " << a5.ptr << endl;
	cout << "---" << endl;

    // rvalue (&&)
    // static_assert
    // auto
    // nullptr
    // std::array, std::unordered_map
    // std::unique_ptr, std::shared_ptr
    // cbegin, cend
    // next, prior
    // fstream(std::string)
    // std::bind
    // lambda

	static_assert(sizeof(int) <= sizeof(long), "not big enough");
	auto a = f(7);
	cout << "a: " << a.ptr << endl;
	decltype(f(6)) aa = f(8);
	cout << "aa: " << aa.ptr << endl;

	char *u = nullptr;
	int8_t r;

	array<int,8> t = {1,2,3,4,5,6,7};
	unordered_map<int,int> y;
	cout << *t.cbegin() << endl;
	cout << *next( t.cbegin(), 5) << endl;

	unique_ptr<A> pa;
	shared_ptr<A> pa2;

	wofstream fs1(L"a1.txt");
	wofstream fs2(wstring(L"a2.txt"));
	bind(&f,777)();

    /* Только в бусте */
	boost::math::acosh(1.6L);

	vector<int> v;
	for (int i = 0; i < 10; ++i) 
		v.push_back(i);

	int evenCount = 0;
	for_each(v.begin(), v.end(), [&evenCount] (int n) -> void
	{
		cout << n;

		if (n % 2 == 0) 
		{
			cout << " is even " << endl;
			evenCount++;
		}
		else 
			cout << " is odd " << endl;
	});

	cout << "There are " << evenCount 
		<< " even numbers in the vector." << endl;

	f2(888);

	return 0;
}
