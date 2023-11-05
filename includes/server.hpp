/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:24 by tpereira          #+#    #+#             */
/*   Updated: 2023/11/05 02:40:32 by fporto           ###   ########.fr       */
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

		std::map<std::string, Channel *> _channels;
		std::map<int, User *>	_operators;
		std::map<int, User *>	_users;
		std::vector<pollfd>		_pollfds;

		const time_t _upTime;
		time_t _previousPing;
		void updatePing();
		void updatePoll();

		void createUser();
		// void printUsers();
		// void printChannels();


		// # Configs
		std::string	_serverName;
		std::string	_password;
		uint16_t	_port;

		unsigned int _ping;
		unsigned int _timeout;
		unsigned int _maxUsers;

		// void setOperator();
		// User *getOperator();
		// vector<User *> getOperators();
		// void delOperator();

		// void setUser();
		// User *getUser();
		std::vector<User *> getUsers() const;
		void delUser(User *user);

		struct s_msg parseMessage(User *user, const char * const buffer);
		std::vector< std::vector<std::string> >splitBuffer(const char * const buffer);
		bool isCmd(const std::string &param) const;
		bool isChannel(const std::string &channel) const;
		// void lookForCmd(User *user, vector<std::string> words, struct s_msg &msg);
		void lookForCmd(User *user, const int cmd, std::vector<std::string> params, struct s_msg &msg);

		void receiveMsg(std::vector<pollfd>::const_iterator it);
		// void printMsg(vector<pollfd>::const_iterator it);
		void printMsg2(User *user, const char *msg);

		/*
		** LIST OF CMDS
		*/
		void passCmd(User *user, const std::vector<std::string> &params);
		void nickCmd(User *user, const std::vector<std::string> &params);
		void userCmd(User *user, const std::vector<std::string> &params);
		void joinCmd(User *user, const std::vector<std::string> &params);
		void colorCmd(User *user, const std::vector<std::string> &params);
		void quitCmd(User *user, const std::string &reason);
		void capCmd(User *user, const std::vector<std::string> &params);
		void topicCmd(User *user, const std::vector<std::string> &params);
		void privmsgCmd(User *user, const std::vector<std::string> &params);
		void awayCmd(User *user, const std::vector<std::string> &params);
		void pingCmd(User *user, const std::vector<std::string> &token);
		void modeCmd(User *user, const std::vector<std::string> &params);
		void kickCmd(User *user, const std::vector<std::string> &params);
		void inviteCmd(User *user, const std::vector<std::string> &params);
		void partCmd(User *user, std::vector<std::string> params);
		void whoCmd(const User *user, const std::vector<std::string> &params);
		void whoisCmd(const User *user, const std::vector<std::string> &params);



	public:
		Server();
		Server(const char * const port);
		Server(const char * const port, const std::string password);
		~Server();

		void parseConfig();
		void setup();
		void run();

		void setPort(const char * const arg);
		void setPassword(const std::string &password);

		void sendMsg(const User *user, const int n) const;
		void sendMsg(const User *user, const std::string &msg, const std::string &src) const;
		void sendMsg(const int user_fd, const std::string &msg) const;
		void sendMsg(const int user_fd, const int n) const;
		void sendMsg(const User *user, const std::string &msg) const;

		void broadcast(const std::string &message) const;

		void sendClear(const User *user) const;
		void sendClear(const int user_fd) const;

		void sendColorMsg(const int user_fd, const std::string &msg, const std::string &color) const;
		void sendColorMsg(const User *user, const std::string &msg, const std::string &color) const;

		void sendErrFatal(User *user, const std::string &reason);

		Channel *createChannel(const std::string &channelName);
		Channel *createChannel(const std::string &channelName, const User *creator);
		Channel *getChannel(const std::string &channelName) const;
		std::vector<Channel *> getChannels() const;

		User *getUser(const int user_fd) const;
		User *getUser(const std::string &nick) const;

		std::string getPassword();

		void delChannel(const Channel *channel);

		bool shareChannels(const User *user1, const User *user2) const;

		bool isOperator(const User *user) const;
};

#endif
