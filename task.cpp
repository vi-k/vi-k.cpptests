#include <string>
#include <iostream>
using namespace std;

#include <boost/task.hpp>

std::string echo(std::string const& msg) 
{
	return msg;
} 

void main() 
{ 
	boost::tasks::handle< std::string > h( 
		boost::tasks::async( 
			boost::tasks::make_task(echo, "Hello World!") ) ); 

	std::cout << h.get() << std::endl; 
} 
