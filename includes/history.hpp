#ifndef HISTORY_HPP
# define HISTORY_HPP

# include <map>
# include <string>
# include <iostream>

class displayChannels
{
	public:
		Display();

		void set(unsigned char pos, std::string line);
		void remove(unsigned char pos);

	private:
		std::map<unsigned char, std::string> history;

		void update();
		void clear();
}

#endif