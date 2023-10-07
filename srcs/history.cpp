#include "history.hpp"

History::History()
{
	update();
}

void History::update()
{
	clear();
	for (map<unsigned int, string>::iterator i = _history.begin(); i != _history.end(); i++)
		cout << i->second << "\033[0m" << endl << flush;
}

void History::clear()
{
	cout << "\033[2J" << flush;
}

void History::set(unsigned int i, string line)
{
	if (_history[i].compare(line) == 0)
		return;

	// _history[i] = line;
	_history[i] = line;
	update();
}

void History::set(string line)
{
	unsigned int last = _history.size();

	_history.insert(std::pair<unsigned int, string>(last, line));
	update();
}

void History::remove(unsigned int i)
{
	_history.erase(i);
	// update();
}

/* ************************************************************************** */
