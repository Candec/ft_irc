#ifndef SERVER_HPP
# define SERVER_HPP

# include "user.hpp"
# include "channel.hpp"

# include <string>
# include <vector>
# include <map>

# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <poll.h>
# include <ctime>

class Server 
{
	public:
		Server();
		Server(std::string _port, std::string _password);
		~Server();

		void setup();
		void start();

		//setPort();
		//setPassword();

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
		uint16_t port;
		std::string password;
}

#endif