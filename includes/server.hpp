/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:24 by tpereira          #+#    #+#             */
/*   Updated: 2023/09/16 13:17:25 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

enum Switch { OFF, ON };

class Server 
{

	private:
		int fd;

		History history;
	
		std::map<int, User> operators;
		std::map<int, User> users;
		std::vector<pollfd> pollfds;
		
		time_t upTime;
		time_t previous_ping;
		//void sendPing();

		// void addUser();
		// void displayUsers();
		// void displayChannels();

		// # Configs
		std::string port;
		std::string password;

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
		// void delUser();

		// void setChannel();
		// Channel *getChannel();
		// std::vector<Channel *> getChannels();
		// void delChannel();

		bool run;
};

#endif