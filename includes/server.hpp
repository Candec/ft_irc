#ifndef SERVER_HPP
# define SERVER_HPP

# include "user.hpp"
# include "channel.hpp"
# include "history.hpp"

# include <algorithm>
# include <iostream>
# include <ostream>
# include <sstream>
# include <string>
# include <vector>
# include <map>

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <fcntl.h>
# include <poll.h>

# include <cstdlib>
# include <cstdio>
# include <ctime>




enum Switch { OFF, ON };

class Server 
{
	public:
		Server();
		Server(std::string _port, std::string _password);
		~Server();

		void setup();
		void start();

		void setPort(char *_port);
		void setPassword(char *_password);

		// void setOperator();
		// User *getOperator();
		// std::vector<User *> getOperators();
		// void delOperator();

		// void setUser();
		// User *getUser();
		std::vector<User *> getUsers();
		// void delUser();

		// void setChannel();
		// Channel *getChannel();
		// std::vector<Channel *> getChannels();
		// void delChannel();

		bool run;

	private:
		int fd;
	
		std::map<int, User> operators;
		std::map<int, User> users;
		std::vector<pollfd> pollfds;
		
		std::string upTime;
		// time_t previous_ping
		//void sendPing();

		// void addUser();
		// void displayUsers();
		// void displayChannels();

		// # Configs
		std::string port;
		std::string password;

};

#endif