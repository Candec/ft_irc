/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:24 by tpereira          #+#    #+#             */
/*   Updated: 2023/09/23 16:35:12 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "main.hpp"

# define BUFFER 1024
# define MESSAGE_END "\r\n"

enum Switch { OFF, ON };

struct s_msg {
	User	*src;
	time_t	timestamp;
	char	buffer[BUFFER + 1];
	std::string	command;
	std::string	nick;
	std::string	user;
	std::string	password;
	bool	op;
	bool	away;
};

class Server
{
	private:
		int listen_fd;

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
		void printUsers();
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
		void run();

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

		struct s_msg parseMessage(User *user, const char* const buffer) const;
		const std::vector< std::vector<std::string> > splitBuffer(const char* const buffer) const;
		void parseLine(User *user, struct s_msg& msg, const std::vector<std::string>& words) const;

		void sendMsg(int client_fd, const std::string &msg);
};

#endif
