#include "history.hpp"

History::History()
{
	update();
}

void History::update()
{
	for (map<unsigned int, string>::iterator i = _history.begin(); i != _history.end(); i++)
		cout << i->second << "\033[0m" << endl;
}

void History::clear()
{
	cout << "\033[2J" << flush;
}

void History::set(unsigned int i, string line)
{
	if (_history[i].compare(line) == 0)
		return;

	_history[i] = line;
	update();
}

void History::remove(unsigned int i)
{
	_history.erase(i);
	// update();
}

/* ************************************************************************** */
