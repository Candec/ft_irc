#ifndef USER_HPP
# define USER_HPP

# include "utility.hpp"

# include <ctime>
# include <string>
# include <vector>

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <unistd.h>
# include <fcntl.h>
# include <poll.h>

enum { VERIFY, ACCEPT, ONLINE, OFFLINE };

class User
{
	public:
		User(int fd, struct sockaddr_in addr);
		~User();

		void sendPrivateMessage(User &To, std::string Message);
		void write(std::string Message);

 		// Setters
		void setStatus(int status);
		void setPreviousPing(time_t ping);
		void setNick(std::string nick);
		void setUser(std::string user);
		void setName(std::string name);
		void setRole(std::string role);
		void setPreviousNick(std::string previousNick);
		void setPreviousChannel(std::string previousChannel);

		// Getters
		int getFd();
		int getStatus();
		time_t getPreviousPing();
		std::string getHostaddr();
		std::string getHostname();
		std::string getNick();
		std::string getUser();
		std::string getName();
		std::string getRole();
		std::string getPreviousNick();
		std::string getPreviousChannel();

	private:
		int fd;
		int status;
		time_t previousPing;
		std::string hostaddr;
		std::string hostname;
		std::string nick;
		std::string user;
		std::string name;
		std::string role;
		std::string previousNick;
		std::string previousChannel;

		std::vector<std::string> waitToSend;
};

#endif