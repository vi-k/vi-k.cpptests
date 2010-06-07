#include <climits>
#include <cstdlib>

#include <iostream> 
#include <sstream>
#include <boost/format.hpp>

#include <libs/date_time/src/gregorian/date_generators.cpp>
#include <libs/date_time/src/posix_time/posix_time_types.cpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time;


//  This value specifies, how to unroll the integer string generation loop in 
//  Karma.
//      Set this to some integer in between 0 (no unrolling) and max expected 
//      integer string len (complete unrolling). If not specified, this value
//      defaults to 6.
#define BOOST_KARMA_NUMERICS_LOOP_UNROLL 6

#include <boost/spirit/include/karma.hpp>

using namespace std;
using namespace boost::spirit;

#define MAX_ITERATION 1000000


template <typename OutputIterator>
struct escaped_string
  : karma::grammar<OutputIterator, std::string(char const*)>
{
    escaped_string()
      : escaped_string::base_type(esc_str)
    {
        esc_char.add('\a', "\\a")('\b', "\\b")('\f', "\\f")('\n', "\\n")
                    ('\r', "\\r")('\t', "\\t")('\v', "\\v")
                    ('\'', "\\\'")('"', "\\\"")
            ;
        esc_str =   karma::lit(karma::_r1)
                << *(esc_char | karma::print | "\\x" << karma::hex)
                <<  karma::lit(karma::_r1)
            ;
    }
    karma::rule<OutputIterator, std::string(char const*)> esc_str;
    karma::symbols<char, char const*> esc_char;
};

///////////////////////////////////////////////////////////////////////////////
struct random_fill
{
    int operator()() const
    {
        int scale = std::rand() / 100 + 1;
        return (std::rand() * std::rand()) / scale;
    }
};

///////////////////////////////////////////////////////////////////////////////
int main()
{
    namespace karma = boost::spirit::karma;

    cout << "Converting " << MAX_ITERATION 
         << " randomly generated int values to strings." << flush << endl;

    std::srand(0);
    std::vector<int> v (MAX_ITERATION);
    std::generate(v.begin(), v.end(), random_fill()); // randomly fill the vector

    // test the C libraries ltoa function (the most low level function for
    // string conversion available)
    {
        //[karma_int_performance_ltoa
        char buffer[65]; // we don't expect more than 64 bytes to be generated here
        //<-
        std::string str;
		ptime start = microsec_clock::local_time();
        //->
        for (int i = 0; i < MAX_ITERATION; ++i)
        {
            _itoa_s(v[i], buffer, 10);
            //<-
            str = buffer;      // compensate for string ops in other benchmarks
            //->
        }
        //]

		time_duration time = microsec_clock::local_time() - start;
        cout << "ltoa:\t\t" << to_simple_string(time) << endl;
    }

    // test the iostreams library
    {
        //[karma_int_performance_iostreams
        std::stringstream str;
        //<-
		ptime start = microsec_clock::local_time();
        //->
        for (int i = 0; i < MAX_ITERATION; ++i)
        {
            str.str("");
            str << v[i];
        }
        //]

		time_duration time = microsec_clock::local_time() - start;
        cout << "iostreams:\t" << to_simple_string(time) << endl;
    }

    // test the Boost.Format library
    {
        //[karma_int_performance_format
        std::string str;
        boost::format int_format("%d");
        //<-
		ptime start = microsec_clock::local_time();
        //->
        for (int i = 0; i < MAX_ITERATION; ++i)
        {
            str = boost::str(int_format % v[i]);
        }
        //]

		time_duration time = microsec_clock::local_time() - start;
        cout << "Boost.Format:\t" << to_simple_string(time) << endl;
    }

    // test the Karma int_ generation routines 
    {
        std::string str;
		ptime start = microsec_clock::local_time();

        //[karma_int_performance_plain
        char buffer[65]; // we don't expect more than 64 bytes to be generated here
        for (int i = 0; i < MAX_ITERATION; ++i)
        {
            char *ptr = buffer;
            karma::generate(ptr, int_, v[i]);
            *ptr = '\0';
            //<-
            str = buffer;      // compensate for string ops in other benchmarks
            //->
        }
        //]

		time_duration time = microsec_clock::local_time() - start;
        cout << "int_:\t\t" << to_simple_string(time) << endl;
    }

    // test the Karma int_ generation routines 
    {
	    typedef std::back_insert_iterator<std::string> sink_type;

        std::string str;
        sink_type sink(str);
		ptime start = microsec_clock::local_time();

        //[karma_int_performance_plain
        for (int i = 0; i < MAX_ITERATION; ++i)
        {
            karma::generate(sink, int_, v[i]);
        }
        //]

		time_duration time = microsec_clock::local_time() - start;
        cout << "int_:\t\t" << to_simple_string(time) << endl;
    }

    {
	    typedef std::back_insert_iterator<std::string> sink_type;
 
		std::string generated;
		sink_type sink(generated);
 
		std::string str("string to escape: \n\r\t\"'\x19");
		char const* quote = "\'\'\'";
 
		escaped_string<sink_type> g;
		karma::generate(sink, g(quote), str);
		cout << generated << endl;
		// this will emit: ’’’string to escape: \n\r\t\"\'\x19’’’</a>
    }

    return 0;
}
