/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:17:30 by tpereira          #+#    #+#             */
/*   Updated: 2023/11/08 03:21:48 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "main.hpp"
using std::map;
using std::string;
using std::vector;

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
	enum ModeLetter {
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
		std::string	_name;
		std::string	_modes;
		// std::string	_topic;
		struct _s_topic {
			std::string	topic;
			const User	*setBy;
			time_t		setAt;
		}			_topic;
		std::string	_key; // Password
		char		_type;
		char		_status;

		size_t		_client_limit;
		std::map<const uint, User *>	_users;
		// std::map<User *, bool>	_operators;
		std::map<const uint, const User *>	_operators;
		// std::map<int, std::string>	_user_modes;

		std::vector<const User *>	_invitations;
		std::map<const uint, const User *>	_banned;

		std::map<int, std::string>	_history;

	public:
		Channel();
		Channel(const std::string &name);
		~Channel();

		// Setters
		void setName(const std::string &name);
		void setMode(const std::string &mode);
		void setTopic(const std::string &topic, const User *setBy);
		void setKey(const std::string &key, const User *src);
		void setType(ChannelFlags::Type type);
		void setStatus(ChannelFlags::Status status);

		void setClientLimit(const size_t limit);
		// void setUserModes(const User *user, const std::string mode);
		// void setHistory(const std::string &line);

		// Getters
		const std::string	getName() const;
		const std::string	getModes() const;
		const std::string	getTopic() const;
		const std::string	getTopicSetBy() const;
		const std::string	getTopicSetAt() const;
		const std::string	getKey() const;
		char				getType() const;
		char				getStatus() const;

		size_t				getClientLimit() const;
		// const std::string	getUserModes(const User *user) const;
		const std::vector<const User *>	getUsers() const;
		const std::vector<const User *>	getOperators() const;
		const std::vector<const User *>	getInvitations() const;

		bool isModeImplemented(ChannelFlags::ModeLetter mode) const;
		void addMode(ChannelFlags::ModeLetter letter, std::vector<std::string> arguments, const User *caller);
		// void addMode(ChannelFlags::ModeLetter letter);
		void removeMode(ChannelFlags::ModeLetter letter, std::vector<std::string> &arguments, User *caller);
		// void removeMode(ChannelFlags::ModeLetter letter);

		void addUser(User *user);
		void removeUser(const User *user);
		void removeUser(const std::string &nick);

		void ban(User *user);
		void unban(const User *user);

		bool isMember(const User *user) const;
		bool isOperator(const User *user) const;
		bool isFull() const;
		bool isBanned(const User *user) const;
		bool isInviteOnly() const;
		bool noExternalMessages() const;
		bool isTopicProtected() const;

		void addInvitedUser(const User *user);
		bool isInvitedUser(const User *user) const;
		void revokeInvitation(const User *user);

		void broadcast(const std::string &msg) const;
		void broadcast(const std::string &msg, const User *exclude, const std::string &src) const;
		void broadcast(const std::string &msg, const User *exclude) const;

		void update();
		void set(const std::string &line);
		void setLog(const std::string &line);
		void setMsg(const std::string &line, const std::string &nick);

		void remove(const unsigned int i);

		// void join(const std::string &channelName, const std::string &key);
};

#endif
