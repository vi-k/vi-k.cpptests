#include <windows.h>

#include <iostream>
using namespace std;

#define STDCALL
//#define STDCALL __stdcall
//#define BOOST_BIND_ENABLE_STDCALL
//#define BOOST_MEM_FN_ENABLE_STDCALL

#include <boost/bind.hpp>
#include <boost/function.hpp>

class test_t {
	private:
		long id_;
		bool flag_;
	public:
		boost::function3<void,int,int,int> on_mouse_wheel;
		void def_mouse_move(int,int,int);

		test_t(long id) : id_(id),flag_(false) {}
		long func1(int num, const char *str);
		long __stdcall func2(int num, const char *str);
		void func3(int num, const char *str);
		void STDCALL on_timer1(HWND,UINT,UINT_PTR,DWORD);
		void STDCALL on_timer2(PVOID,BOOLEAN);
		DWORD STDCALL thread_proc(LPVOID);
};

void test_t::def_mouse_move(int,int,int)
{
	cout << "def_mouse_move()" << endl;
}

long test_t::func1( int num, const char *str)
{
	cout << endl;
	cout << "func1()" << endl;
	cout << "id:  " << id_ << endl;
	cout << "num: " << num << endl;
	cout << "str: " << str << endl;

	return id_;
}

long __stdcall test_t::func2( int num, const char *str)
{
	cout << endl;
	cout << "func2()" << endl;
	cout << "id:  " << id_ << endl;
	cout << "num: " << num << endl;
	cout << "str: " << str << endl;

	return id_;
}

void test_t::func3( int num, const char *str)
{
	cout << endl;
	cout << "func3()" << endl;
	cout << "id:  " << id_ << endl;
	cout << "num: " << num << endl;
	cout << "str: " << str << endl;
}

typedef void (*func3_t)( int num, const char *str);
typedef boost::function<void (int,const char*)> b_func3_t;
typedef boost::function2<void,int,const char*> b_func3_t2;

void STDCALL test_t::on_timer1(HWND,UINT,UINT_PTR,DWORD)
{
	static int count = 0;
	cout << count++ << endl;
}

void STDCALL test_t::on_timer2(PVOID,BOOLEAN)
{
	static int count = 0;
	cout << count++ << endl;
}

void __stdcall on_timer2_test(PVOID,BOOLEAN)
{
	cout << "on_timer2_test" << endl;
}

typedef DWORD (STDCALL *THREAD_PROC)( LPVOID data);

DWORD STDCALL test_t::thread_proc( LPVOID data)
{
	cout << "thread_proc()" << endl;	
	cout << "id:  " << id_ << endl;
	cout << "data:  " << (int)data << endl;
	return 5;
}

DWORD STDCALL stat_thread_proc(LPVOID data)
{
	cout << "stat_thread_proc()" << endl;	
	cout << "data:  " << (int)data << endl;
	return 5;
}

DWORD STDCALL stat_thread_proc2(int a,LPVOID data)
{
	cout << "stat_thread_proc2() with a=" << a << endl;	
	cout << "data:  " << (int)data << endl;
	return 5;
}

typedef boost::function4<void,HWND,UINT,UINT_PTR,DWORD> on_timer1_t;
typedef boost::function2<void,PVOID,BOOLEAN> on_timer2_t;
typedef boost::function1<DWORD,LPVOID> thread_proc_t;

template <class S, class T>
class stdcall_mem_fun_t : public std::unary_function<T*, S>
{
public:
    explicit stdcall_mem_fun_t(S (STDCALL T::*p)(void*),T *t) : ptr(p), t_(t) {}
    //S __stdcall operator()(void *data) const { return (t_->*ptr)(data); }
    S operator()(void *data) const { return (S)data; } //(t_->*ptr)(data); }
private:
    S (STDCALL T::*ptr)(void*);
    T *t_;
};

template<class S, class T>
inline stdcall_mem_fun_t<S,T> stdcall_mem_fun(S (STDCALL T::*f)(void*),T *t)
{
    return stdcall_mem_fun_t<S,T>(f,t);
}

int sum(int a, int b)
{
	return a + b;
}

int ptr(int a, void *str)
{
	//cout << str << endl;
	return a;
}

void omm(int delta, int x, int y) {
	cout << "on_mouse_move" << endl;
}

