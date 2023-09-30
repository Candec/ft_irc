/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:24 by tpereira          #+#    #+#             */
/*   Updated: 2023/09/30 15:27:03 by jibanez-         ###   ########.fr       */
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
	string	command;
	// string	nick;
	// string	user;
	// string	password;
	bool	op;
	bool	away;
};

class Server
{
	private:
		int listen_fd;

		History history;

		map<std::string, Channel *> channels;
		map<int, User *> operators;
		map<int, User *> users;
		vector<pollfd> pollfds;

		time_t upTime;
		time_t previousPing;
		void updatePing();
		void updatePoll();

		void addUser();
		void printUsers();
		// void displayChannels();


		// # Configs
		string serverName;
		string password;
		unsigned int port;

		unsigned int ping;
		unsigned int timeout;
		unsigned int maxUsers;

	public:
		Server();
		Server(string _port, string _password);
		~Server();

		void setup();
		void run();

		void setPort(string _port);
		void setPassword(string _password);

		// void setOperator();
		// User *getOperator();
		// vector<User *> getOperators();
		// void delOperator();

		// void setUser();
		// User *getUser();
		vector<User *> getUsers();
		void delUser(User &user);

		void setChannel(string channelName);
		// Channel *getChannel();
		vector<Channel *> getChannels();
		void delChannel(Channel &channel);

		struct s_msg parseMessage(User *user, const char* const buffer) const;
		const vector< vector<string> > splitBuffer(const char* const buffer) const;
		void parseLine(User *user, struct s_msg& msg, const vector<string>& words) const;

		void sendMsg(int user_fd, const string &msg);
		int receiveMsg(vector<pollfd>::iterator it);
		int receiveMsg2(int user_fd);
		void printMsg(vector<pollfd>::const_iterator it);
		void printMsg2(const int user_fd, const char *msg);
};

#endif
