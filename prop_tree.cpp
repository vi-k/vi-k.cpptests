#include <iostream>
#include <string>
#include <set>
#include <fstream>
using namespace std;

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/detail/xml_parser_writer_settings.hpp>
#include <libs/serialization/src/utf8_codecvt_facet.cpp>

#if 1
#define PTREE wptree
#define IFSTREAM wifstream
#define OFSTREAM wofstream
#define CHAR wchar_t
#define COUT wcout
#define _(text) L##text
#define STRING wstring
#else
#define PTREE ptree
#define IFSTREAM ifstream
#define OFSTREAM ofstream
#define CHAR char
#define COUT cout
#define _(text) text
#define STRING string
#endif

int main(void)
{
	using boost::property_tree::PTREE;
	PTREE pt;
	bool utf8_forced = true;
	bool utf8 = false;

	COUT.imbue( locale( COUT.getloc(),
			new boost::archive::detail::utf8_codecvt_facet));

	IFSTREAM fs( _("prop_tree.cfg") );
	COUT << _("exist: ") << ( fs.is_open() ? _("true") : _("false") ) << endl;	
	
	CHAR ch = fs.get();
	if (ch == 0xEF && fs.get() == 0xBB && fs.get() == 0xBF) {
		utf8 = true;
		fs.imbue( locale( fs.getloc(),
			new boost::archive::detail::utf8_codecvt_facet));
		COUT << _("utf8") << endl;
	}
	else {
		utf8 = false;
		fs.imbue( locale("") );
		COUT << _("Standart locale()") << endl;
		fs.seekg(0);
	}

	if (!utf8 && utf8_forced) {
		utf8 = true;
		fs.imbue( locale( fs.getloc(),
			new boost::archive::detail::utf8_codecvt_facet));
		COUT << _("utf8") << endl;
	}

	//read_json(fs, pt);
	read_xml(fs, pt);

	BOOST_FOREACH(PTREE::value_type &v, pt.get_child( _("test.<xmlattr>") ) ) {
		COUT << v.first << _("=") << v.second.data()
				<< _(" size=") << v.second.data().size() << endl;
	}

	{
		//wstring wstr = pt.get<wstring>( _("test.<xmlattr>.image"), _("-none-") );
		//wcout << wstr << endl;
		IFSTREAM fstest( pt.get<STRING>( _("test.<xmlattr>.image") ).c_str() );
		COUT << _("exist: ") << ( fstest.is_open() ? _("true") : _("false") ) << endl;
	}

	/*-
	COUT << endl;
	COUT << _("[ping_server]") << endl;
	std::pair<PTREE::assoc_iterator, PTREE::assoc_iterator> p =
		pt.equal_range( _("ping_server") );
	while (p.first != p.second) {
		COUT << _("{") << endl;
		BOOST_FOREACH(PTREE::value_type &v, (*p.first).second) {
			COUT << _("\t") << v.first << _("=") << v.second.data() << endl;
		}
		COUT << _("}") << endl;

		p.first++;
	}
    -*/

	for (int i = 0; i < 3; i++) {
		const CHAR *filename;
		
		switch (i) {
			case 0: filename = _("prop_tree.out.json"); break;
			case 1: filename = _("prop_tree.out.xml"); break;
			case 2: filename = _("prop_tree.out.info"); break;
		}
		
		OFSTREAM fs(filename);
		if (utf8) {
			fs.put(0x00EF);
			fs.put(0x00BB);
			fs.put(0x00BF);
			fs.imbue( locale( fs.getloc(),
					new boost::archive::detail::utf8_codecvt_facet));
		}
		else {
			fs.imbue( locale("") );
		}

		switch (i) {
			case 0: write_json(fs, pt); break;
			case 1: {
				boost::property_tree::xml_writer_settings<CHAR> xs( _('\t'), 1, _("utf-8") );
				write_xml(fs, pt, xs);
				break;
			}
			case 2: write_info(fs, pt); break;
		}
	}

	//write_info(fs2, pt);
	//read_xml("prop_tree.xml", pt);

#if 0
	STRING m_file = pt.get<STRING>( _("debug.filename") );
	COUT << m_file << endl;

	int m_level = pt.get( _("debug.level"), 0);
	COUT << m_level << endl;

	set<STRING> m_modules;

	BOOST_FOREACH(PTREE::value_type &v, pt.get_child( _("debug.modules") ) ) {
		m_modules.insert(v.second.data());
	}

	BOOST_FOREACH(STRING &str, m_modules) {
		COUT << str << endl;
	}
#endif
	return 0;
}