#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

#include <boost/bind.hpp>
#include <boost/function.hpp>

#include <my_ptr.h>
#include <my_thread.h>
#include <my_employer.h>

class A
{
public:
	void close()
	{
		cout << "close()" << endl;
	}

};

class B
{
public:
	void operator()()
	{
		cout << "operator()" << endl;
	}

};

int main(void)
{
	my::employer employer;

	A a;

	my::worker::ptr ptr1 = employer.new_worker("aaa", boost::bind(&A::close, &a));
	my::worker::ptr ptr2 = employer.new_worker("bbb", B());

	employer.lets_finish();

	ptr1.reset();

	long count = employer.number_of_workers();
	cout << "workers: " << count << endl;
	cout << "not finished: " << employer.check_for_finish() << endl;
	for (long i = 0; i < count; i++)
	{
		cout << i << ": " << employer.worker_name( employer[i] )
			<< " finished=" << employer.worker_finished( employer[i] ) << endl;
	}

	ptr2.reset();

	employer.wait_for_finish();

	return 0;
}
