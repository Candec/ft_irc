/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tpereira <tpereira@42Lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:24 by tpereira          #+#    #+#             */
/*   Updated: 2023/10/21 10:16:00 by tpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "main.hpp"

enum Switch { OFF, ON };

# define CONFIG_FILE "./Configuration/irc.config"

const string Commands[7] = {
	"PASS",
	"NICK",
	"USER",
	"COLOR",
	"JOIN",
	"QUIT",
	"CAP"
};

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

		bool isChannel(const string &channel) const;
		void delChannel(const Channel *channel);

		struct s_msg parseMessage(User *user, const char * const buffer);
		vector< vector<string> >splitBuffer(const char * const buffer);
		bool isCmd(const string &word);
		void lookForCmd(User *user, vector<string> words, struct s_msg &msg);
		bool hasSpace(const string &str) const;

		void receiveMsg(vector<pollfd>::const_iterator it);
		int receiveMsg2(const int user_fd);
		void printMsg(vector<pollfd>::const_iterator it);
		void printMsg2(const int user_fd, const char *msg);

		/*
		** LIST OF CMDS
		*/
		void passCmd(User *user, const vector<string> words);
		void nickCmd(User *user, const vector<string> &words);
		void userCmd(User *user, const vector<string> &words);
		void joinCmd(User *user, vector<string> words);
		void colorCmd(User *user, const vector<string> &words);
		void quitCmd(User *user);
		void capCmd(User *user, vector<string> &words);

	public:
		Server();
		Server(const char * const port, const string password);
		~Server();

		void parseConfig();
		void setup();
		void run();

		void setPort(const char * const arg);
		void setPassword(const string &password);

		void sendClear(const int user_fd);
		void sendMsg(const int user_fd, const string &msg);
		void sendMsg(const int user_fd, const int n);
		void sendColorMsg(const int user_fd, const string &msg, const string &color);
		void sendError(const int user_fd, const string &reason);

		void createChannel(const string &channelName);
		Channel *getChannel(const string &channelName);

		vector<Channel *> getChannels() const;


};

#endif
