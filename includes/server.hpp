/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:24 by tpereira          #+#    #+#             */
/*   Updated: 2023/10/09 14:49:14 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "main.hpp"

enum Switch { OFF, ON };

struct s_msg {
	User	*src;
	time_t	timestamp;
	char	buffer[BUFFER + 1];
	bool	command;
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

		// History history;

		map<std::string, Channel *> channels;
		map<int, User *> operators;
		map<int, User *> users;
		vector<pollfd> pollfds;

		time_t upTime;
		time_t previousPing;
		void updatePing();
		void updatePoll();

		void addUser();
		// void printUsers();
		// void printChannels();


		// # Configs
		string serverName;
		string password;
		unsigned int port;

		unsigned int ping;
		unsigned int timeout;
		unsigned int maxUsers;

		// void setOperator();
		// User *getOperator();
		// vector<User *> getOperators();
		// void delOperator();

		// void setUser();
		// User *getUser();
		vector<User *> getUsers();
		void delUser(User &user);

		bool isChannel(string const &channel);
		void delChannel(Channel &channel);

		struct s_msg parseMessage(User *user, char* buffer);
		vector< vector<string> > splitBuffer(char* buffer);
		void lookForCmd(User *user, struct s_msg& msg, vector<string> words);

		void receiveMsg(vector<pollfd>::iterator it);
		int receiveMsg2(int user_fd);
		void printMsg(vector<pollfd>::const_iterator it);
		void printMsg2(const int user_fd, const char *msg);

		/*
		** LIST OF CMDS
		*/

		void passCmd(User *user, vector<string> words, struct s_msg& msg);
		void nickCmd(User *user, vector<string> words, struct s_msg& msg);
		void userCmd(User *user, vector<string> words, struct s_msg& msg);
		void joinCmd(User *user, vector<string> words, struct s_msg& msg);
		void colorCmd(User *user, vector<string> words, struct s_msg& msg);
		void quitCmd(User *user, vector<string> words, struct s_msg& msg);







	public:
		Server();
		Server(string _port, string _password);
		~Server();

		void setup();
		void run();

		void setPort(string _port);
		void setPassword(string _password);

		void sendClear(int user_fd);
		void sendMsg(int user_fd, const string &msg);
		void sendColorMsg(int user_fd, const string &msg, const string &color);

		void setChannel(string channelName);
		Channel *getChannel(const std::string &channelName);

		vector<Channel *> getChannels();


};

#endif
