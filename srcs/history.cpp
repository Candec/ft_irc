#include "history.hpp"

History::History()
{
	update();
}

void History::update()
{
	clear();

	for (std::map<unsigned int, std::string>::iterator i = history.begin(); i != history.end(); i++)
		std::cout << i->second << "\033[0m" << std::endl;
}

void History::clear()
{
	// std::cout << "\033[2J" << std::flush;
}

void History::set(unsigned int i, std::string line)
{
	if (history[i].compare(line) == 0)
		return;
	
	history[i] = line;
	update();
}

void History::remove(unsigned int i)
{
	history.erase(i);
	update();
}