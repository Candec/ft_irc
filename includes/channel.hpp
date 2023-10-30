/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:30 by tpereira          #+#    #+#             */
/*   Updated: 2023/10/30 22:07:43 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "main.hpp"

class Server;
class User;

# define CHANNEL_NAME_MAX_LEN 200

// Maybe later change enums to arrays for pattern matching
namespace ChannelFlags {

enum Type {
	REGULAR = '#',
	LOCAL = '&'
};

enum Status {
	PUBLIC = '=',
	SECRET = '@', // Channel mode +s
	PRIVATE = '*' // Channel mode +p (deprecated)
};

// When making changes, certify that they're reflected in isModeImplemented()
enum Mode {
	INVITE_ONLY = 'i',
	PROTECTED_TOPIC = 't',
	KEY_CHANNEL = 'k',
	OPERATOR = 'o',
	CLIENT_LIMIT = 'l'
};

}

namespace Operator {

enum Command {
	KICK,
	INVITE,
	TOPIC,
	MODE
};

}

class Channel
{
	private:
		string	_name;
		string	_modes;
		string	_topic;
		string	_key; // Password
		char	_type;
		char	_status;

		uint	_client_limit;
		map<int, User *>	_users;
		// map<User *, bool>	_operators;
		map<int, User *>	_operators;
		// map<int, string>	_user_modes;

		vector<User *>		_invitations;
		map<int, User *>	_banned;

		map<int, string>	_history;

	public:
		Channel();
		Channel(const string &name);
		~Channel();

		// Setters
		void setName(const string &name);
		void setMode(const string &mode);
		void setTopic(const string &topic);
		void setKey(const string &key, const User *src);
		void setType(ChannelFlags::Type type);
		void setStatus(ChannelFlags::Status status);

		void setClientLimit(const uint limit);
		// void setUserModes(const User *user, const string mode);
		void setHistory(const string &line);

		// Getters
		const string	getName() const;
		const string	getMode() const;
		const string	getTopic() const;
		const string	getKey() const;
		char			getType() const;
		char			getStatus() const;

		uint			getClientLimit() const;
		// const string	getUserModes(const User *user) const;
		vector<User *>	getUsers() const;

		bool isModeImplemented(ChannelFlags::Mode mode) const;
		void addMode(ChannelFlags::Mode letter, std::vector<std::string> arguments, const User *caller);
		// void addMode(ChannelFlags::Mode letter);
		void removeMode(ChannelFlags::Mode letter, std::vector<std::string> &arguments, User *caller);
		// void removeMode(ChannelFlags::Mode letter);

		void addUser(User *user);
		void removeUser(User *user);
		void removeUser(const string &nick);

		void ban(User *user);
		void unban(const User *user);

		bool isMember(User *user) const;
		bool isOperator(User *user) const;
		bool isFull() const;
		bool isBanned(const User *user) const;
		bool isInviteOnly() const;
		bool noExternalMessages() const;
		bool isTopicProtected() const;

		void addInvitedUser(User *user);
		bool isInvitedUser(User *user) const;
		void revokeInvitation(User *user);

		void broadcast(const string &msg) const;
		void broadcast(const string &msg, const User *exclude, const string &src) const;
		void broadcast(const string &msg, const User *exclude) const;

		void update();
		void set(const string &line);
		void setLog(const string &line);
		void setMsg(const string &line, const string &nick);

		void remove(const unsigned int i);

		// void join(const string &channelName, const string &key);
};

#endif
