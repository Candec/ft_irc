#ifndef SERVER_HPP
# define SERVER_HPP

# include "user.hpp"
# include "channel.hpp"
# include "history.hpp"
# include "utility.hpp"

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
# include <fstream>
# include <string>
# include <utility>

enum Switch { OFF, ON };

class Server 
{
	private:
		int fd;

		History history;
	
		std::map<int, Channel *> channels;
		std::map<int, User *> operators;
		std::map<int, User *> users;
		std::vector<pollfd> pollfds;
		
		time_t upTime;
		time_t previous_ping;
		void updatePing();

		// void addUser();
		// void displayUsers();
		// void displayChannels();

		// # Configs
		std::string port;
		std::string password;

		unsigned int ping;
		unsigned int timeout;

	public:
		Server();
		Server(char *_port, char *_password);
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
		void delUser(User &user);

		// void setChannel();
		// Channel *getChannel();
		// std::vector<Channel *> getChannels();
		void delChannel(Channel &channel);

		bool run;
};

#endif