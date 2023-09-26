#include "history.hpp"

History::History()
{
	update();
}

void History::update()
{
	clear();

	for (map<unsigned int, string>::iterator i = history.begin(); i != history.end(); i++)
		cout << i->second << "\033[0m" << endl;
}

void History::clear()
{
	cout << "\033[2J" << flush;
}

void History::set(unsigned int i, string line)
{
	if (history[i].compare(line) == 0)
		return;

	history[i] = line;
	update();
}

void History::remove(unsigned int i)
{
	history.erase(i);
	// update();
}

/* ************************************************************************** */
