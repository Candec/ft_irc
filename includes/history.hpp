#ifndef HISTORY_HPP
# define HISTORY_HPP

# include <map>
# include <string>
# include <iostream>

class History
{
	public:
		History();

		void set(unsigned int pos, std::string line);
		void remove(unsigned int pos);

	private:
		std::map<unsigned int, std::string> history;

		void update();
		void clear();
};

#endif