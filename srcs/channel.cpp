/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fporto <fporto@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/16 13:16:55 by jibanez-          #+#    #+#             */
/*   Updated: 2023/11/08 06:03:27 by fporto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.hpp"

Channel::Channel() {}
Channel::Channel(const std::string &name) : _name(name), _type(name[0])
{
	// const std::string str = WLC_CH_MSG + name + RESET;

	setStatus(ChannelFlags::PUBLIC);

	// set(YELLOW_BG + str);
	if (!_topic.topic.empty())
		set(_topic.topic);
}
Channel::~Channel() {} //? Missing any handling of players' current channel?

// Setters
void Channel::setName(const std::string &name) { _name = name; }
void Channel::setMode(const std::string &modes) { _modes = modes; }
void Channel::setTopic(const std::string &topic, const User *setBy)
{
	_topic.topic = topic;
	_topic.setBy = setBy;
	_topic.setAt = time(NULL);
}
void Channel::setKey(const std::string &key, const User *src)
{
	if (key.find(' ') != std::string::npos)
		return src->sendError(ERR_INVALIDKEY, _name);
		// return src->sendError(ERR_INVALIDKEY, _name);
	_key = key;
}
void Channel::setType(ChannelFlags::Type type) { _type = (char)type; }
void Channel::setStatus(ChannelFlags::Status status) { _status = status; }

void Channel::setClientLimit(const size_t limit) { _client_limit = limit; }
// void Channel::setUserModes(const User *user, const std::string modes) { _user_modes.at(user->getFd()) = modes; }

// Getters
const std::string	Channel::getName() const { return _name; }
const std::string	Channel::getModes() const { return _modes; }
const std::string	Channel::getTopic() const { return _topic.topic; }
const std::string	Channel::getTopicSetBy() const { return _topic.setBy->getNick(); }
const std::string	Channel::getTopicSetAt() const { return toString(_topic.setAt); }
const std::string	Channel::getKey() const { return _key; }
char				Channel::getType() const { return _type; }
char				Channel::getStatus() const { return _status; }

size_t				Channel::getClientLimit() const { return _client_limit; }
// const std::string	Channel::getUserModes(const User *user) const { return _user_modes.at(user->getFd()); }
const std::vector<const User *>	Channel::getUsers() const
{
	std::vector<const User *> users;

	for (std::map<const uint, User *>::const_iterator i = _users.begin(); i != _users.end(); ++i)
		users.push_back(i->second);
	return users;
}
const std::vector<const User *>	Channel::getOperators() const
{
	std::vector<const User *> operators;

	for (std::map<const uint, const User *>::const_iterator i = _operators.begin(); i != _operators.end(); ++i)
		operators.push_back(i->second);
	return operators;
}
const std::vector<const User *>	Channel::getInvitations() const { return _invitations; }

bool Channel::isModeImplemented(ChannelFlags::ModeLetter modeLetter) const
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
void Channel::addMode(ChannelFlags::ModeLetter letter, std::vector<std::string> arguments, const User *caller)
{
	if (!isModeImplemented(letter))
		return;

	switch (letter)
	{
	case ChannelFlags::OPERATOR:
	{
		if (!arguments.size())
			return;

		const User *user = server->getUser(arguments[0]);
		if (!user)
			return;

		_operators.insert(std::pair<int, const User *>(user->getFd(), user));

		log(MAGENTA + arguments[0] + RESET + " is now a channel operator of " + YELLOW + _name);

		arguments.erase(arguments.begin());
		break;
	}
	case ChannelFlags::CLIENT_LIMIT:
		if (!arguments.size())
			return;

		_client_limit = atoi(arguments[0].c_str());

		log(YELLOW + _name + RESET + " now has a client limit of " + YELLOW + toString(_client_limit));

		arguments.erase(arguments.begin());
		break;
	case ChannelFlags::INVITE_ONLY:
		log(YELLOW + _name + RESET + " is now an invite-only channel");
		break;
	case ChannelFlags::PROTECTED_TOPIC:
		log(YELLOW + _name + RESET + "'s topic is now protected");
		break;
	case ChannelFlags::KEY_CHANNEL:
		if (!arguments.size())
			return;

		setKey(arguments[0], caller);

		log(YELLOW + _name + RESET + " is now protected by key " + YELLOW + _key);

		arguments.erase(arguments.begin());
		break;
	default:
		break;
	}
	const char mode = letter;

	log(YELLOW + _name + BLUE + ": " + GREEN + "Adding" + BLUE + " mode " + mode);

	if (_modes.find(mode) == std::string::npos)
		_modes += mode;
	else
		log("Channel already had that mode");
}
void Channel::removeMode(ChannelFlags::ModeLetter letter, std::vector<std::string> &arguments, User *caller)
{
	if (!isModeImplemented(letter))
		return;

	switch (letter)
	{
	case ChannelFlags::OPERATOR:
	{
		const User *user = server->getUser(arguments[0]);
		if (!user)
			return;

		if (arguments.size())
			arguments.erase(arguments.begin());

		_operators.erase(_operators.find(user->getFd()));
		if (_operators.size())
			return;

		log(MAGENTA + arguments[0] + RESET + " is no longer a channel operator of " + YELLOW + _name);
		break;
	}
	case ChannelFlags::CLIENT_LIMIT:
		_client_limit = 0;

		log(YELLOW + _name + RESET + " no longer has a client limit");
		break;
	case ChannelFlags::INVITE_ONLY:
		log(YELLOW + _name + RESET + " is no longer an invite-only channel");
		break;
	case ChannelFlags::PROTECTED_TOPIC:
		log(YELLOW + _name + RESET + "'s topic is no longer protected");
		break;
	case ChannelFlags::KEY_CHANNEL:
		setKey("", caller);

		log(YELLOW + _name + RESET + " is no longer protected by key");
	default:
		break;
	}
	const char mode = letter;

	log(YELLOW + _name + BLUE + ": " \
		+ RED + "Removing" + BLUE + " mode " + mode);

	size_t pos = _modes.find(mode);
	if (pos != std::string::npos)
		_modes.erase(pos);
	else
		log("Channel didn't have that mode");
}

void Channel::addUser(User *user)
{
	if (!user)
		return;

	log(YELLOW + _name + BLUE + ": " + GREEN + "Adding" + BLUE + " user " \
		+ MAGENTA + user->getNick() \
		+ RESET + " (" + MAGENTA + toString(user->getFd()) + RESET + ")");

	_users[user->getFd()] = user;

	if (!_topic.topic.empty())
		user->sendReply(RPL_TOPIC, _name);
	else
		user->sendReply(RPL_NOTOPIC, _name);
}
void Channel::removeUser(const User *user)
{
	if (!user)
		return;

	log(YELLOW + _name + RESET + ": " \
		+ RED + "Removing " \
		+ MAGENTA + user->getNick() + RESET + " (" \
		+ MAGENTA + toString(user->getFd()) + RESET + ")");

	if (isOperator(user))
		_operators.erase(_operators.find(user->getFd()));
	_users.erase(_users.find(user->getFd()));
}
void Channel::removeUser(const std::string &nick)
{
	for (std::map<const uint, User *>::iterator i = _users.begin(); i != _users.end(); ++i)
		if (i->second->getNick() == nick)
			return removeUser(i->second);
}

void Channel::ban(User *user)
{
	if (!isBanned(user))
		_banned.insert(std::pair<int, User *>(user->getFd(), user));
}
void Channel::unban(const User *user)
{
	if (isBanned(user))
		_banned.erase(user->getFd());
}

bool Channel::isMember(const User *user) const { return (_users.find(user->getFd()) != _users.end()); }
bool Channel::isMember(const std::string &nick) const
{
	std::map<const uint, User *>::const_iterator it;
	for (it = _users.begin(); it != _users.end(); ++it)
		if (it->second->getNick() == nick)
			return true;
	return false;
}
bool Channel::isOperator(const User *user) const { return (_operators.find(user->getFd()) != _operators.end()); }
bool Channel::isFull() const { return (_users.size() == _client_limit); }
bool Channel::isBanned(const User *user) const { return (_banned.find(user->getFd()) != _banned.end()); }
bool Channel::isInviteOnly() const { return (_modes.find('i') != std::string::npos); }
bool Channel::noExternalMessages() const { return (_modes.find('n') != std::string::npos); }
bool Channel::isTopicProtected() const { return (_modes.find('t') != std::string::npos); }


void Channel::addInvitedUser(const User *user)
{
	if (!isInvitedUser(user))
		_invitations.push_back(user);
}
bool Channel::isInvitedUser(const User *user) const { return (find(_invitations.begin(), _invitations.end(), user) != _invitations.end()); }
void Channel::revokeInvitation(const User *user)
{
	std::vector<const User *>::iterator i = find(_invitations.begin(), _invitations.end(), user);
	if (i != _invitations.end())
		_invitations.erase(i);
}

void Channel::broadcast(const std::string &msg) const
{
	broadcast(msg, NULL);
}
void Channel::broadcast(const std::string &msg, const User *exclude, const std::string &src) const
{
	broadcast(":" + src + "@" + server->getName() + " " + msg, exclude);
}
void Channel::broadcast(const std::string &msg, const User *exclude) const
{
	for (std::map<const uint, User *>::const_iterator it = _users.begin(); it != _users.end(); ++it)
		if (it->second != exclude)
			server->sendMsg(it->second, msg);
}

void Channel::update()
{
	for (std::map<const uint, User *>::const_iterator it = _users.begin(); it != _users.end(); it++)
	{
		User *user = it->second;
		if (isMember(user))
		{
			for (std::map<int, std::string>::iterator i = _history.begin(); i != _history.end(); i++)
				server->sendMsg(user, i->second);
		}
	}
}

void Channel::set(const std::string &line)
{
	unsigned int last = _history.size();

	_history.insert(std::pair<unsigned int, std::string>(last, line));
	update();
}

void Channel::setLog(const std::string &line)
{
	unsigned int last = _history.size();

	_history.insert(std::pair<unsigned int, std::string>(last, "[ log ]: " + line));
	update();
}

void Channel::setMsg(const std::string &line, const std::string &nick)
{
	unsigned int last = _history.size();

	_history.insert(std::pair<unsigned int, std::string>(last, "[ " + nick + " ]: " + line));
	update();
}

void Channel::remove(const unsigned int i)
{
	_history.erase(i);
	update();
}
