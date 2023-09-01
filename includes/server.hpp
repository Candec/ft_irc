#ifndef SERVER_HPP
# define SERVER_HPP

# include "client.hpp"
# include "channel.hpp"

# include <poll.h>
# include <string>
# include <vector>
# include <map>

# include <ctime>

class Server 
{
	public:
		Server();
		~Server();

		void init();
		void exec();

		// setOperator();
		// getOperator();
		// getOperators();
		// delOperator();

		// setUser();
		// getUser();
		std::vector<User *> getUsers();
		// delUser();

		// setChannel();
		// getChannel();
		// getChannels();
		// delChannel();

	private:
		std::map<int, User> operators;
		std::map<int, User> users;
		
		std::string upTime;
		time_t previous_ping
		//void sendPing();

		// void addUser();
		// void displayUsers();
		// void displayChannels();

}

#endif