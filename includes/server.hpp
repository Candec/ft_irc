/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:24 by tpereira          #+#    #+#             */
/*   Updated: 2023/09/16 18:41:18 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "main.hpp"

enum Switch { OFF, ON };

class Server
{
	private:
		int _fd;

		History _history;
	
		std::map<int, User> _operators;
		std::map<int, User> _users;
		std::vector<pollfd> _pollfds;
		
		time_t _upTime;
		// time_t _previous_ping;
		//void sendPing();

		// void addUser();
		// void displayUsers();
		// void displayChannels();

		// # Configs
		std::string _port;
		std::string _password;

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
		// void delChannel();

		bool run;
};

#endif
