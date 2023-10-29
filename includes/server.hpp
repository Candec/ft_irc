/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:24 by tpereira          #+#    #+#             */
/*   Updated: 2023/10/29 23:05:49 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "commands.hpp"
# include "main.hpp"

enum Switch { OFF, ON };

# define CONFIG_FILE "./Configuration/irc.config"

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

extern Server *server;

class Server
{
	private:
		int _listen_fd;

		// History history;

		map<string, Channel *> _channels;
		map<int, User *> _operators;
		map<int, User *> _users;
		vector<pollfd> _pollfds;

		const time_t _upTime;
		time_t _previousPing;
		void updatePing();
		void updatePoll();

		void createUser();
		// void printUsers();
		// void printChannels();


		// # Configs
		string _serverName;
		string _password;
		uint16_t _port;

		unsigned int _ping;
		unsigned int _timeout;
		unsigned int _maxUsers;

		// void setOperator();
		// User *getOperator();
		// vector<User *> getOperators();
		// void delOperator();

		// void setUser();
		// User *getUser();
		vector<User *> getUsers() const;
		void delUser(User *user);

		struct s_msg parseMessage(User *user, const char * const buffer);
		vector< vector<string> >splitBuffer(const char * const buffer);
		bool isCmd(const string &param);
		// void lookForCmd(User *user, vector<string> words, struct s_msg &msg);
		void lookForCmd(User *user, const int cmd, vector<string> params, struct s_msg &msg);

		void receiveMsg(vector<pollfd>::const_iterator it);
		// void printMsg(vector<pollfd>::const_iterator it);
		void printMsg2(User *user, const char *msg);

		/*
		** LIST OF CMDS
		*/
		void passCmd(User *user, const vector<string> &params);
		void nickCmd(User *user, const vector<string> &params);
		void userCmd(User *user, const vector<string> &params);
		void joinCmd(User *user, const vector<string> &params);
		void colorCmd(User *user, const vector<string> &params);
		void quitCmd(User *user, const string &reason);
		void capCmd(User *user, const vector<string> &params);
		void topicCmd(User *user, const vector<string> &params);
		void privmsgCmd(User *user, const vector<string> &params);
		void awayCmd(User *user, const vector<string> &params);
		void pingCmd(User *user, const vector<string> &token);
		void modeCmd(User *user, const vector<string> &params);
		void kickCmd(User *user, const vector<string> &params);
		void inviteCmd(User *user, const vector<string> &params);



	public:
		Server();
		Server(const char * const port, const string password);
		~Server();

		void parseConfig();
		void setup();
		void run();

		void setPort(const char * const arg);
		void setPassword(const string &password);

		void sendMsg(const User *user, const string &msg) const;
		void sendMsg(const User *user, const int n) const;
		void sendMsg(const int user_fd, const string &msg) const;
		void sendMsg(const int user_fd, const int n) const;
		void sendMsg(const User *user, const string &msg, const string &src) const;

		void broadcast(const string &message) const;

		void sendClear(const User *user) const;
		void sendClear(const int user_fd) const;

		void sendColorMsg(const int user_fd, const string &msg, const string &color) const;
		void sendColorMsg(const User *user, const string &msg, const string &color) const;

		void sendErrFatal(User *user, const string &reason);

		Channel *createChannel(const string &channelName);
		Channel *createChannel(const string &channelName, const User *creator);
		Channel *getChannel(const string &channelName) const;
		vector<Channel *> getChannels() const;

		User *getUser(const int user_fd) const;
		User *getUser(const string &nick) const;

		void delChannel(const Channel *channel);
};

#endif
