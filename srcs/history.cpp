#include "history.cpp"

Display()
{
	update();
}

Display::update()
{
	clear();

	for (std::map<unsigned char, std::string>::iterator i = history.begin(); i != history.end(); i++)
		std::cout << it->second << "\033[0m" << std::endl;
}

Display::clear()
{
	std::cout << "\033[2J" << std::flush;
}

Display::set(unsigned char i, std::string line)
{
	if (line[i] == line)
		return
	
	history[i] = line;
	update();
}

Display::remove(unsigned char i)
{
	history.erase(i);
	update();
}