int main(void)
{
	test_t x1(1), x2(2), x3(3);
	
	//cout << "return: " <<
	//boost::bind<long,long (test_t::*)(int,const char*)>( &test_t::func1, &x1, _1, _2)(11,"111");
	//(boost::bind<long>( &test_t::func1, &x1, _1, _2))(11,"111");
	//<< endl;
	//cout << "return: " << boost::bind<long (int, const char*)>( &test_t::func1, &x1, _1, _2)(11,"111") << endl;
	//cout << "return: " << boost::bind( &test_t::func2, &x1, _1, _2)(11,"111") << endl;

/*-
	boost::bind( &test_t::func3, &x3, _1, _2)(33,"333");

	b_func3_t f3;
	f3 = boost::bind( &test_t::func3, &x3, _1, _2);
	f3(33,"333-1");

	b_func3_t2 f3_2;
	f3_2 = boost::bind( &test_t::func3, &x3, _1, _2);
	f3_2(33,"333-2");

	//func3_t f3_3;
	//f3_3 = (func3_t)&f3_2; //boost::bind( &test_t::func3, &x3, _1, _2);
	//f3_3(33,"333-2");
-*/

//	on_timer1_t ft1 = boost::bind( &test_t::on_timer1, &x3, _1, _2, _3, _4);
//	SetTimer( NULL, 0, 500, (TIMERPROC)&ft1);

	//on_timer2_t ft2 = boost::bind<void __stdcall>( boost::mem_fn(&test_t::on_timer2), &x3, _1, _2);
//	on_timer2_t ft2 = boost::bind( boost::mem_fn(test_t::on_timer2), &x3, _1, _2);
//	HANDLE ht;
//	CreateTimerQueueTimer( &ht, NULL, (WAITORTIMERCALLBACK)&ft2,
//		NULL, 10, 0, WT_EXECUTEINTIMERTHREAD);
//	CreateTimerQueueTimer( &ht, NULL, (WAITORTIMERCALLBACK)on_timer2_test,
//		NULL, 10, 0, WT_EXECUTEINTIMERTHREAD);

	//stdcall_mem_fun( &test_t::thread_proc)(&x2);
	cout << stdcall_mem_fun( &test_t::thread_proc,&x2)((void*)1) << endl;
//	thread_proc_t ftt = boost::bind( &test_t::thread_proc, &x2, _1);
	cout << stdcall_mem_fun_t<DWORD,test_t>(&test_t::thread_proc,&x2)((void*)2) << endl;
//	LPTHREAD_START_ROUTINE tsr = (LPTHREAD_START_ROUTINE)&stdcall_mem_fun_t<DWORD,test_t>(&test_t::thread_proc,&x2);
//	tsr((void*)3);

//	THREAD_PROC tsr2 = &stdcall_mem_fun_t<DWORD,test_t>(&test_t::thread_proc,&x2);
//	tsr2((void*)4);

//	CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)&stdcall_mem_fun_t<DWORD,test_t>(&test_t::thread_proc,&x2), NULL, 0, NULL);


/*-
//Работает
	typedef void (WINAPI *GetNativeSystemInfoPtr)(LPSYSTEM_INFO);
	HMODULE h = LoadLibrary (("Kernel32.dll"));
	GetNativeSystemInfoPtr fp = (GetNativeSystemInfoPtr)GetProcAddress(h,"GetNativeSystemInfo");
	SYSTEM_INFO info;
	fp(&info); //works!
	cout << "Processors: " << info.dwNumberOfProcessors << endl;

	boost::function<void (LPSYSTEM_INFO)> fb = (GetNativeSystemInfoPtr)GetProcAddress(h,"GetNativeSystemInfo");
	SYSTEM_INFO info2;
	fb(&info2);
	cout << "Processors: " << info2.dwNumberOfProcessors << endl;

//Не работает
	//fp = (GetNativeSystemInfoPtr)fb;
	//fp(&info2);
-*/

//	boost::function<DWORD __stdcall (LPVOID)> f;

	cout << boost::bind(sum,1,2)() << endl;
	cout << boost::bind(sum,1,_1)(2) << endl;
	cout << boost::bind(sum,_1,_2)(1,2) << endl;

	void *data = NULL;

	boost::function<DWORD (LPVOID)> fg =
	//DWORD (STDCALL *fgg)(LPVOID) = 
	//DWORD (*fgg)() = 
	boost::bind<DWORD>(stat_thread_proc2, 123, _1);
	fg((void*)1);

	/*-
	struct F {
		int operator()(int a, int b) { return 1; }
		int operator()(long a, long b) { return 2; }
	};
	F f;
	int x = 104;
	long y = 105;
	cout << boost::bind<int>(f, _1, _1)(x) << endl;
	cout << boost::bind<int>(f, _1, _1)(y) << endl;
	-*/

	//DWORD (STDCALL *fgg)(LPVOID) = &fg;

	//boost::bind<DWORD (__stdcall*)(void*)>(&stat_thread_proc2,1)(NULL);
	//void fg = boost::bind(&test_t::thread_proc,&x1)

	//CreateThread( NULL, 0, stat_thread_proc,
//	CreateThread( NULL, 0, boost::bind(&test_t::thread_proc,&x1),
//		(void*)6, 0, NULL);

	//boost::function<void (int,const char*)> b_func3_t;
	boost::function<void (int delta, int x, int y)> on_mouse_wheel;

	on_mouse_wheel = omm;
	if (on_mouse_wheel) on_mouse_wheel(1, 2, 3);

	x1.on_mouse_wheel = omm;
	x1.on_mouse_wheel(1,2,3);
	
	x1.on_mouse_wheel = boost::bind(&test_t::def_mouse_move, &x1, _1, _2, _3);
	x1.on_mouse_wheel(1,2,3);

	SleepEx(2000,TRUE);

//	cout << "return: " << boost::bind( &test_t::func2, &x2, _1, _2)(22,"222") << endl;

	return 0;
}
