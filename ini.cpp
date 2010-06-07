#include <iostream>
#include <string>
#include <set>
#include <fstream>
using namespace std;

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/property_tree/ini_parser.hpp>

int main(void)
{
	using boost::property_tree::ptree;

	ptree pt;

	read_ini("ini.ini", pt);

	for (ptree::iterator it = pt.begin(); it != pt.end(); it++)
	{
		cout << "[" << it->first << "]" << endl;

		for (ptree::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
		{
			cout << it2->first << "=" << it2->second.data() << endl;
		}
	}

	cout << endl;
	cout << pt.get<string>("first_section.A") << endl;
	cout << pt.get<int>("first_section.B") << endl;
	cout << pt.get<double>("first_section.C") << endl;

    /*

	//pt.put_child("[new_section]", "");
    /*-

	pt.put("second_section.int", "1");
	pt.put("second_section.int", "2");
	pt.add("second_section.int", "3");
	pt.put("second_section.double", "3.45");
    -*/

	write_ini("ini2.ini", pt);

	return 0;
}