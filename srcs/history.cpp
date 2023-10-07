#include "history.hpp"

History::History() { update(); }

void History::update()
{
	//got the server point, need the channel pointer to get the users within the channel
	clear();
	for (map<unsigned int, User *>::iterator it = _user->begin(); i != _user->end(); it++)
	{
		for (map<unsigned int, string>::iterator i = _history.begin(); i != _history.end(); i++)
		{
			if (it->isOnChannel(it->getFd()))
			sendMsg(it->getFd(), i->second);
		}
	}
	// cout << i->second << RESET << endl << flush;
}

void History::clear()
{
	cout << CLEAR << flush;
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
