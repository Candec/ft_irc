/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jibanez- <jibanez-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:16:55 by jibanez-          #+#    #+#             */
/*   Updated: 2023/11/01 19:14:27 by jibanez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.hpp"

Channel::Channel() : _modes("n") {}
Channel::Channel(const string &name) : _name(name), _modes("n"), _type(name[0])
{
	// const string str = WLC_CH_MSG + name + RESET;

	setStatus(ChannelFlags::PUBLIC);

	// set(YELLOW_BG + str);
	if (!_topic.topic.empty())
		set(_topic.topic);
}
Channel::~Channel() {} //? Missing any handling of players' current channel?

// Setters
void Channel::setName(const string &name) { _name = name; }
void Channel::setMode(const string &modes) { _modes = modes; }
void Channel::setTopic(const string &topic, const User *setBy)
{
	_topic.topic = topic;
	_topic.setBy = setBy;
	_topic.setAt = time(NULL);
}
void Channel::setKey(const string &key, const User *src)
{
	if (key.find(' ') != string::npos)
		return src->sendError(ERR_INVALIDKEY, _name);
	_key = key;
}
void Channel::setType(ChannelFlags::Type type) { _type = (char)type; }
void Channel::setStatus(ChannelFlags::Status status) { _status = status; }

void Channel::setClientLimit(const uint limit) { _client_limit = limit; }
// void Channel::setUserModes(const User *user, const string modes) { _user_modes.at(user->getFd()) = modes; }

// Getters
const string	Channel::getName() const { return _name; }
const string	Channel::getMode() const { return _modes; }
const string	Channel::getTopic() const { return _topic.topic; }
const string	Channel::getTopicSetBy() const { return _topic.setBy->getNick(); }
const string	Channel::getTopicSetAt() const { return toString(_topic.setAt); }
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
void Channel::addMode(ChannelFlags::Mode letter, std::vector<std::string> arguments, const User *caller)
{
	User *user;

	switch (letter)
	{
	case ChannelFlags::OPERATOR:
		user = server->getUser(arguments[0]);
		if (!user)
			return;

		_operators.insert(pair<int, User *>(user->getFd(), user));

		if (arguments.size())
			arguments.erase(arguments.begin());
		break;
	case ChannelFlags::CLIENT_LIMIT:
		_client_limit = atoi(arguments[0].c_str());
		break;
	case ChannelFlags::KEY_CHANNEL:
		setKey(arguments[0], caller);

		if (arguments.size())
			arguments.erase(arguments.begin());
		break;
	default:
		break;
	}
	const char mode = letter;

	log(YELLOW + _name + BLUE + ": " \
		+ GREEN + "Adding" + BLUE + " mode " + mode);

	if (_modes.find(mode) == string::npos)
		_modes += mode;
	else
		log("Channel already had that mode");
}
void Channel::removeMode(ChannelFlags::Mode letter, std::vector<std::string> &arguments, User *caller)
{
	User *user;
	switch (letter)
	{
	case ChannelFlags::OPERATOR:
		user = server->getUser(arguments[0]);
		if (!user)
			return;

		if (arguments.size())
			arguments.erase(arguments.begin());

		_operators.erase(_operators.find(user->getFd()));
		break;
	case ChannelFlags::CLIENT_LIMIT:
		_client_limit = 0;
		break;
	case ChannelFlags::INVITE_ONLY:
		break;
	case ChannelFlags::PROTECTED_TOPIC:
		break;
	case ChannelFlags::KEY_CHANNEL:
		setKey("", caller);
	default:
		break;
	}
	const char mode = letter;

	log(YELLOW + _name + BLUE + ": " \
		+ RED + "Removing" + BLUE + " mode " + mode);

	size_t pos = _modes.find(mode);
	if (pos != string::npos)
		_modes.erase(pos);
	else
		log("Channel didn't have that mode");
}

void Channel::addUser(User *user)
{
	if (!user)
		return;

	log(YELLOW + _name + BLUE + ": " + GREEN + "Adding" + BLUE + " user " \
		+ MAGENTA + toString(user->getFd()));

	_users[user->getFd()] = user;

	if (!_topic.topic.empty())
		user->sendReply(RPL_TOPIC, _name);
	else
		user->sendReply(RPL_NOTOPIC, _name);
}
void Channel::removeUser(User *user)
{
	if (!user)
		return;

	log(string("    ") + YELLOW + _name + RESET + ": " \
		+ RED + "Removing " \
		+ MAGENTA + user->getNick() + RESET + " (" \
		+ MAGENTA + toString(user->getFd()) + RESET + ")");

	if (isOperator(user))
		_operators.erase(_operators.find(user->getFd()));
	_users.erase(_users.find(user->getFd()));
}
void Channel::removeUser(const string &nick)
{
	for (map<int, User *>::iterator i = _users.begin(); i != _users.end(); ++i)
		if (i->second->getNick() == nick)
			return removeUser(i->second);
}

void Channel::ban(User *user)
{
	if (!isBanned(user))
		_banned.insert(pair<int, User *>(user->getFd(), user));
}
void Channel::unban(const User *user)
{
	if (isBanned(user))
		_banned.erase(user->getFd());
}

bool Channel::isMember(const User *user) const { return (_users.find(user->getFd()) != _users.end()); }
bool Channel::isOperator(const User *user) const { return (_operators.find(user->getFd()) != _operators.end()); }
bool Channel::isFull() const { return (_users.size() == _client_limit); }
bool Channel::isBanned(const User *user) const { return (_banned.find(user->getFd()) != _banned.end()); }
bool Channel::isInviteOnly() const { return (_modes.find('i') != string::npos); }
bool Channel::noExternalMessages() const { return (_modes.find('n') != string::npos); }
bool Channel::isTopicProtected() const { return (_modes.find('t') != string::npos); }


void Channel::addInvitedUser(User *user)
{
	if (!isInvitedUser(user))
		_invitations.push_back(user);
}
bool Channel::isInvitedUser(const User *user) const { return (find(_invitations.begin(), _invitations.end(), user) != _invitations.end()); }
void Channel::revokeInvitation(const User *user)
{
	vector<User *>::iterator i = find(_invitations.begin(), _invitations.end(), user);
	if (i != _invitations.end())
		_invitations.erase(i);
}

void Channel::broadcast(const string &msg) const
{
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
	for (map<int, User *>::const_iterator it = _users.begin(); it != _users.end(); it++)
	{
		User *user = it->second;
		if (isMember(user))
		{
			for (map<int, string>::iterator i = _history.begin(); i != _history.end(); i++)
				server->sendMsg(user, i->second);
		}
	}
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
