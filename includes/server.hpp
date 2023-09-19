/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:24 by tpereira          #+#    #+#             */
/*   Updated: 2023/09/19 12:20:29 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "main.hpp"
// # include "channel.hpp"
// # include "history.hpp"
// # include "utility.hpp"

// # include <algorithm>
// # include <iostream>
// # include <ostream>
// # include <sstream>
// # include <string>
// # include <vector>
// # include <map>

// # include <sys/socket.h>
// # include <netinet/in.h>
// # include <arpa/inet.h>
// # include <unistd.h>
// # include <fcntl.h>
// # include <poll.h>

// # include <cstdlib>
// # include <cstdio>
// # include <ctime>
// # include <fstream>
// # include <string>
// # include <utility>

# define BUFFER 2048
# define MESSAGE_END "\r\n"

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
		time_t previousPing;
		void updatePing();
		void updatePoll();

		void addUser();
		// void displayUsers();
		// void displayChannels();

		// # Configs
		std::string serverName;
		std::string password;
		unsigned int port;

		unsigned int ping;
		unsigned int timeout;
		unsigned int maxUsers;

	public:
		Server();
		Server(std::string _port, std::string _password);
		~Server();

		void setup();
		void start();

		void setPort(std::string _port);
		void setPassword(std::string _password);

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
