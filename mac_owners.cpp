#include <iostream> /* cout, wcout */
#include <fstream> /* cout, wcout */
using namespace std;

#include <boost/spirit/include/qi.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <boost/unordered_map.hpp>
#include <boost/functional/hash.hpp>

/* Для тестов */
#include <libs/date_time/src/posix_time/posix_time_types.cpp>
#include <libs/date_time/src/gregorian/date_generators.cpp>
#include <boost/date_time/posix_time/posix_time.hpp>

typedef boost::unordered_map<unsigned int,string> mac_owners_map_t;

typedef boost::unordered_map<unsigned int,string,
	boost::hash<unsigned char> > mac_owners_map_t_2;

typedef std::map<unsigned int,string> mac_owners_map_t_3;

template<class T>
bool mac_owners_parse(const char *file, T *p_mac_owners)
{
	using namespace boost::spirit;

	fstream fs(file);
	if (!fs.is_open()) return false;

	/* Ждём две пустые строки, чтобы начать разбор */
	{
		string str;
		int n = 0;

		while ( getline(fs, str) ) {
			if (!str.empty()) n = 0;
			else if (++n == 2) break;
		}

		if (n != 2) return false;
	}

    /* Разбор всех */
	{
		string str;
		int n = 0; /* Нумерация строк */
		bool owner_parsed = false;
		unsigned int id;
		string name;
		string country;
		int i = 0;

		while ( getline(fs, str) ) {
			/* Когда встречаем пустую строку, только тогда
				и сохраняем очередного владельца */
			if (str.empty()) {
				if (p_mac_owners && owner_parsed) {
					
					if (!country.empty())
						name += string(" (") + country + string(")");

					/* Имеются повторяющиеся! Проверяем */
					T::iterator it = p_mac_owners->find(id);

					/* Нет такого */
					if (it == p_mac_owners->end())
						(*p_mac_owners)[id] = name;
					/* Уже есть такой. Объединяем */
					else {
						it->second += string(" / ") + name;
					}
				}

				n = 0;
				owner_parsed = false;
				name.clear(); /* Очищаем обязательно! */
			}
			else if (++n == 2) {
				/* Почти всё, что нам нужно, находится во второй строке */

				char ch;
				string::iterator it = str.begin();

				bool res = qi::parse(it, str.end(),
					qi::hex
						>> *qi::lit(' ')
						>> qi::lit("(base 16)")
						>> *qi::lit('\t')
						>> *qi::char_
					, id, name);

				if (!res || it != str.end()) return false;

				owner_parsed = true;
			}
			else {
				/* В последней строке берём страну. Интересно же всё таки */
				country = boost::trim_copy(str);
			}
		}
	}

	return true;
}

int main(int argc, char *argv[])
{
	using namespace boost::posix_time;

	wcout.imbue( locale("Russian_Russia.866") );
	wcout << L"Разбор владельцев mac-адресов\n\n";

	mac_owners_map_t mac_owners;
	mac_owners_map_t_2 mac_owners_2;
	mac_owners_map_t_3 mac_owners_3;

	ptime start = microsec_clock::local_time();
	wcout << L"Разбор без сохранения в map\n";
	bool res;
	for (int i = 0; i < 10; i++)
		if (!mac_owners_parse("mac_owners.txt", (mac_owners_map_t*)NULL))
			wcout << L"Ошибка разбора\n";
	cout << to_simple_string(microsec_clock::local_time() - start) << endl;

	start = microsec_clock::local_time();
	wcout << L"Разбор с сохранением в map\n";
	for (int i = 0; i < 10; i++) {
		mac_owners.clear();
		if (!mac_owners_parse("mac_owners.txt", &mac_owners))
			wcout << L"Ошибка разбора\n";
	}
	cout << to_simple_string(microsec_clock::local_time() - start) << endl;

	wcout << L"Количество: " << mac_owners.size() << endl;

	cout << endl;
	cout << "0001C8: " << mac_owners[0x0001C8] << endl;
	cout << "080030: " << mac_owners[0x080030] << endl;

	cout << endl;

	if (!mac_owners_parse("mac_owners.txt", &mac_owners_2))
		wcout << L"Ошибка разбора\n";
	if (!mac_owners_parse("mac_owners.txt", &mac_owners_3))
		wcout << L"Ошибка разбора\n";

	const int count = 10000000;
	wcout << L"Выборка случайных значений (" << count << L")\n";

	wcout << L"boost::unordered_map hash=default\n";
	start = microsec_clock::local_time();
	for (int i = 0; i < count; i++) {
		int id = rand() & 0xFFFFFF;
		mac_owners.find(id);
	}
	cout << to_simple_string(microsec_clock::local_time() - start) << endl;

	wcout << L"boost::unordered_map hash=unsigned char\n";
	start = microsec_clock::local_time();
	for (int i = 0; i < count; i++) {
		int id = rand() & 0xFFFFFF;
		mac_owners_2.find(id);
	}
	cout << to_simple_string(microsec_clock::local_time() - start) << endl;

	wcout << L"std::map\n";
	start = microsec_clock::local_time();
	for (int i = 0; i < count; i++) {
		int id = rand() & 0xFFFFFF;
		mac_owners_3.find(id);
	}
	cout << to_simple_string(microsec_clock::local_time() - start) << endl;

	return 0;
}
