/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:30 by tpereira          #+#    #+#             */
/*   Updated: 2023/10/12 15:43:14 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "main.hpp"

class Server;
class User;

# define CHANNEL_NAME_MAX_LEN 200

// Maybe later change enums to arrays for pattern matching

enum ChannelTypes {
	REGULAR = '#',
	LOCAL = '&'
};

enum ChannelStatus {
	PUBLIC = '=',
	SECRET = '@', // Channel mode +s
	PRIVATE = '*' // Channel mode +p (deprecated)
};

enum ChannelModes {
	INVITE_ONLY = 'i',
	PROTECTED_TOPIC = 't',
	KEY_CHANNEL = 'k',
	OPERATOR = 'o',
	CLIENT_LIMIT = 'l'
};

enum Op_Cmds {
	KICK,
	INVITE,
	TOPIC,
	MODE
};

class Channel
{
	private:
		string	_name;
		string	_modes;
		string	_topic;
		string	_key; // Password
		char	_type;
		char	_status;

		uint	_users_max;
		map<int, User *>	_users;
		map<User *, bool>	_operators;
		// map<int, string>	_user_modes;

		vector<User *>		_invitations;

		map<int, string>	_history;
		Server	*_server;

	public:
		Channel();
		Channel(const string name, Server *server);
		~Channel();

		// Setters
		void setName(const string name);
		void setMode(const string mode);
		void setTopic(const string topic);
		void setKey(const string key);
		void setType(const char type);
		void setStatus(const char status);

		void setMaxUsers(const uint users_max);
		// void setUserModes(const User *user, const string mode);
		void setHistory(const string line);

		// Getters
		const string	getName() const;
		const string	getMode() const;
		const string	getTopic() const;
		const string	getKey() const;
		char			getType() const;
		char			getStatus() const;

		uint			getMaxUsers() const;
		// const string	getUserModes(const User *user) const;
		vector<User *>	getUsers() const;

		void addUser(User *user);
		void removeUser(User *user);
		void removeUser(const string &nick);

		bool isUser(User *user);
		bool isOnChannel(int const &fd);
		bool isOperator(User *user);

		void addInvitedUser(User *user);
		bool isInvitedUser(User *user) const;
		void revokeInvitation(User *user);

		void broadcast(User *user, const string &message);

		void update();
		void set(const string &line);
		void setLog(const string &line);
		void setMsg(const string &line, const string &nick);

		void remove(const unsigned int i);
};

#endif
