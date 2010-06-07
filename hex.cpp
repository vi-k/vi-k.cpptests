#include <iostream>
#include <ios>

int main(void)
{
	std::cout << 123 << std::endl;
	std::cout << std::hex << 123 << std::endl;
	std::cout << std::uppercase << 123 << std::endl;
	std::cout << std::dec << 123 << std::endl;
	std::cout << false << "-" << std::boolalpha << false << std::endl;
	std::cout << std::noboolalpha << true << "-" << std::boolalpha << true << std::endl;

	return 0;
}