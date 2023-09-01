#include "history.cpp"

History::History()
{
	update();
}

History::update()
{
	clear();

	for (std::map<unsigned char, std::string>::iterator i = history.begin(); i != history.end(); i++)
		std::cout << it->second << "\033[0m" << std::endl;
}

History::clear()
{
	std::cout << "\033[2J" << std::flush;
}

History::set(unsigned char i, std::string line)
{
	if (line[i] == line)
		return
	
	history[i] = line;
	update();
}

History::remove(unsigned char i)
{
	history.erase(i);
	update();
}