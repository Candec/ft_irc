#include "utility.hpp"

void error(std::string str, bool quit)
{
	std::cout << "Error: " << str << std::endl;
	if (quit)
		exit(EXIT_FAILURE);
}