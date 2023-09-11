#include "utility.hpp"

void error(std::string str, bool exit)
{
	std::cout << "Error: " << str << std::endl;
	if (exit)
		exit(EXIT_FAILURE);
}