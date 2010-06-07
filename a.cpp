#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
	string str("string string string");

	char *new_str = new char[str.size()];
	copy(str.begin(), str.end(), new_str);
	cout << str << endl;
	cout << new_str << endl;
	delete[] new_str;

	wstring wstr(L"wstring wstring wstring");
	wchar_t *new_wstr = new wchar_t[wstr.size()];
	copy(wstr.begin(), wstr.end(), new_wstr);
	wcout << wstr << endl;
	wcout << new_wstr << endl;
	delete[] new_wstr;
		
	return 0;
}
