/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:16:55 by jibanez-          #+#    #+#             */
/*   Updated: 2023/10/29 14:55:36 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.hpp"

Channel::Channel() : _modes("n") {}
Channel::Channel(const string &name) : _name(name), _modes("n")
{
	_type = _name[0];

	const string str = WLC_CH_MSG + name + RESET;

	setStatus(ChannelFlags::PUBLIC);

	set(YELLOW_BG + str);
	if (!_topic.empty())
		set(_topic);
}
Channel::~Channel() {} //? Missing any handling of players' current channel?

// Setters
void Channel::setName(const string &name) { _name = name; }
void Channel::setMode(const string &modes) { _modes = modes; }
void Channel::setTopic(const string &topic) { _topic = topic; }
void Channel::setKey(const string &key, const User *src)
{
	if (key.find(' ') != string::npos)
		return src->sendError(ERR_INVALIDKEY, "MODE", _name);
	_key = key;
}
void Channel::setType(ChannelFlags::Type type) { _type = (char)type; }
void Channel::setStatus(ChannelFlags::Status status) { _status = status; }

void Channel::setClientLimit(const uint limit) { _client_limit = limit; }
// void Channel::setUserModes(const User *user, const string modes) { _user_modes.at(user->getFd()) = modes; }

// Getters
const string	Channel::getName() const { return _name; }
const string	Channel::getMode() const { return _modes; }
const string	Channel::getTopic() const { return _topic; }
const string	Channel::getKey() const { return _key; }
char			Channel::getType() const { return _type; }
char			Channel::getStatus() const { return _status; }

uint			Channel::getClientLimit() const { return _client_limit; }
// const string	Channel::getUserModes(const User *user) const { return _user_modes.at(user->getFd()); }
vector<User *>	Channel::getUsers() const
{
	vector<User *> users = vector<User *>();

	for (map<int, User *>::const_iterator i = _users.begin(); i != _users.end(); ++i)
		users.push_back(i->second);
	return users;
}

bool Channel::isModeImplemented(ChannelFlags::Mode modeLetter) const
{
	switch (modeLetter)
	{
	case ChannelFlags::INVITE_ONLY:
	case ChannelFlags::PROTECTED_TOPIC:
	case ChannelFlags::KEY_CHANNEL:
	case ChannelFlags::OPERATOR:
	case ChannelFlags::CLIENT_LIMIT:
		return true;
	default:
		return false;
	}
}
void Channel::addMode(ChannelFlags::Mode letter, std::vector<std::string> &arguments, User *caller)
{
	switch (letter)
	{
	case ChannelFlags::OPERATOR:
		_operators.push_back(server->getUser(arguments[0]));
		arguments.erase(arguments.begin());
		break;
	case ChannelFlags::KEY_CHANNEL:
		setKey(arguments[0], caller);
		arguments.erase(arguments.begin());
		__attribute__ ((fallthrough));
	default:
		const char mode = letter;

		log(_name + ": Adding mode " + mode);

		if (_modes.find(mode) != string::npos)
			_modes += mode;
		else
			log("Channel already had that mode");
	}
}
void Channel::removeMode(ChannelFlags::Mode letter, std::vector<std::string> &arguments, User *caller)
{
	User *user;
	switch (letter)
	{
	case ChannelFlags::OPERATOR:
		user = server->getUser(arguments[0]);
		arguments.erase(arguments.begin());
		for (std::vector<User *>::iterator it = _operators.begin(); it != _operators.end(); ++it)
			if (*it == user)
				_operators.erase(it);
		break;
	case ChannelFlags::CLIENT_LIMIT:
		setKey("", caller);
		__attribute__ ((fallthrough));
	case ChannelFlags::INVITE_ONLY:
	case ChannelFlags::PROTECTED_TOPIC:
	case ChannelFlags::KEY_CHANNEL:
	default:
		const char mode = letter;

		log(_name + ": Removing mode " + mode);

		size_t pos = _modes.find(mode);
		if (pos != string::npos)
			_modes.erase(pos);
		else
			log("Channel didn't have that mode");
	}
}

void Channel::addUser(User *user)
{
	log(_name + ": Adding user " + toString(user->getFd()));

	_users[user->getFd()] = user;

	if (!_topic.empty())
		user->sendReply(RPL_TOPIC, "TOPIC", _name);
	else
		user->sendReply(RPL_NOTOPIC, "TOPIC", _name);
	// _operators[user] = false;
	// _user_modes[user->getFd()] = "";
}
void Channel::removeUser(User *user)
{
	log(_name + ": Removing user " + toString(user->getFd()));

	_users.erase(_users.find(user->getFd()));
}
void Channel::removeUser(const string &nick)
{
	for (map<int, User *>::iterator i = _users.begin(); i != _users.end(); ++i)
		if (i->second->getNick() == nick) {
			log(_name + ": Removing user " + toString(i->second->getFd()));

			return _users.erase(i);
		}
}

void Channel::ban(User *user)
{
	if (!isBanned(user))
		_banned.push_back(user);
}
void Channel::unban(const User *user)
{
	if (isBanned(user))
		for (vector<User *>::iterator it = _banned.begin(); it != _banned.end(); ++it)
			if (*it == user)
				_banned.erase(it);
}

bool Channel::isMember(User *user) const { return _users.find(user->getFd()) != _users.end(); }
bool Channel::isOperator(User *user) const
{
	for (vector<User *>::const_iterator it = _operators.begin(); it != _operators.end(); ++it)
		if (*it == user)
			return true;
	return false;
}
bool Channel::isFull() const { return (_users.size() == _client_limit); }
bool Channel::isBanned(const User *user) const
{
	for (vector<User *>::const_iterator it = _banned.begin(); it != _banned.end(); ++it)
		if (*it == user)
			return true;
	return false;
}
bool Channel::isInviteOnly() const { return (_modes.find('i') != string::npos); }
bool Channel::noExternalMessages() const { return (_modes.find('n') != string::npos); }
bool Channel::isTopicProtected() const { return(_modes.find('t') != string::npos); }


void Channel::addInvitedUser(User *user) { _invitations.push_back(user); }
bool Channel::isInvitedUser(User *user) const { return (find(_invitations.begin(), _invitations.end(), user) != _invitations.end()); }
void Channel::revokeInvitation(User *user)
{
	vector<User *>::iterator i = find(_invitations.begin(), _invitations.end(), user);
	if (i != _invitations.end())
		_invitations.erase(i);
}

void Channel::broadcast(const string &msg) const
{
	// for (map<int, User *>::const_iterator it = _users.begin(); it != _users.end(); ++it)
	// 	server->sendMsg(it->second, msg);
	broadcast(msg, NULL);
}
void Channel::broadcast(const string &msg, const User *exclude, const string &src) const
{
	broadcast(":" + src + " " + msg, exclude);
}
void Channel::broadcast(const string &msg, const User *exclude) const
{
	for (map<int, User *>::const_iterator it = _users.begin(); it != _users.end(); ++it)
		if (it->second != exclude)
			server->sendMsg(it->second, msg);
}

void Channel::update()
{
	// clear();
	for (map<int, User *>::const_iterator it = _users.begin(); it != _users.end(); it++)
	{
		User *user = it->second;
		if (isMember(user))
		{
			// server->sendClear(user->getFd());
			server->sendClear(user);
			for (map<int, string>::iterator i = _history.begin(); i != _history.end(); i++)
				server->sendMsg(user, i->second);
		}
	}
	// cout << i->second << RESET << endl << flush;
}

void Channel::set(const string &line)
{
	unsigned int last = _history.size();

	_history.insert(pair<unsigned int, string>(last, line));
	update();
}

void Channel::setLog(const string &line)
{
	unsigned int last = _history.size();

	_history.insert(pair<unsigned int, string>(last, "[ log ]: " + line));
	update();
}

void Channel::setMsg(const string &line, const string &nick)
{
	unsigned int last = _history.size();

	_history.insert(pair<unsigned int, string>(last, "[ " + nick + " ]: " + line));
	update();
}

void Channel::remove(const unsigned int i)
{
	_history.erase(i);
	update();
}
